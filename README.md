# NeuralNetwork

A small C++ feed-forward neural network that trains with backpropagation on the XOR problem. The project includes a Makefile plus helper scripts to inspect training results and render the network topology.

## Project Notes

This repository is kept as a compact educational example of a neural network implemented from scratch in C++. The README documents how to build, run, inspect, and clean the project without committing generated binaries or training outputs.

## Features

- Configurable network topologies from `data/input/topology.txt`
- XOR training samples from `data/input/xor.txt`
- Backpropagation with momentum
- Training logs written under `data/output/`
- Optional Python plot for training error and predictions
- Optional Graphviz render of the configured topology

## Requirements

- `g++` with C++17 support
- `make`
- Optional for plots: Python 3 with `matplotlib`
- Optional for topology PNGs: Graphviz (`dot`)

## Build

```bash
make
```

This builds the executable:

```text
ann
```

## Run

```bash
make run
```

The command trains every topology listed in `data/input/topology.txt` and writes XOR predictions to `predictions.txt`.

Example output:

```text
0 0 -> 0
0 1 -> 0.999
1 0 -> 0.999
1 1 -> 0
```

Training error files are written to `data/output/data/`, and the run summary is written to `data/output/log/xor.log`.

## Plot Results

```bash
make plot
```

This creates `xor_plot.png` from the run summary and prediction output.

## Render Network Topology

```bash
make network
```

This creates:

- `network.dot`
- `network.png`

## Input Files

`data/input/topology.txt` contains one topology per line:

```text
2 2 1
2 3 1
2 4 1
```

`data/input/xor.txt` stores alternating input and target-output lines:

```text
0.0 1.0
1.0
1.0 1.0
0.0
```

## Clean

```bash
make clean
```

Removes generated binaries and visualization files.

```bash
make distclean
```

Also removes `data/output/`.
