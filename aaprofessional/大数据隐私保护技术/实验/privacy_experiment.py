#!/usr/bin/env python3
from __future__ import annotations

import argparse
import csv
import math
from dataclasses import dataclass
from pathlib import Path

import numpy as np


# 实验参数与结果结构 

# 指定的四个隐私预算。epsilon 越小，隐私保护越强，误差越大
DEFAULT_EPSILONS = (0.1, 1.0, 2.0, 4.0)

# 保存实验结果
@dataclass(frozen=True)
class ExperimentResult:
    scenario: str  # 实验场景：有可信第三方或无可信第三方
    mechanism: str  # 使用的隐私机制：Laplace 或 Unary Encoding
    epsilon: float  # 当前隐私预算
    repetition: int  # 当前是第几次重复实验
    mse: float  # 本次实验得到的平均平方误差
    theoretical_mse_unclipped: float  # 不进行截断时的理论 MSE
    sensitivity: float | None = None  # 拉普拉斯机制使用的全局敏感度
    p: float | None = None  # UE 中真实比特被报告为 1 的概率
    q: float | None = None  # UE 中非真实比特被报告为 1 的概率

# 读取命令行参数，使数据规模和实验次数等设置可以灵活修改
def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)

    # 数据文件每行代表一个用户，行内的整数是该用户购买的商品编号
    parser.add_argument(
        "--data",
        type=Path,
        default=Path("data/simulated_transactions.txt"),
        help="Transaction file: one user per line, item IDs separated by spaces.",
    )
    parser.add_argument("--domain-size", type=int, default=100)

    # 单个用户最多贡献 20 个不同商品，因此中心化频数向量的 add/remove 邻接关系 L1 敏感度为 20
    parser.add_argument("--max-contribution", type=int, default=20)

    # 隐私预算、重复次数和随机种子共同控制实验过程
    parser.add_argument(
        "--epsilons", type=float, nargs="+", default=list(DEFAULT_EPSILONS)
    )
    parser.add_argument("--repetitions", type=int, default=50)
    parser.add_argument("--seed", type=int, default=20260914)
    parser.add_argument("--output-dir", type=Path, default=Path("results"))

    # 默认把估计值截断到合法频数区间；传入该选项可以关闭截断
    parser.add_argument(
        "--no-clipping",
        action="store_true",
        help="Do not clip estimated frequencies to the valid range.",
    )
    return parser.parse_args()


# 读取交易文件，并检查每条用户记录是否满足实验约束

def load_transactions(
    path: Path, domain_size: int, max_contribution: int
) -> list[np.ndarray]:

    if domain_size <= 0 or max_contribution <= 0:
        raise ValueError("domain size and max contribution must be positive")

    transactions: list[np.ndarray] = []
    with path.open("r", encoding="utf-8") as file:
        for line_number, line in enumerate(file, start=1):
            fields = line.split()
            if not fields:
                raise ValueError(f"line {line_number} is an empty transaction")

            # 将一行字符串转成 NumPy 整数数组，便于后续统计
            items = np.asarray([int(field) for field in fields], dtype=np.int64)

            # 商品编号必须在 1 ~ d 之间
            if np.any(items < 1) or np.any(items > domain_size):
                raise ValueError(
                    f"line {line_number} contains an item outside 1..{domain_size}"
                )

            # 同一用户对同一商品只贡献一次，保证统计的是购买人次
            if len(np.unique(items)) != len(items):
                raise ValueError(f"line {line_number} contains duplicate items")

            # 贡献上界既约束数据，也决定拉普拉斯机制的敏感度
            if len(items) > max_contribution:
                raise ValueError(
                    f"line {line_number} exceeds the contribution bound "
                    f"{max_contribution}"
                )
            transactions.append(items)

    if not transactions:
        raise ValueError("the transaction file is empty")
    return transactions

# 计算完整数据中 item1 到 itemd 的精确购买人次
def full_frequency(transactions: list[np.ndarray], domain_size: int) -> np.ndarray:
    counts = np.zeros(domain_size, dtype=np.float64)
    for transaction in transactions:
        # 商品编号从 1 开始，而数组下标从 0 开始，因此先减 1
        counts += np.bincount(transaction - 1, minlength=domain_size)
    return counts

