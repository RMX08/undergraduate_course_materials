#!/usr/bin/env python3
"""Create the MSE comparison chart and a Markdown result table."""

from __future__ import annotations

import argparse
import csv
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import font_manager


SCENARIO_LABELS = {
    "trusted_third_party": "可信第三方（拉普拉斯机制）",
    "no_trusted_third_party": "无可信第三方（UE 机制）",
}

SCENARIO_COLORS = {
    "trusted_third_party": "#0072B2",
    "no_trusted_third_party": "#D55E00",
}

SCENARIO_MARKERS = {
    "trusted_third_party": "o",
    "no_trusted_third_party": "s",
}

# 依次尝试常见中文字体，确保中文标题和图例能够正常显示。
CHINESE_FONT_CANDIDATES = (
    Path("/mnt/c/Windows/Fonts/msyh.ttc"),
    Path("/mnt/c/Windows/Fonts/simhei.ttf"),
    Path("/mnt/c/Windows/Fonts/simsun.ttc"),
)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--summary",
        type=Path,
        default=Path("results/experiment_summary.csv"),
    )
    parser.add_argument("--output-dir", type=Path, default=Path("results"))
    return parser.parse_args()


def read_summary(path: Path) -> dict[str, list[dict[str, float]]]:
    """Read summary rows and group them by privacy scenario."""
    grouped: dict[str, list[dict[str, float]]] = {}
    with path.open("r", encoding="utf-8", newline="") as file:
        for row in csv.DictReader(file):
            scenario = row["scenario"]
            grouped.setdefault(scenario, []).append(
                {
                    "epsilon": float(row["epsilon"]),
                    "mean_mse": float(row["mean_mse"]),
                    "std_mse": float(row["std_mse"]),
                    "repetitions": float(row["repetitions"]),
                }
            )

    expected = set(SCENARIO_LABELS)
    if set(grouped) != expected:
        raise ValueError(f"expected scenarios {sorted(expected)}, got {sorted(grouped)}")
    for rows in grouped.values():
        rows.sort(key=lambda row: row["epsilon"])
    return grouped


def find_chinese_font() -> Path:
    """返回当前系统中第一个可用的中文字体文件。"""
    for font_path in CHINESE_FONT_CANDIDATES:
        if font_path.is_file():
            return font_path
    raise FileNotFoundError("未找到可用中文字体，请安装微软雅黑、黑体或宋体")


def create_chart(
    grouped: dict[str, list[dict[str, float]]], output_dir: Path
) -> tuple[Path, Path]:
    """Draw mean MSE with one-standard-deviation error bars."""
    chinese_font_path = find_chinese_font()
    chinese_font = font_manager.FontProperties(fname=chinese_font_path)
    legend_font = font_manager.FontProperties(fname=chinese_font_path, size=9.5)

    plt.rcParams.update(
        {
            "font.family": "DejaVu Sans",
            "font.size": 10,
            "axes.titlesize": 13,
            "axes.labelsize": 11,
            "legend.fontsize": 9.5,
        }
    )
    figure, axis = plt.subplots(figsize=(7.2, 4.8), constrained_layout=True)

    for scenario in ("trusted_third_party", "no_trusted_third_party"):
        rows = grouped[scenario]
        epsilons = np.asarray([row["epsilon"] for row in rows])
        means = np.asarray([row["mean_mse"] for row in rows])
        standard_deviations = np.asarray([row["std_mse"] for row in rows])
        axis.errorbar(
            epsilons,
            means,
            yerr=standard_deviations,
            label=SCENARIO_LABELS[scenario],
            color=SCENARIO_COLORS[scenario],
            marker=SCENARIO_MARKERS[scenario],
            markersize=6,
            linewidth=2,
            capsize=4,
            capthick=1.2,
        )

    axis.set_yscale("log")
    axis.set_xticks([0.1, 1.0, 2.0, 4.0], labels=["0.1", "1", "2", "4"])
    axis.set_xlabel("隐私预算 ε", fontproperties=chinese_font, fontsize=11)
    axis.set_ylabel("平均平方误差（对数刻度）", fontproperties=chinese_font, fontsize=11)
    axis.set_title(
        "差分隐私下的商品频数估计误差",
        fontproperties=chinese_font,
        fontsize=13,
    )
    axis.grid(axis="y", which="both", color="#D6DADF", linewidth=0.7, alpha=0.8)
    axis.grid(axis="x", which="major", color="#ECEFF1", linewidth=0.6)
    axis.legend(frameon=False, loc="upper right", prop=legend_font)
    axis.spines["top"].set_visible(False)
    axis.spines["right"].set_visible(False)

    output_dir.mkdir(parents=True, exist_ok=True)
    png_path = output_dir / "mse_comparison.png"
    pdf_path = output_dir / "mse_comparison.pdf"
    figure.savefig(png_path, dpi=300, facecolor="white")
    figure.savefig(pdf_path, facecolor="white")
    plt.close(figure)
    return png_path, pdf_path


