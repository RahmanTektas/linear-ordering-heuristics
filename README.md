# Linear Ordering Problem — Local Search and VND

C implementation and experimental comparison of heuristic methods for the **Linear Ordering Problem (LOP)**.

The project was completed for ULB's Heuristic Optimization course.

## Problem

Given a weighted directed graph, LOP asks for a permutation that maximizes the total weight of forward arcs. Since the problem is NP-hard, this project compares several local-search strategies rather than solving large instances exactly.

## Implemented methods

**Initial solutions**
- random permutation
- constructive CW initialization

**Neighborhoods**
- transpose
- exchange
- insert

**Search strategies**
- first improvement
- best improvement
- Variable Neighborhood Descent (VND)

## Evaluation

The experiments compare objective quality and runtime across benchmark instances against best-known objective values.

Twelve single-neighborhood configurations were evaluated. In the recorded experiment summary, the strongest single-neighborhood configuration averaged about **2.0% deviation from the best-known values**.

The analysis also includes repeated runs and pairwise statistical comparisons in R.

## Repository layout

```text
src/          C implementation
instances/    benchmark instances
best_known/   reference objective values
scripts/      experiment automation and R analysis
results/      raw and aggregated results
report/       LaTeX report sources
Makefile      build commands
```

## Build and run

```bash
make
./lop --cw --best --insert -i instances/INSTANCE
```

Example VND runs:

```bash
./lop --cw --vnd-tei -i instances/INSTANCE
./lop --cw --vnd-tie -i instances/INSTANCE
```

## Reproduce the experiments

```bash
cd scripts
chmod +x run_experiments_exercise1.sh
chmod +x run_experiments_exercise2.sh
./run_experiments_exercise1.sh
./run_experiments_exercise2.sh
```

The repository includes the solver, experiment scripts, raw results, statistical analysis, and final report so the comparisons can be reproduced from the implementation.


## Attribution

The course starter code is adapted from the ILSLOP implementation by Tommaso Schiavinotto, as documented in the source headers. The coursework work in this repository adds and evaluates the local-search/VND implementations, experiment automation, statistical analysis, and report. The inherited C source headers specify the GNU GPL v3 or later.
