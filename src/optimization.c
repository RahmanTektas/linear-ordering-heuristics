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
#include <limits.h>

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

void createCWSolution(long int *s) {
    int *used = (int *)calloc(PSize, sizeof(int));
    if (used == NULL) {
        fprintf(stderr, "Memory allocation failed in createCWSolution.\n");
        exit(EXIT_FAILURE);
    }

    for (int pos = 0; pos < PSize; pos++) {
        long long int best_score = LLONG_MIN;
        int best_elem = -1;

        for (int k = 0; k < PSize; k++) {
            if (used[k]) continue;

            long long int score = 0;

            for (int j = 0; j < PSize; j++) {
                if (!used[j] && j != k) {
                    score += CostMat[k][j];
                }
            }

            if (score > best_score) {
                best_score = score;
                best_elem = k;
            }
        }

        s[pos] = best_elem;
        used[best_elem] = 1;
    }

    free(used);
}

/*  Incremental delta evaluation */

/*  Transpose: swap adjacent positions i and i+1 */
long long int getDeltaTranspose(long int *s, int i) {
    return CostMat[s[i + 1]][s[i]] - CostMat[s[i]][s[i + 1]];
}

/*  Exchange: swap positions i and j */
long long int getDeltaExchange(long int *s, int i, int j) {
    if (i == j) return 0;

    if (i > j) {
        int tmp = i;
        i = j;
        j = tmp;
    }

    long long int delta = 0;

    delta += CostMat[s[j]][s[i]] - CostMat[s[i]][s[j]];

    for (int k = i + 1; k < j; k++) {
        delta += (CostMat[s[k]][s[i]] - CostMat[s[i]][s[k]]);
        delta += (CostMat[s[j]][s[k]] - CostMat[s[k]][s[j]]);
    }

    return delta;
}

/*  Insert: move element at i to position j */
long long int getDeltaInsert(long int *s, int i, int j) {
    if (i == j) return 0;

    long long int delta = 0;

    if (i < j) {
        for (int k = i + 1; k <= j; k++) {
            delta += CostMat[s[k]][s[i]] - CostMat[s[i]][s[k]];
        }
    } else {
        for (int k = j; k < i; k++) {
            delta += CostMat[s[i]][s[k]] - CostMat[s[k]][s[i]];
        }
    }

    return delta;
}

/*
    Move application
    type: 0=transpose, 1=exchange, 2=insert */
void applyMove(long int *s, int i, int j, int type) {
    if (type == 0) {
        long int tmp = s[i];
        s[i] = s[i + 1];
        s[i + 1] = tmp;
    } else if (type == 1) {
        long int tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    } else {
        long int val = s[i];

        if (i < j) {
            for (int k = i; k < j; k++) {
                s[k] = s[k + 1];
            }
        } else {
            for (int k = i; k > j; k--) {
                s[k] = s[k - 1];
            }
        }

        s[j] = val;
    }
}

/*  Local search
    pivot_rule: 0 = first-improvement, 1 = best-improvement
    neighborhood: 0 = transpose, 1 = exchange, 2 = insert
*/
void localSearch(long int *s, int pivot_rule, int neighborhood) {
    int improved = 1;

    while (improved) {
        improved = 0;
        long long int best_delta = 0;
        int best_i = -1;
        int best_j = -1;

        if (neighborhood == 0) {
            for (int i = 0; i < PSize - 1; i++) {
                long long int delta = getDeltaTranspose(s, i);

                if (delta > 0) {
                    if (pivot_rule == 0) {
                        applyMove(s, i, i + 1, 0);
                        improved = 1;
                        goto next_iteration;
                    } else if (delta > best_delta) {
                        best_delta = delta;
                        best_i = i;
                        best_j = i + 1;
                    }
                }
            }
        } else if (neighborhood == 1) {
            for (int i = 0; i < PSize - 1; i++) {
                for (int j = i + 1; j < PSize; j++) {
                    long long int delta = getDeltaExchange(s, i, j);

                    if (delta > 0) {
                        if (pivot_rule == 0) {
                            applyMove(s, i, j, 1);
                            improved = 1;
                            goto next_iteration;
                        } else if (delta > best_delta) {
                            best_delta = delta;
                            best_i = i;
                            best_j = j;
                        }
                    }
                }
            }
        } else {
            for (int i = 0; i < PSize; i++) {
                for (int j = 0; j < PSize; j++) {
                    if (i == j) continue;

                    long long int delta = getDeltaInsert(s, i, j);

                    if (delta > 0) {
                        if (pivot_rule == 0) {
                            applyMove(s, i, j, 2);
                            improved = 1;
                            goto next_iteration;
                        } else if (delta > best_delta) {
                            best_delta = delta;
                            best_i = i;
                            best_j = j;
                        }
                    }
                }
            }
        }

        if (pivot_rule == 1 && best_delta > 0) {
            applyMove(s, best_i, best_j, neighborhood);
            improved = 1;
        }

next_iteration:
        ;
    }
}

/*  VND helper function, one first-improvement step in one neighborhood
*/
static int firstImprovementStep(long int *s, int neighborhood) {
    if (neighborhood == 0) {
        for (int i = 0; i < PSize - 1; i++) {
            long long int delta = getDeltaTranspose(s, i);
            if (delta > 0) {
                applyMove(s, i, i + 1, 0);
                return 1;
            }
        }
    } else if (neighborhood == 1) {
        for (int i = 0; i < PSize - 1; i++) {
            for (int j = i + 1; j < PSize; j++) {
                long long int delta = getDeltaExchange(s, i, j);
                if (delta > 0) {
                    applyMove(s, i, j, 1);
                    return 1;
                }
            }
        }
    } else if (neighborhood == 2) {
        for (int i = 0; i < PSize; i++) {
            for (int j = 0; j < PSize; j++) {
                if (i == j) continue;
                long long int delta = getDeltaInsert(s, i, j);
                if (delta > 0) {
                    applyMove(s, i, j, 2);
                    return 1;
                }
            }
        }
    }

    return 0;
}

/*  VND
    order_type:
    0 -> transpose, exchange, insert
    1 -> transpose, insert, exchange
*/
void VND(long int *s, int order_type) {
    int order[3];

    if (order_type == 0) {
        order[0] = 0;  /* transpose */
        order[1] = 1;  /* exchange */
        order[2] = 2;  /* insert */
    } else {
        order[0] = 0;  /* transpose */
        order[1] = 2;  /* insert */
        order[2] = 1;  /* exchange */
    }

    int i = 0;

    while (i < 3) {
        int improved = firstImprovementStep(s, order[i]);

        if (improved) {
            i = 0;   /* restart from first neighborhood */
        } else {
            i++;
        }
    }
}