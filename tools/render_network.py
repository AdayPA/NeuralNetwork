#!/usr/bin/env python3

from __future__ import annotations

import argparse
from pathlib import Path


def read_topology(path: Path) -> list[int]:
    lines = [line.strip() for line in path.read_text(encoding="utf-8").splitlines() if line.strip()]
    if not lines:
        raise ValueError("topology file is empty")
    return [int(part) for part in lines[0].split()]


def build_dot(topology: list[int]) -> str:
    lines = [
        "digraph G {",
        "  rankdir=LR;",
        "  splines=true;",
        "  nodesep=0.5;",
        "  ranksep=1.0;",
        "  node [shape=circle, fixedsize=true, width=0.45, fontsize=10];",
    ]

    for layer_idx, count in enumerate(topology):
        lines.append(f'  subgraph cluster_{layer_idx} {{')
        lines.append('    style="invis";')
        lines.append(f'    label="Layer {layer_idx} ({count})";')
        for neuron_idx in range(count):
            lines.append(f'    L{layer_idx}N{neuron_idx} [label="{layer_idx}:{neuron_idx}"];')
        lines.append("  }")

    for layer_idx in range(len(topology) - 1):
        for src_idx in range(topology[layer_idx]):
            for dst_idx in range(topology[layer_idx + 1]):
                lines.append(f"  L{layer_idx}N{src_idx} -> L{layer_idx + 1}N{dst_idx};")

    lines.append("}")
    return "\n".join(lines)


def main() -> None:
    parser = argparse.ArgumentParser(description="Render a Graphviz DOT file for the network topology.")
    parser.add_argument("--topology", required=True, help="Path to the topology file")
    parser.add_argument("--out", default="network.dot", help="Output DOT path")
    args = parser.parse_args()

    topology = read_topology(Path(args.topology))
    dot = build_dot(topology)
    Path(args.out).write_text(dot, encoding="utf-8")
    print(f"saved {args.out}")


if __name__ == "__main__":
    main()
