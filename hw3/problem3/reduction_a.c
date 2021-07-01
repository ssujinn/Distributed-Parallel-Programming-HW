#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ELEMENT 100000

int main() {
	int* arr;
	int max = 0;
	clock_t start, end;

  // random number creation
	srand(time(NULL));
	arr = (int*)malloc(sizeof(int) * NUM_ELEMENT);
	for (int i = 0; i < NUM_ELEMENT; i++)
		arr[i] = rand() % (NUM_ELEMENT * 10);

  // finding max num
	start = clock();
	for (int i = 0; i < NUM_ELEMENT; i++) {
		if (max < arr[i])
			max = arr[i];
	}
	end = clock();

	printf("MAX NUM : %d\n", max);
	printf("EXEC TIME : %f ms\n", (float)(end - start));
	
	return 0;
}