# 从每位用户的交易记录中等概率抽取一个商品
def sample_one_item_per_user(
    transactions: list[np.ndarray], rng: np.random.Generator
) -> np.ndarray:
    return np.asarray(
        [transaction[rng.integers(len(transaction))] for transaction in transactions],
        dtype=np.int64,
    )

# 按照 MSE=(1/d)*sum((f_i-f_i*)^2) 计算平均平方误差
def mean_squared_error(exact: np.ndarray, estimate: np.ndarray) -> float:
    return float(np.mean(np.square(exact - estimate)))

# 把带噪频数限制在 [0, 用户数]，这是不消耗隐私预算的后处理
def clip_frequencies(estimate: np.ndarray, number_of_users: int) -> np.ndarray:
    return np.clip(estimate, 0.0, float(number_of_users))


# 中心化差分隐私 → 使用拉普拉斯机制发布完整交易数据的商品频数
def laplace_frequency_estimate(
    exact_frequency: np.ndarray,
    epsilon: float,
    sensitivity: float,
    rng: np.random.Generator,
    should_clip: bool,
    number_of_users: int,
) -> np.ndarray:

    # 为 item1 到 itemd 的每个频数独立生成拉普拉斯噪声
    noise = rng.laplace(
        loc=0.0, scale=sensitivity / epsilon, size=exact_frequency.shape
    )

    # 发布值 = 精确频数 + 拉普拉斯噪声
    estimate = exact_frequency + noise
    if should_clip:
        estimate = clip_frequencies(estimate, number_of_users)
    return estimate


# 本地化差分隐私 UE 机制 → 根据 epsilon 计算 UE 机制的保留概率 p 和翻转概率 q

def ue_parameters(epsilon: float) -> tuple[float, float]:
    exp_half_epsilon = math.exp(epsilon / 2.0)
    p = exp_half_epsilon / (exp_half_epsilon + 1.0)
    q = 1.0 / (exp_half_epsilon + 1.0)
    return p, q

# 把商品编号转换为长度为 d 的 one-hot 一元编码向量
def unary_encode(sampled_items: np.ndarray, domain_size: int) -> np.ndarray:
    encoded = np.zeros((len(sampled_items), domain_size), dtype=np.uint8)
    encoded[np.arange(len(sampled_items)), sampled_items - 1] = 1
    return encoded

# 在用户本地扰动一元编码，并在聚合端无偏估计商品频数
def ue_frequency_estimate(
    encoded_items: np.ndarray,
    epsilon: float,
    rng: np.random.Generator,
    should_clip: bool,
) -> tuple[np.ndarray, float, float]:
    p, q = ue_parameters(epsilon)

    # 原始比特为 1 时，以概率 p 报告 1；原始比特为 0 时，以概率 q 报告 1。每个用户、每个比特都独立扰动
    report_probabilities = np.where(encoded_items == 1, p, q)
    perturbed_reports = rng.random(encoded_items.shape) < report_probabilities

    # 聚合服务器只接收扰动后的向量，并统计每个位置上 1 的数量 I_i
    reported_one_counts = perturbed_reports.sum(axis=0)

    # E[I_i]=f_i*p+(n-f_i)*q，移项得到无偏估计：
    # f_i_hat=(I_i-n*q)/(p-q)。
    estimate = (reported_one_counts - len(encoded_items) * q) / (p - q)
    if should_clip:
        estimate = clip_frequencies(estimate, len(encoded_items))
    return estimate, p, q

# 计算 UE 无偏估计器在不截断情况下的理论平均方差
def ue_theoretical_mse(
    exact_sampled_frequency: np.ndarray, number_of_users: int, p: float, q: float
) -> float:

    # 真实拥有该值的用户贡献 Bernoulli(p) 方差，其余用户贡献
    # Bernoulli(q) 方差；再除以无偏估计公式分母 (p-q)^2。
    variances = (
        exact_sampled_frequency * p * (1.0 - p)
        + (number_of_users - exact_sampled_frequency) * q * (1.0 - q)
    ) / ((p - q) ** 2)
    return float(np.mean(variances))


