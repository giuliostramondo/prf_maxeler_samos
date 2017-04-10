#include<stdio.h>
#include "prf.h"

#define data_type int64_t

void printAGUOutput(int p, int q, Address2d* elements);

/*
 * Tests if a list of address is within the matrix bound
 * returns 1 if it is whitin bound, 0 otherwise
 */
int checkBoundaries(int p,int q, Address2d* elements);
//Returns the number of parallel access required, with a given shape, to access all the elements of the matrix
int totAccessToScanMatrix(int p, int q, acc_type shape);

data_type* fillPRF(int p, int q, scheme s, int **A_test, int *el_counter);
data_type* generate_expected_output(int p, int q, int **A_test, data_type* input_data, int size);
data_type* all_accesses(int p, int q, acc_type shape, int N, int M, int *el_counter);