def write_markdown_table(
    grouped: dict[str, list[dict[str, float]]], output_dir: Path
) -> Path:
    """Write a Chinese result table and concise observations for the report."""
    central_by_epsilon = {
        row["epsilon"]: row for row in grouped["trusted_third_party"]
    }
    local_by_epsilon = {
        row["epsilon"]: row for row in grouped["no_trusted_third_party"]
    }
    epsilons = sorted(central_by_epsilon)
    if epsilons != sorted(local_by_epsilon):
        raise ValueError("the two scenarios use different epsilon values")

    repetitions = int(central_by_epsilon[epsilons[0]]["repetitions"])
    lines = [
        "# 第三步：实验结果",
        "",
        f"实验在每个隐私预算下独立重复 {repetitions} 次。表中结果为 MSE 均值 ± 标准差，频数估计采用合法区间截断后处理。",
        "",
        "| 隐私预算 ε | 可信第三方：Laplace | 无可信第三方：UE | UE/Laplace MSE 比值 |",
        "|---:|---:|---:|---:|",
    ]

    for epsilon in epsilons:
        central = central_by_epsilon[epsilon]
        local = local_by_epsilon[epsilon]
        ratio = local["mean_mse"] / central["mean_mse"]
        lines.append(
            f"| {epsilon:g} | {central['mean_mse']:,.2f} ± {central['std_mse']:,.2f} "
            f"| {local['mean_mse']:,.2f} ± {local['std_mse']:,.2f} | {ratio:.2f} |"
        )

    central_reduction = (
        1.0
        - central_by_epsilon[epsilons[-1]]["mean_mse"]
        / central_by_epsilon[epsilons[0]]["mean_mse"]
    ) * 100.0
    local_reduction = (
        1.0
        - local_by_epsilon[epsilons[-1]]["mean_mse"]
        / local_by_epsilon[epsilons[0]]["mean_mse"]
    ) * 100.0

    lines.extend(
        [
            "",
            "## 结果分析",
            "",
            "1. 两种场景的 MSE 都随 ε 增大而持续下降，说明放宽隐私保护强度能够提高频数估计精度。",
            "2. 在所有 ε 下，可信第三方场景的 MSE 都小于无可信第三方场景。UE 需要每位用户在本地独立扰动 100 位一元编码，因此累积误差更大。",
            f"3. ε 从 {epsilons[0]:g} 增至 {epsilons[-1]:g} 时，Laplace 的平均 MSE 下降 {central_reduction:.2f}%，UE 的平均 MSE 下降 {local_reduction:.2f}%。",
            "4. ε=0.1 时隐私保护最强，两种机制的扰动最明显；ε=4 时两种机制均取得本实验中的最低 MSE。",
            "",
            "图中纵轴采用对数刻度，误差棒表示 50 次实验 MSE 的一个标准差。",
        ]
    )

    output_dir.mkdir(parents=True, exist_ok=True)
    markdown_path = output_dir / "experiment_results.md"
    markdown_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    return markdown_path


def main() -> None:
    args = parse_args()
    grouped = read_summary(args.summary)
    png_path, pdf_path = create_chart(grouped, args.output_dir)
    markdown_path = write_markdown_table(grouped, args.output_dir)
    print(f"PNG chart: {png_path}")
    print(f"PDF chart: {pdf_path}")
    print(f"Result table: {markdown_path}")


if __name__ == "__main__":
    main()
