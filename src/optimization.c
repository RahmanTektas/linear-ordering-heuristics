/*  Heuristic Optimization assignment, 2015.
    Adapted by Jérémie Dubois-Lacoste from the ILSLOP implementation
    of Tommaso Schiavinotto:
    ---
    ILSLOP Iterated Lcaol Search Algorithm for Linear Ordering Problem
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
#include <values.h>

#include "optimization.h" 
#include "instance.h"
#include "utilities.h"

#ifdef __MINGW32__
#include <float.h>
#define MAX_FLOAT FLT_MAX
#else
#define MAX_FLOAT MAXFLOAT
#endif


long int **CostMat;


long long int computeCost (long int *s ) {
    int h,k;
    long long int sum;
    
    /* Diagonal value are not considered */
    for (sum = 0, h = 0; h < PSize; h++ ) 
	for ( k = h + 1; k < PSize; k++ )
	    sum += CostMat[s[h]][s[k]];
    return(sum);
}


void createRandomSolution(long int *s) {
    int j; 
    long int *random;

    random = generate_random_vector(PSize);
    for ( j = 0 ; j < PSize ; j++ ) {
      s[j] = random[j];
    }
    free ( random );
}

/* --- SPEED-UP SECTION (Mandatory for full marks) --- */

long long int getDeltaTranspose(long int *s, int i) {
    // Page 20 of slides: Simple O(1) swap logic
    return CostMat[s[i+1]][s[i]] - CostMat[s[i]][s[i+1]];
}

long long int getDeltaExchange(long int *s, int i, int j) {
    if (i == j) return 0;
    if (i > j) { int temp = i; i = j; j = temp; }
    long long int delta = CostMat[s[j]][s[i]] - CostMat[s[i]][s[j]];
    for (int k = i + 1; k < j; k++) {
        delta += (CostMat[s[k]][s[i]] - CostMat[s[i]][s[k]]) + 
                 (CostMat[s[j]][s[k]] - CostMat[s[k]][s[j]]);
    }
    return delta;
}

long long int getDeltaInsert(long int *s, int i, int j) {
    if (i == j) return 0;
    long long int delta = 0;
    if (i < j) {
        for (int k = i + 1; k <= j; k++)
            delta += CostMat[s[k]][s[i]] - CostMat[s[i]][s[k]];
    } else {
        for (int k = j; k < i; k++)
            delta += CostMat[s[i]][s[k]] - CostMat[s[k]][s[i]];
    }
    return delta;
}

/* --- MOVE APPLICATION SECTION --- */

void applyMove(long int *s, int i, int j, int type) {
    if (type == 0) { // Transpose
        long int tmp = s[i]; s[i] = s[i+1]; s[i+1] = tmp;
    } else if (type == 1) { // Exchange
        long int tmp = s[i]; s[i] = s[j]; s[j] = tmp;
    } else if (type == 2) { // Insert
        long int val = s[i];
        if (i < j) for (int k = i; k < j; k++) s[k] = s[k+1];
        else for (int k = i; k > j; k--) s[k] = s[k-1];
        s[j] = val;
    }
}

/* --- MASTER SEARCH LOOP --- */

void localSearch(long int *s, int pivot_rule, int neighborhood) {
    int improvement = 1;
    while (improvement) {
        improvement = 0;
        long long int best_delta = 0;
        int best_i = -1, best_j = -1;

        for (int i = 0; i < PSize; i++) {
            // Adjust j loop based on neighborhood type
            int j_start = (neighborhood == 0) ? i + 1 : 0;
            int j_end = (neighborhood == 0) ? i + 2 : PSize;
            if (j_end > PSize) j_end = PSize;

            for (int j = j_start; j < j_end; j++) {
                if (i == j) continue;
                if (neighborhood == 1 && i >= j) continue; // Avoid double counting exchanges

                long long int delta = 0;
                if (neighborhood == 0) delta = getDeltaTranspose(s, i);
                else if (neighborhood == 1) delta = getDeltaExchange(s, i, j);
                else delta = getDeltaInsert(s, i, j);

                if (delta > 0) {
                    if (pivot_rule == 0) { // FIRST IMPROVEMENT
                        applyMove(s, i, j, neighborhood);
                        improvement = 1;
                        goto restart_search; // Immediately restart scan as per slides
                    } else if (delta > best_delta) { // BEST IMPROVEMENT
                        best_delta = delta;
                        best_i = i; best_j = j;
                    }
                }
            }
        }
        if (best_delta > 0) {
            applyMove(s, best_i, best_j, neighborhood);
            improvement = 1;
        }
        restart_search:;
    }
}

void createCWSolution(long int *s) {
    // Array to remember which elements we've already put in our solution
    int *already_placed = (int *)calloc(PSize, sizeof(int));
    
    // Loop to place one element at a time, from position 0 to PSize-1
    for (int pos = 0; pos < PSize; pos++) {
        long long int best_score = -999999999999; // Start with a super low score
        int best_element = -1;

        // Check every single element to see if it's the best one to place next
        for (int k = 0; k < PSize; k++) {
            if (already_placed[k] == 0) { // If we haven't placed it yet
                long long int score = 0;
                
                // Compare 'k' against everything we have already placed
                for (int i = 0; i < pos; i++) {
                    long int placed_elem = s[i];
                    // Score = (Edges from placed to k) MINUS (Edges from k to placed)
                    score += CostMat[placed_elem][k] - CostMat[k][placed_elem];
                }
                
                // If this is the best score we've seen, remember it
                if (score > best_score || best_element == -1) {
                    best_score = score;
                    best_element = k;
                }
            }
        }
        
        // Put the winning element into our solution array
        s[pos] = best_element;
        already_placed[best_element] = 1;
    }
    
    free(already_placed);
}