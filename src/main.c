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

// Global configuration variables
int use_cw_init = 0;     // 0 = random, 1 = cw
int use_best_pivot = 0;  // 0 = first, 1 = best
int neigh_type = 0;      // 0 = transpose, 1 = exchange, 2 = insert
char *FileName = NULL;

void readOpts(int argc, char **argv) {
    // Loop through all command line arguments starting from index 1
    for (int i = 1; i < argc; i++) {
        // Initialization
        if (strcmp(argv[i], "--cw") == 0) use_cw_init = 1;
        else if (strcmp(argv[i], "--random") == 0) use_cw_init = 0;
        
        // Pivoting
        else if (strcmp(argv[i], "--first") == 0) use_best_pivot = 0;
        else if (strcmp(argv[i], "--best") == 0) use_best_pivot = 1;
        
        // Neighborhoods
        else if (strcmp(argv[i], "--transpose") == 0) neigh_type = 0;
        else if (strcmp(argv[i], "--exchange") == 0) neigh_type = 1;
        else if (strcmp(argv[i], "--insert") == 0) neigh_type = 2;
        
        // Instance File
        else if (strcmp(argv[i], "-i") == 0) {
            // Make sure there is actually a file name after -i
            if (i + 1 < argc) {
                FileName = strdup(argv[++i]); // Grab the next argument and increment 'i'
            }
        }
    }

    // Safety check
    if (FileName == NULL) {
        printf("Error: No instance file provided. Use -i <filename>\n");
        exit(1);
    }
}



int main (int argc, char **argv) 
{
  long int i,j;
  long int *currentSolution;

  /* Do not buffer output */
  setbuf(stdout,NULL);
  setbuf(stderr,NULL);
  
  if (argc < 2) {
    printf("No instance file provided (use -i <instance_name>). Exiting.\n");
    exit(1);
  }
  
  /* Read parameters */
  readOpts(argc, argv);

  /* Read instance file */
  CostMat = readInstance(FileName);
  // printf("Data have been read from instance file. Size of instance = %ld.\n\n", PSize);

  /* initialize random number generator, deterministically based on instance.
   * To do this we simply set the seed to the sum of elements in the matrix, so it is constant per-instance,
   but (most likely) varies between instances */
  Seed = (long int) 0;
    for (i=0; i < PSize; ++i)
      for (j=0; j < PSize; ++j)
        Seed += (long int) CostMat[i][j];
  // printf("Seed used to initialize RNG: %ld.\n\n", Seed);
  
  /* starts time measurement */
  start_timers();currentSolution = (long int *)malloc(PSize * sizeof(long int));

  // 1. INITIALIZATION
  if (use_cw_init == 1) {
      createCWSolution(currentSolution);
      // printf("Initialization: CW Heuristic\n");
  } else {
      createRandomSolution(currentSolution);
      // printf("Initialization: Random\n");
  }

  // 2. RUN LOCAL SEARCH
  // printf("Starting Local Search...\n");
  localSearch(currentSolution, use_best_pivot, neigh_type);

  // 3. PRINT FINAL RESULTS (Clean format for the script)
  long long int final_cost = computeCost(currentSolution);
  double total_time = elapsed_time(VIRTUAL);
  
  // This output is captured by the result variable in the bash script
  printf("%lld %g", final_cost, total_time);

  free(currentSolution);
  return 0;
}