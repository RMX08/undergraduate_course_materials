#!/usr/bin/env python3
"""Generate a reproducible synthetic transaction dataset for the DP experiment."""

from __future__ import annotations

import argparse
import csv
import json
import math
import random
from collections import Counter
from pathlib import Path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--users", type=int, default=10_000)
    parser.add_argument("--items", type=int, default=100)
    parser.add_argument("--max-transaction-length", type=int, default=20)
    parser.add_argument("--mean-extra-items", type=float, default=4.5)
    parser.add_argument("--zipf-exponent", type=float, default=1.1)
    parser.add_argument("--categories", type=int, default=10)
    parser.add_argument("--category-boost", type=float, default=4.0)
    parser.add_argument("--seed", type=int, default=20260914)
    parser.add_argument("--output-dir", type=Path, default=Path("data"))
    return parser.parse_args()


def poisson_sample(mean: float, rng: random.Random) -> int:
    """Draw from a Poisson distribution using Knuth's method."""
    threshold = math.exp(-mean)
    product = 1.0
    count = 0
    while product > threshold:
        count += 1
        product *= rng.random()
    return count - 1


def weighted_sample_without_replacement(
    weights: list[float], sample_size: int, rng: random.Random
) -> list[int]:
    """Return 1-based item IDs sampled according to positive weights."""
    keys = [(-math.log(max(rng.random(), 1e-15)) / weight, index + 1)
            for index, weight in enumerate(weights)]
    keys.sort()
    return [item_id for _, item_id in keys[:sample_size]]


def build_transactions(args: argparse.Namespace) -> list[list[int]]:
    if args.users <= 0 or args.items <= 0:
        raise ValueError("users and items must be positive")
    if not 1 <= args.max_transaction_length <= args.items:
        raise ValueError("max transaction length must be between 1 and items")
    if not 1 <= args.categories <= args.items:
        raise ValueError("categories must be between 1 and items")
    if args.mean_extra_items < 0 or args.zipf_exponent <= 0 or args.category_boost <= 0:
        raise ValueError("distribution parameters must be positive")

    rng = random.Random(args.seed)
    base_weights = [1.0 / (rank ** args.zipf_exponent)
                    for rank in range(1, args.items + 1)]
    transactions: list[list[int]] = []

    for _ in range(args.users):
        transaction_length = min(
            1 + poisson_sample(args.mean_extra_items, rng),
            args.max_transaction_length,
        )
        preferred_category = rng.randrange(args.categories)
        user_weights = []
        for index, base_weight in enumerate(base_weights):
            category = min(index * args.categories // args.items, args.categories - 1)
            boost = args.category_boost if category == preferred_category else 1.0
            user_weights.append(base_weight * boost)

        transaction = weighted_sample_without_replacement(
            user_weights, transaction_length, rng
        )
        transactions.append(sorted(transaction))

    return transactions


def write_outputs(
    args: argparse.Namespace, transactions: list[list[int]]
) -> tuple[Path, Path, Path]:
    output_dir = args.output_dir
    output_dir.mkdir(parents=True, exist_ok=True)
    transactions_path = output_dir / "simulated_transactions.txt"
    frequencies_path = output_dir / "true_item_frequencies.csv"
    metadata_path = output_dir / "simulated_transactions_metadata.json"

    with transactions_path.open("w", encoding="utf-8") as file:
        for transaction in transactions:
            file.write(" ".join(map(str, transaction)) + "\n")

    frequencies = Counter(item for transaction in transactions for item in transaction)
    with frequencies_path.open("w", encoding="utf-8", newline="") as file:
        writer = csv.writer(file)
        writer.writerow(["item_id", "true_frequency"])
        for item_id in range(1, args.items + 1):
            writer.writerow([item_id, frequencies[item_id]])

    lengths = [len(transaction) for transaction in transactions]
    metadata = {
        "format": "one user transaction per line; unique item IDs separated by spaces",
        "parameters": {
            "number_of_users": args.users,
            "item_domain": [1, args.items],
            "transaction_length_model": "1 + Poisson(mean_extra_items), clipped at max_transaction_length",
            "mean_extra_items": args.mean_extra_items,
            "max_transaction_length": args.max_transaction_length,
            "item_popularity_model": "Zipf distribution with a per-user preferred-category boost",
            "zipf_exponent": args.zipf_exponent,
            "number_of_categories": args.categories,
            "preferred_category_boost": args.category_boost,
            "random_seed": args.seed,
        },
        "observed_statistics": {
            "number_of_transactions": len(transactions),
            "total_item_occurrences": sum(lengths),
            "minimum_transaction_length": min(lengths),
            "maximum_transaction_length": max(lengths),
            "mean_transaction_length": sum(lengths) / len(lengths),
            "items_with_nonzero_frequency": len(frequencies),
        },
        "privacy_analysis_note": (
            "The configured maximum transaction length is the per-user clipping bound "
            "used later for central-DP sensitivity analysis."
        ),
    }
    with metadata_path.open("w", encoding="utf-8") as file:
        json.dump(metadata, file, ensure_ascii=False, indent=2)
        file.write("\n")

    return transactions_path, frequencies_path, metadata_path


def main() -> None:
    args = parse_args()
    transactions = build_transactions(args)
    paths = write_outputs(args, transactions)
    print(f"Generated {len(transactions)} transactions in {paths[0]}")
    print(f"True frequencies: {paths[1]}")
    print(f"Metadata: {paths[2]}")


if __name__ == "__main__":
    main()
