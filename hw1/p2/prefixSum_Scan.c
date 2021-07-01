#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[]){
    int numprocs, rank;
    int sendbuf, recvbuf;
    int *num;
    int tag = 1;
	MPI_Status ss1, ss2;
    double start, end;
    double tottime, maxtime;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    num = (int*)malloc(sizeof(int)*numprocs);
    start = MPI_Wtime();

	// random number (in rank 0)
    if (rank == 0){
	srand(time(NULL));
	printf("<Before>\n");
	for (int i = 0; i < numprocs; i++){
	    num[i] = rand() % 100;
		printf("num %d> %d\n", i, num[i]);

	    // send data
		if (i != 0)
	    MPI_Send(&num[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		else
			sendbuf = num[0];
	}
    }
    // receive data
    else {
	MPI_Recv(&sendbuf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &ss1);
    }

	// partial sum
    MPI_Scan(&sendbuf, &recvbuf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	// gather data
	if (rank != 0){
		MPI_Send(&recvbuf, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
	}
	else {
		printf("<After>\n");
		for (int i = 0; i < numprocs; i++){
			if (i != 0)
			MPI_Recv(&num[i], 1, MPI_INT, i, 2, MPI_COMM_WORLD, &ss2);
			printf("num %d> %d\n", i, num[i]);
		}
	}

    end = MPI_Wtime();
    tottime = 1000*(end-start);


    //printf("Proc %d > Elapsed time = %e ms\n", rank, tottime);

	// Max time
    MPI_Reduce(&tottime, &maxtime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0){
	printf("Max time : %e ms\n", maxtime); 
    }
    MPI_Finalize();
    free(num);

    return 0;
}
