#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

typedef struct IMG{
    char M;
    char N;
    int width;
    int height;
    int max;
    unsigned char *pixels;
}IMG;

int main(int argc, char *argv[]){
    int numprocs, rank;
    int w, h, size, mode;
    IMG *img, *new;
    unsigned char *sendbuf, *recvbuf, *newbuf, *output;
    char filename[100];
    FILE *fp;
	double start, end;

    MPI_Init(&argc, &argv);

	start = MPI_Wtime();

    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0){
    char line[100];

    printf("filename? ");
    scanf("%s", filename);
    filename[strlen(filename)] = 0;

    fp = fopen(filename, "r");
    if (fp == NULL){
	printf("file open error!\n");
	return 0;
    }

    img = (IMG*)malloc(sizeof(IMG));

    fgets(line, sizeof(line), fp); 
    sscanf(line, "%c%c", &img->M, &img->N);
    fgets(line, sizeof(line), fp);
    if (line[0] == '#')
	fgets(line, sizeof(line), fp);
    sscanf(line, "%d %d", &img->width, &img->height);
    fgets(line, sizeof(line), fp);
    sscanf(line, "%d\n", &img->max);
 
    if (img->M != 'P' || img->N != '6' || img->max != 255){
	printf("file format error!\n");
	return 0;
    }

    w = img->width;
    h = img->height;
    size = w * h;
    img->pixels = (unsigned char*)malloc(sizeof(unsigned char) * size * 3);
    for (int i = 0; i < size * 3; i++){
	fread(&img->pixels[i], sizeof(unsigned char), 1, fp);
    }

    fclose(fp);

    printf("Select mode\n");
    printf("1. flip an image horizontally\n");
    printf("2. grayscale (average)\n");
    printf("3. smooth image\n");

    scanf("%d", &mode);
    filename[strlen(filename) - 4] = 0;

    new = (IMG*)malloc(sizeof(new));
    new->pixels = (unsigned char*)malloc(sizeof(unsigned char) * size * 3);
    new->M = img->M;
    new->N = img->N;
    new->width = img->width;
    new->height = img->height;
    new->max = img->max;
    }

    MPI_Datatype rpix;
    int tmph;

    MPI_Bcast(&h, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&w, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&mode, 1, MPI_INT, 0, MPI_COMM_WORLD);

    tmph = h / numprocs;
	if (tmph == 0)
		tmph = 1;
    MPI_Type_contiguous(w * 3 * tmph, MPI_UNSIGNED_CHAR, &rpix);
    MPI_Type_commit(&rpix);
    sendbuf = (unsigned char*)malloc(sizeof(unsigned char) * h * w * 3);
    recvbuf = (unsigned char*)malloc(sizeof(unsigned char) * h * w * 3);
    newbuf = (unsigned char*)malloc(sizeof(unsigned char) * h * w * 3);
    output = (unsigned char*)malloc(sizeof(unsigned char) * h * w * 3);

    if (rank == 0){
	for (int i = 0; i < size * 3; i++)
	    sendbuf[i] = img->pixels[i];
    }

    MPI_Scatter(sendbuf, 1, rpix, recvbuf, 1, rpix, 0, MPI_COMM_WORLD);

    switch (mode){
	case 1:
	    if (rank == 0){
	    strcat(filename, "_flip_par.ppm");
	    fp = fopen(filename, "wb");
	    }

	    for (int i = 0; i < tmph; i++){
		for (int j = 0; j < w * 3; j += 3){
			newbuf[i * (w * 3) + j] = recvbuf[i * (w * 3) + (w * 3) - j - 3];
			newbuf[i * (w * 3) + j + 1] = recvbuf[i * (w * 3) + (w * 3) - j - 2];
			newbuf[i * (w * 3) + j + 2] = recvbuf[i * (w * 3) + (w * 3) - j - 1];
		}
	    }
	    break;

	case 2:
	    if (rank == 0){
	    strcat(filename, "_grayscale_par.ppm");
	    fp = fopen(filename, "w");
	    }

	    for (int i = 0; i < tmph; i++){
		for (int j = 0; j < w * 3; j += 3){
		    int avg;
		    double tmp;
		    tmp = recvbuf[i * (w * 3) + j] + recvbuf[i * (w * 3) + j + 1] + recvbuf[i * (w * 3) + j + 2];
		    tmp /= (double)3;
		    avg = (int)tmp;
		    newbuf[i * (w * 3) + j] = newbuf[i * (w * 3) + j + 1] = newbuf[i * (w * 3) + j + 2] = avg;
		}
	    }
	    break;
	case 3:
	    if (rank == 0){
	    strcat(filename, "_smooth_par.ppm");
	    fp = fopen(filename, "w");
	    }

	    for (int i = 0; i < tmph; i++){
		for (int j = 0; j < w; j++){
		    for (int c = 0; c < 3; c++){
		   	int avg;
		    	double tmp = 0;

			for (int m = i - 1; m <= i + 1; m++){
			    for (int n = j - 1; n <= j + 1; n++){
				if (m < 0 || n < 0 || m > tmph - 1 || n > w - 1)
				    continue;
				tmp += recvbuf[m * (w * 3) + n * 3 + c];
			    }
			}

		    	tmp /= (double)9;
		    	avg = (int)tmp;
		    	newbuf[i * (w * 3) + j * 3 + c] = avg;
		    }
		}
	    }
	    break;
	default:
	    printf("wrong option\n");
	    return 0;
    }

    MPI_Gather(newbuf, 1, rpix, output, 1, rpix, 0, MPI_COMM_WORLD);

    if (rank == 0){
    fprintf(fp, "%c%c\n", 'P', '3');
    fprintf(fp, "%d %d\n", w, h);
    fprintf(fp, "%d\n", 255);
    
    for (int i = 0; i < size * 3; i++){
	fprintf(fp, "%d ", output[i]);
    }

    fclose(fp);
    free(new->pixels);
    free(img->pixels);
    free(new);
    free(img);
    }

    free(recvbuf);
    free(sendbuf);
    free(newbuf);
    free(output);

	end = MPI_Wtime();

	if (rank == 0)
		printf("exec time : %e\n", 1000* (end - start));
    MPI_Finalize();

    return 0;
}
