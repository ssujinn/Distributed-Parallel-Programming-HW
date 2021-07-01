#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]){
	int numprocs;
	int *num;
	clock_t start, end;

	if (argv[1]){
		numprocs = atoi(argv[1]);
	}
	else {
		printf("enter the numprocs with execfile\n");
		return 0;
	}

	num = (int*)malloc(sizeof(int) * numprocs);

	start = clock();

	// random number
	srand(time(NULL));
	printf("<Before>\n");
	for (int i = 0; i < numprocs; i++){
	    num[i] = rand() % 100;
		printf("num %d> %d\n", i, num[i]);
	}

	// partial sum
	for (int i = 1; i < numprocs; i++)
		num[i] += num[i - 1];
	

	// print result
	printf("<After>\n");
	for (int i = 0; i < numprocs; i++)
		printf("num %d> %d\n", i, num[i]);

	end = clock();

	printf("total time : %e ms\n", (double)end - start);

	return 0;
}
