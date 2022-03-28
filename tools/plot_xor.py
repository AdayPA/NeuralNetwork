#!/usr/bin/env python3

from __future__ import annotations

import argparse
from pathlib import Path

try:
    import matplotlib.pyplot as plt
except ModuleNotFoundError as exc:
    raise SystemExit("matplotlib is required for plotting: python3 -m pip install matplotlib") from exc


def resolve_error_log(path: Path) -> Path:
    rows: list[tuple[float, Path]] = []
    with path.open("r", encoding="utf-8") as fh:
        for line in fh:
            parts = line.split()
            if len(parts) < 4:
                continue
            try:
                error = float(parts[1])
            except ValueError:
                continue

            candidate = Path(parts[-1])
            if candidate.exists():
                rows.append((error, candidate))

    if not rows:
        return path

    return min(rows, key=lambda item: item[0])[1]


def read_log(path: Path) -> list[float]:
    values: list[float] = []
    with path.open("r", encoding="utf-8") as fh:
        for line in fh:
            line = line.strip()
            if not line:
                continue
            try:
                values.append(float(line.split()[-1]))
            except ValueError:
                continue
    return values


def read_predictions(path: Path) -> list[tuple[str, float]]:
    rows: list[tuple[str, float]] = []
    with path.open("r", encoding="utf-8") as fh:
        for line in fh:
            line = line.strip()
            if not line or "->" not in line:
                continue
            left, right = line.split("->", 1)
            rows.append((left.strip(), float(right.strip())))
    return rows


def main() -> None:
    parser = argparse.ArgumentParser(description="Plot XOR training error and predictions.")
    parser.add_argument("--log", required=True, help="Path to the training error log file")
    parser.add_argument("--predictions", help="Optional path to a text file with 'x y -> value' lines")
    parser.add_argument("--out", default="xor_plot.png", help="Output image path")
    args = parser.parse_args()

    log_path = Path(args.log)
    error_log_path = resolve_error_log(log_path)
    errors = read_log(error_log_path)

    fig, axes = plt.subplots(2 if args.predictions else 1, 1, figsize=(10, 6 if args.predictions else 4))
    if args.predictions:
        axes = axes.ravel().tolist()
    else:
        axes = [axes]

    ax0 = axes[0]
    ax0.plot(errors, linewidth=1.0)
    ax0.set_title("Training Error")
    ax0.set_xlabel("Iteration")
    ax0.set_ylabel("Error")
    ax0.grid(True, alpha=0.3)

    if args.predictions:
        preds = read_predictions(Path(args.predictions))
        ax1 = axes[1]
        labels = [item[0] for item in preds]
        values = [item[1] for item in preds]
        ax1.bar(labels, values)
        ax1.set_ylim(0.0, 1.0)
        ax1.set_title("XOR Predictions")
        ax1.set_ylabel("Output")
        ax1.grid(True, axis="y", alpha=0.3)

    fig.tight_layout()
    fig.savefig(args.out, dpi=160)
    print(f"saved {args.out}")


if __name__ == "__main__":
    main()
