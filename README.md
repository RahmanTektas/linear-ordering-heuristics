INFO-H-413 Heuristic Optimization
Implementation Exercise 1
Author: Rahman Tektas

This project implements the algorithms required for Implementation Exercise 1 on the Linear Ordering Problem (LOP).

Files
-----
- src/                  source code
- instances/            LOP instances
- best_known/           best known solution values
- results/              raw data files used for the analysis, the exercise1_results.txt and exercise2_results.txt are the main files to store results.
- scripts/              scripts folder to generate results, run experiments, statsic analysis
- report/               folder containing the report material in latex format
- Makefile              compilation
- lop                   executable generated after compilation
- report.pdf            report in PDF



Compilation
-----------
Run:

    make

This creates the executable:

    ./lop

To clean compiled files, run:

    make clean

Usage
-----
Exercise 1.1 algorithms:

    ./lop --random --first --transpose -i instances/INSTANCE
    ./lop --random --first --exchange -i instances/INSTANCE
    ./lop --random --first --insert -i instances/INSTANCE

    ./lop --random --best --transpose -i instances/INSTANCE
    ./lop --random --best --exchange -i instances/INSTANCE
    ./lop --random --best --insert -i instances/INSTANCE

    ./lop --cw --first --transpose -i instances/INSTANCE
    ./lop --cw --first --exchange -i instances/INSTANCE
    ./lop --cw --first --insert -i instances/INSTANCE

    ./lop --cw --best --transpose -i instances/INSTANCE
    ./lop --cw --best --exchange -i instances/INSTANCE
    ./lop --cw --best --insert -i instances/INSTANCE

Exercise 1.2 algorithms:

    ./lop --cw --vnd-tei -i instances/INSTANCE
    ./lop --cw --vnd-tie -i instances/INSTANCE

Exercise 2

Options
-------
Initialization:
- --random
- --cw

Pivoting rules:
- --first
- --best

Neighborhoods:
- --transpose
- --exchange
- --insert

VND:
- --vnd-tei   transpose -> exchange -> insert
- --vnd-tie   transpose -> insert -> exchange

Run all experiments
-------------------
First make the script executable:

    chmod +x run_experiments_exercise1.sh
    chmod +x run_experiments_exercise2.sh


Then go to scripts folder and run:

    ./run_experiments_exercise1.sh
    ./run_experiments_exercise2.sh


This generates the raw data files used for the analysis.

Then we can generate the summary by running:
    Rscript generate_summary_exercise1.R

Statistical analysis
--------------------
The raw data can be analyzed in R to compute:
- average percentage deviation from best known solutions
- total computation time
- standard deviations
- statistical tests

First make the script executable:
    chmod +x make_r_files.sh

Then run:
    ./make_r_files.sh

Then go to scripts folder and run:
    Rscript test_ex1.R
    Rscript test_ex2.R
    Rscript test_ex1_all_wilcoxon.R

macOS note:
Rscript should work the same way on macOS if R is installed and available in the terminal. If it is not found, install R and check that Rscript is in your PATH.


Submission
----------
The submission contains:
- report in PDF
- source code
- scripts to generate results inside scripts/ folder
- README.txt
- raw data used for statistical tests inside results/ folder
