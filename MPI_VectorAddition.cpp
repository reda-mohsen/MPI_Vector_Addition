/*
*	Author: Reda Mohsen
*	Topic: Vector Addition - MPI (Collective Communication)
*/

#include <mpi.h>
#include <iostream>
#include "stdio.h"

#define VECTOR_SIZE 32

int main(int argc, char** argv)
{
	int size, rank;
	int local_n;
	int* local_a, * local_b, * local_c;
	int* A = NULL, * B = NULL, * C = NULL;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	local_n = VECTOR_SIZE / size;

	local_a = (int*)malloc(local_n * sizeof(int));
	local_b = (int*)malloc(local_n * sizeof(int));
	local_c = (int*)malloc(local_n * sizeof(int));

	if (rank == 0)
	{
		A = (int*)malloc(VECTOR_SIZE * sizeof(int));
		B = (int*)malloc(VECTOR_SIZE * sizeof(int));
		C = (int*)malloc(VECTOR_SIZE * sizeof(int));

		// Initialize Vector A and Vector B
		for (int i = 0; i < VECTOR_SIZE; i++)
			A[i] = B[i] = i + 1;
		printf("The Vector A is [ ");
		// Print Vector A
		for (int i = 0; i < VECTOR_SIZE; i++)
			printf("%d ", A[i]);
		printf("] \n");
		// Print Vector B
		printf("The Vector B is [ ");
		for (int i = 0; i < VECTOR_SIZE; i++)
			printf("%d ", B[i]);
		printf("] \n\n");
	}

	MPI_Scatter(A, local_n, MPI_INT, local_a, local_n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(B, local_n, MPI_INT, local_b, local_n, MPI_INT, 0, MPI_COMM_WORLD);
	free(A);
	free(B);

	// Do vector addition in each portion
	for (int i = 0; i < local_n; i++)
		local_c[i] = local_a[i] + local_b[i];

	// Gather all the results in the rank 0 process
	MPI_Gather(local_c, local_n, MPI_INT, C, local_n, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("The Addition Vector Result is [ ");
		for (int i = 0; i < VECTOR_SIZE; i++)
			printf("%d ", C[i]);
		free(C);
		printf("] \n");
	}
	MPI_Finalize();
}



