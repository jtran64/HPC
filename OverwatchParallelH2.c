//			Christian and Johnny
//		Serial Matrix Multiplication
//		Program will multiply 2 square matricies
//		and return result to a 3rd matrix.
//		User must set the MATRIX_SIZE variable
//
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

// specifies the size of square matrix (n x n)
const int MATRIX_SIZE = 640;
const int NUM_PROCESSES = 16;

// purpose: prints the matrix to screen 
// requires: populated matrix 
void matrix_print(int *matrix)
{
	int i, z;
	FILE *outfile;
	outfile = fopen("./OverwatchParallelOutput.txt", "w");
	for (i = 0; i < MATRIX_SIZE; i++)
	{
		
		//fprintf(outfile, "\nrow %d: ", i);
		for (z = 0; z < MATRIX_SIZE; z++)
			fprintf(outfile, " %d ", matrix[i*MATRIX_SIZE+z]);
	}
	fprintf(outfile, "\n");
	fclose(outfile);
}

void matrix_init1D(int matrix[], int size, int value)
{
		int i;
		int sz = size*size;

		for(i = 0; i<size; i++)
			matrix[i] = value;
}

// MAIN //
int main (int argc, char *argv[])
{
	double time;
	int rank, size, partition, matSize;
	matSize = MATRIX_SIZE * MATRIX_SIZE;
	
	int matrix_A[matSize];
	int matA_local[matSize/NUM_PROCESSES];
	int matrix_B[matSize];
	int result[matSize/NUM_PROCESSES];
	int combined[matSize];

	//printf("STARTING!!!\n");
	
		matrix_init1D(matrix_B, MATRIX_SIZE * MATRIX_SIZE, 2);
		MPI_Init(&argc, &argv);
		time = MPI_Wtime();
		MPI_Comm_rank (MPI_COMM_WORLD, &rank);
		MPI_Comm_size (MPI_COMM_WORLD, &size);
		partition = (MATRIX_SIZE * MATRIX_SIZE)/size;
		
		matrix_init1D(result, partition, 0);
		matrix_init1D(matA_local, partition, 1);
		//MPI_Scatter(matrix_A, partition, MPI_INT, matA_local, partition, MPI_INT, 0, MPI_COMM_WORLD);

		int x, y, z;

		//START MATH HERE
		for(x = 0; x < MATRIX_SIZE / size; x++)
		{
			for(y = 0; y < MATRIX_SIZE; y++)
			{
				for(z = 0; z < MATRIX_SIZE; z++)
					result[x*MATRIX_SIZE + y] += matA_local[(x*MATRIX_SIZE) + z] * matrix_B[z * MATRIX_SIZE];
			}
		}
		
		MPI_Gather(result, partition, MPI_INT, combined, partition, MPI_INT, 0, MPI_COMM_WORLD);
		if(rank == 0)
			matrix_print(combined);
		
	//printf("\nDONE!");
	time = MPI_Wtime();
	MPI_Finalize();
	printf("%f\n", time);
	return 0;
}
