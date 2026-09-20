# Linear Ordering Problem — Local Search and VND

A C implementation and empirical study of local-search heuristics for the Linear Ordering Problem (LOP).

Developed by Rahman Tektas for the ULB Heuristic Optimization course.

## Problem

Given a weighted directed graph, the Linear Ordering Problem asks for a permutation that maximizes the total weight of forward arcs:

    maximize  sum c[pi(i), pi(j)]  for every i < j

LOP is NP-hard, so this project focuses on fast heuristic methods and on measuring their solution quality against best-known values.

## Implemented methods

### Initial solutions

- random permutations
- a constructive CW initialization strategy

### Neighborhoods

- transpose: swap adjacent elements
- exchange: swap any two positions
- insert: remove one element and insert it elsewhere

### Search policies

- first improvement
- best improvement
- Variable Neighborhood Descent with two neighborhood orders:
  - transpose → exchange → insert
  - transpose → insert → exchange

## Experimental workflow

The repository contains the complete path from implementation to analysis:

1. compile the solver in C;
2. run heuristic combinations across benchmark instances;
3. record objective values and execution times;
4. compare solutions with best-known values;
5. aggregate results in R;
6. run statistical comparisons and generate report material.

## Repository structure

| Path | Purpose |
| --- | --- |
| src/ | C implementation of the solver and neighborhoods |
| instances/ | Benchmark LOP instances |
| best_known/ | Reference objective values |
| scripts/ | Experiment automation and R analysis |
| results/ | Raw outputs and aggregated measurements |
| report/ | LaTeX sources for the technical report |
| report.pdf | Final experimental report |
| Makefile | Reproducible build commands |

## Build

    make

This produces the command-line solver:

    ./lop

Clean generated build files with:

    make clean

## Usage

Examples for individual local searches:

    ./lop --random --first --transpose -i instances/INSTANCE
    ./lop --random --best --exchange -i instances/INSTANCE
    ./lop --cw --best --insert -i instances/INSTANCE

Examples for Variable Neighborhood Descent:

    ./lop --cw --vnd-tei -i instances/INSTANCE
    ./lop --cw --vnd-tie -i instances/INSTANCE

The CLI separates initialization, pivoting rule, and neighborhood choice so experiments can compare one design decision at a time.

## Reproducing the experiments

From the scripts directory:

    chmod +x run_experiments_exercise1.sh
    chmod +x run_experiments_exercise2.sh
    ./run_experiments_exercise1.sh
    ./run_experiments_exercise2.sh

Generate summaries and statistical tests with R:

    Rscript generate_summary_exercise1.R
    Rscript test_ex1.R
    Rscript test_ex2.R
    Rscript test_ex1_all_wilcoxon.R

## Evaluation

The analysis tracks:

- deviation from the best-known objective value
- computation time
- variability across repeated runs
- pairwise statistical comparisons between heuristic configurations

This makes the project an experimental engineering study rather than only an algorithm implementation.

## What this project demonstrates

- translating combinatorial-optimization ideas into efficient C
- designing modular neighborhood and pivoting strategies
- building reproducible benchmark pipelines
- separating raw results from analysis code
- using statistical evidence to compare heuristic algorithms
