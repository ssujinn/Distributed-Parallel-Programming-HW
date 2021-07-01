#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[]){
    int numprocs, rank;
    int sendbuf = 0, recvbuf = 0;
    int *num;
    int dst, src, tag;
    MPI_Status ss1, ss2, st1, st2;
    MPI_Request r1, r2, rq1, rq2, r3, r4;

    double start, end;
    double tottime, maxtime;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    num = (int*)malloc(sizeof(int)*numprocs);

    start = MPI_Wtime();

    // random data (in rank 0)
    if (rank == 0){
	srand(time(NULL));
	printf("<Before>\n");
	for (int i = 0; i < numprocs; i++){
	    num[i] = rand() % 100;
		printf("num %d> %d\n", i, num[i]);
	    // send data
		if (i != 0)
			MPI_Isend(&num[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &r1);
		else
			sendbuf = num[0];
	}
    }
    // receive data
    else {
		MPI_Irecv(&sendbuf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &r2);
		MPI_Wait(&r2, &ss1);
    }

	// partial sum
    tag = 1;
    src = rank - 1;
    dst = rank + 1;
	
	if (rank == 0)
		MPI_Isend(&sendbuf, 1, MPI_INT, dst, tag, MPI_COMM_WORLD, &rq2);

    if (rank != 0){
		MPI_Irecv(&recvbuf, 1, MPI_INT, src, tag, MPI_COMM_WORLD, &rq1);
		MPI_Wait(&rq1, &st1);
	 }
	sendbuf += recvbuf;
    if (rank != numprocs - 1 && rank != 0){
    	MPI_Isend(&sendbuf, 1, MPI_INT, dst, tag, MPI_COMM_WORLD, &rq2);
    }

	// gather data
	if (rank != 0){
		MPI_Isend(&sendbuf, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &r3);
	}
	else {
		printf("<After>\n");
		for (int i = 0; i < numprocs; i++){
			if (i != 0){
			MPI_Irecv(&num[i], 1, MPI_INT, i, 2, MPI_COMM_WORLD, &r4);
			MPI_Wait(&r4, &ss2);
			}
			printf("num %d> %d\n", i, num[i]);
		}
	}


    end = MPI_Wtime();
    tottime = 1000 * (end - start);
    //printf("rank %d : %d -> %d\n", rank, before, after);
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