# 在全部 epsilon 下重复运行两种机制并收集每轮 MSE
def run_experiments(
    transactions: list[np.ndarray],
    domain_size: int,
    max_contribution: int,
    epsilons: list[float],
    repetitions: int,
    seed: int,
    should_clip: bool,
) -> tuple[list[ExperimentResult], np.ndarray, np.ndarray]:
    if repetitions <= 0:
        raise ValueError("repetitions must be positive")
    if any(epsilon <= 0 for epsilon in epsilons):
        raise ValueError("all privacy budgets must be positive")

    # 固定随机种子，使抽样、噪声和最终结果均可复现
    rng = np.random.default_rng(seed)
    number_of_users = len(transactions)

    # 中心化场景使用完整交易频数作为无噪声基准
    exact_full_frequency = full_frequency(transactions, domain_size)
    sampled_items = sample_one_item_per_user(transactions, rng)

    exact_sampled_frequency = np.bincount(
        sampled_items - 1, minlength=domain_size
    ).astype(np.float64)
    encoded_items = unary_encode(sampled_items, domain_size)

    results: list[ExperimentResult] = []
    for epsilon in epsilons:
        # 拉普拉斯变量的方差为 2b^2，其中 b=sensitivity/epsilon
        central_theoretical_mse = 2.0 * (max_contribution / epsilon) ** 2

        # 提前计算当前 epsilon 下 UE 的概率参数和理论误差
        p, q = ue_parameters(epsilon)
        local_theoretical_mse = ue_theoretical_mse(
            exact_sampled_frequency, number_of_users, p, q
        )

        for repetition in range(1, repetitions + 1):
            # 场景一：可信第三方先统计精确频数，再统一加入拉普拉斯噪声。
            central_estimate = laplace_frequency_estimate(
                exact_frequency=exact_full_frequency,
                epsilon=epsilon,
                sensitivity=float(max_contribution),
                rng=rng,
                should_clip=should_clip,
                number_of_users=number_of_users,
            )
            results.append(
                ExperimentResult(
                    scenario="trusted_third_party",
                    mechanism="laplace",
                    epsilon=epsilon,
                    repetition=repetition,
                    mse=mean_squared_error(exact_full_frequency, central_estimate),
                    theoretical_mse_unclipped=central_theoretical_mse,
                    sensitivity=float(max_contribution),
                )
            )

            # 场景二：每位用户在本地扰动一元编码，服务器只聚合扰动结果。
            local_estimate, p, q = ue_frequency_estimate(
                encoded_items=encoded_items,
                epsilon=epsilon,
                rng=rng,
                should_clip=should_clip,
            )
            results.append(
                ExperimentResult(
                    scenario="no_trusted_third_party",
                    mechanism="unary_encoding",
                    epsilon=epsilon,
                    repetition=repetition,
                    mse=mean_squared_error(exact_sampled_frequency, local_estimate),
                    theoretical_mse_unclipped=local_theoretical_mse,
                    p=p,
                    q=q,
                )
            )

    return results, exact_full_frequency, exact_sampled_frequency


#  MSE 汇总与结果保存 

def summarize_results(results: list[ExperimentResult]) -> list[dict[str, object]]:
    """按场景、机制和 epsilon 分组，计算 MSE 均值与标准差。"""

    groups: dict[tuple[str, str, float], list[ExperimentResult]] = {}
    for result in results:
        key = (result.scenario, result.mechanism, result.epsilon)
        groups.setdefault(key, []).append(result)

    summaries: list[dict[str, object]] = []
    for (scenario, mechanism, epsilon), group in groups.items():
        mse_values = np.asarray([result.mse for result in group])

        # mean_mse 用于最终曲线，std_mse 反映多次随机实验的波动程度。
        summaries.append(
            {
                "scenario": scenario,
                "mechanism": mechanism,
                "epsilon": epsilon,
                "repetitions": len(group),
                "mean_mse": float(mse_values.mean()),
                "std_mse": float(mse_values.std(ddof=1)) if len(group) > 1 else 0.0,
                "theoretical_mse_unclipped": group[0].theoretical_mse_unclipped,
            }
        )
    return summaries


