/*  Heuristic Optimization assignment, 2015.
    Adapted by Jérémie Dubois-Lacoste from the ILSLOP implementation
    of Tommaso Schiavinotto:
    ---
    ILSLOP Iterated Local Search Algorithm for Linear Ordering Problem
    Copyright (C) 2004  Tommaso Schiavinotto (tommaso.schiavinotto@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "instance.h"
#include "utilities.h"
#include "timer.h"
#include "optimization.h"

#include <string.h>

/* Global configuration variables */
int use_cw_init = 0;      /* 0 = random, 1 = cw */
int use_best_pivot = 0;   /* 0 = first, 1 = best */
int neigh_type = 0;       /* 0 = transpose, 1 = exchange, 2 = insert */
int use_vnd = 0;          /* 0 = normal local search, 1 = VND */
int vnd_order_type = 0;   /* 0 = tei, 1 = tie */
char *FileName = NULL;

void readOpts(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        /* Initialization */
        if (strcmp(argv[i], "--cw") == 0) use_cw_init = 1;
        else if (strcmp(argv[i], "--random") == 0) use_cw_init = 0;

        /* Pivoting */
        else if (strcmp(argv[i], "--first") == 0) use_best_pivot = 0;
        else if (strcmp(argv[i], "--best") == 0) use_best_pivot = 1;

        /* Neighborhoods */
        else if (strcmp(argv[i], "--transpose") == 0) neigh_type = 0;
        else if (strcmp(argv[i], "--exchange") == 0) neigh_type = 1;
        else if (strcmp(argv[i], "--insert") == 0) neigh_type = 2;

        /* VND options */
        else if (strcmp(argv[i], "--vnd-tei") == 0) {
            use_vnd = 1;
            vnd_order_type = 0;
        }
        else if (strcmp(argv[i], "--vnd-tie") == 0) {
            use_vnd = 1;
            vnd_order_type = 1;
        }

        /* Instance file */
        else if (strcmp(argv[i], "-i") == 0) {
            if (i + 1 < argc) {
                FileName = strdup(argv[++i]);
            }
        }
    }

    if (FileName == NULL) {
        printf("Error: No instance file provided. Use -i <filename>\n");
        exit(1);
    }
}

int main(int argc, char **argv) {
    long int i, j;
    long int *currentSolution;

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    if (argc < 2) {
        printf("No instance file provided (use -i <instance_name>). Exiting.\n");
        exit(1);
    }

    readOpts(argc, argv);

    CostMat = readInstance(FileName);

    /* constant random seed per instance */
    Seed = 0;
    for (i = 0; i < PSize; ++i) {
        for (j = 0; j < PSize; ++j) {
            Seed += (long int)CostMat[i][j];
        }
    }

    start_timers();

    currentSolution = (long int *)malloc(PSize * sizeof(long int));
    if (currentSolution == NULL) {
        fprintf(stderr, "Memory allocation failed for currentSolution.\n");
        exit(1);
    }

    /* Initialization */
    if (use_cw_init == 1) {
        createCWSolution(currentSolution);
    } else {
        createRandomSolution(currentSolution);
    }

    if (use_vnd) {
        VND(currentSolution, vnd_order_type);
    } else {
        localSearch(currentSolution, use_best_pivot, neigh_type);
    }

    long long int final_cost = computeCost(currentSolution);
    double total_time = elapsed_time(VIRTUAL);

    printf("%lld %g", final_cost, total_time);

    free(currentSolution);
    free(FileName);

    return 0;
}