def write_csv(path: Path, rows: list[dict[str, object]], fieldnames: list[str]) -> None:
    """通用 CSV 写入函数，先写列名，再写每一行结果。"""

    with path.open("w", encoding="utf-8", newline="") as file:
        writer = csv.DictWriter(file, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)


def write_outputs(
    output_dir: Path,
    results: list[ExperimentResult],
    exact_full_frequency: np.ndarray,
    exact_sampled_frequency: np.ndarray,
) -> tuple[Path, Path, Path]:
    """保存逐轮结果、MSE 汇总和精确频数基准三个文件。"""

    output_dir.mkdir(parents=True, exist_ok=True)

    # experiment_runs 保存每一次重复实验，便于检查随机波动。
    runs_path = output_dir / "experiment_runs.csv"

    # experiment_summary 保存每个 epsilon 的均值和标准差，用于绘图和报告。
    summary_path = output_dir / "experiment_summary.csv"

    # baseline_frequencies 保存两种场景各自使用的无噪声精确频数。
    baseline_path = output_dir / "baseline_frequencies.csv"

    # 将 dataclass 对象转换成适合 CSV 输出的字典。
    run_rows = [
        {
            "scenario": result.scenario,
            "mechanism": result.mechanism,
            "epsilon": result.epsilon,
            "repetition": result.repetition,
            "mse": result.mse,
            "theoretical_mse_unclipped": result.theoretical_mse_unclipped,
            "sensitivity": "" if result.sensitivity is None else result.sensitivity,
            "p": "" if result.p is None else result.p,
            "q": "" if result.q is None else result.q,
        }
        for result in results
    ]
    write_csv(
        runs_path,
        run_rows,
        [
            "scenario",
            "mechanism",
            "epsilon",
            "repetition",
            "mse",
            "theoretical_mse_unclipped",
            "sensitivity",
            "p",
            "q",
        ],
    )

    # 汇总全部重复实验后写入第二个 CSV。
    summaries = summarize_results(results)
    write_csv(
        summary_path,
        summaries,
        [
            "scenario",
            "mechanism",
            "epsilon",
            "repetitions",
            "mean_mse",
            "std_mse",
            "theoretical_mse_unclipped",
        ],
    )

    # 同时保存 item1 到 itemd 的完整数据频数和抽样数据频数。
    baseline_rows = [
        {
            "item_id": item_id + 1,
            "full_data_frequency": int(exact_full_frequency[item_id]),
            "sampled_data_frequency": int(exact_sampled_frequency[item_id]),
        }
        for item_id in range(len(exact_full_frequency))
    ]
    write_csv(
        baseline_path,
        baseline_rows,
        ["item_id", "full_data_frequency", "sampled_data_frequency"],
    )
    return runs_path, summary_path, baseline_path


# ------------------------- 7. 主程序入口 -------------------------

def main() -> None:
    """依次完成参数读取、数据加载、实验运行、结果保存和信息打印。"""

    # 第一步：读取命令行参数并加载交易数据。
    args = parse_args()
    transactions = load_transactions(
        args.data, args.domain_size, args.max_contribution
    )

    # 第二步：运行中心化 DP 和本地化 DP 实验。
    results, exact_full_frequency, exact_sampled_frequency = run_experiments(
        transactions=transactions,
        domain_size=args.domain_size,
        max_contribution=args.max_contribution,
        epsilons=args.epsilons,
        repetitions=args.repetitions,
        seed=args.seed,
        should_clip=not args.no_clipping,
    )

    # 第三步：将实验结果写入指定文件夹。
    paths = write_outputs(
        args.output_dir, results, exact_full_frequency, exact_sampled_frequency
    )

    # 最后在终端输出运行概况和三个结果文件的位置。
    print(f"Loaded {len(transactions)} user transactions from {args.data}")
    print(
        "Central DP: Laplace mechanism, "
        f"add/remove sensitivity = {args.max_contribution}"
    )
    print("Local DP: sample one item per user, then apply unary encoding")
    print(f"Per-run results: {paths[0]}")
    print(f"MSE summary: {paths[1]}")
    print(f"Exact frequency baselines: {paths[2]}")


if __name__ == "__main__":
    # 只有直接运行本文件时才执行 main；被其他模块导入时不会自动实验。
    main()
