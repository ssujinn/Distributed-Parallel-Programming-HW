#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct IMG{
    char M;
    char N;
    int width;
    int height;
    int max;
    unsigned char *pixels;
}IMG;

int main(){
    FILE *fp;
    char filename[100];
    char line[100];
    IMG *img, *new;
    int mode;
    int w, h, size;
	time_t start, end;

	start = clock();

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

    switch (mode){
	case 1:
	    strcat(filename, "_flip_seq.ppm");
	    fp = fopen(filename, "w");

	    for (int i = 0; i < h; i++){
		for (int j = 0; j < w * 3; j += 3){
			new->pixels[i * (w * 3) + j] = img->pixels[i * (w * 3) + (w * 3) - j - 3];
			new->pixels[i * (w * 3) + j + 1] = img->pixels[i * (w * 3) + (w * 3) - j - 2];
			new->pixels[i * (w * 3) + j + 2] = img->pixels[i * (w * 3) + (w * 3) - j - 1];
		}
	    }
	    break;
	case 2:
	    strcat(filename, "_grayscale_seq.ppm");
	    fp = fopen(filename, "w");

	    for (int i = 0; i < h; i++){
		for (int j = 0; j < w * 3; j += 3){
		    int avg;
		    double tmp;
		    tmp = img->pixels[i * (w * 3) + j] + img->pixels[i * (w * 3) + j + 1] + img->pixels[i * (w * 3) + j + 2];
		    tmp /= (double)3;
		    avg = (int)tmp;
		    new->pixels[i * (w * 3) + j] = new->pixels[i * (w * 3) + j + 1] = new->pixels[i * (w * 3) + j + 2] = avg;
		}
	    }
	    break;
	case 3:
	    strcat(filename, "_smooth_seq.ppm");
	    fp = fopen(filename, "w");

	    for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
		    for (int c = 0; c < 3; c++){
		   	int avg;
		    	double tmp = 0;
			
			if (i != 0){
			    if (j != 0)
				tmp += img->pixels[(i - 1) * (w * 3) + (j - 1) * 3 + c];
			    tmp += img->pixels[(i - 1) * (w * 3) + j * 3 + c];
			    if (j != w - 1)
				tmp += img->pixels[(i - 1) * (w * 3) + (j + 1) * 3 + c];
			}
			if (j != 0)
			    tmp += img->pixels[i * (w * 3) + (j - 1) * 3 + c];
			tmp += img->pixels[i * (w * 3) + j * 3 + c];
			if (j != w - 1)
			    tmp += img->pixels[i * (w * 3) + (j + 1) * 3 + c];
			if (i != h - 1){
			    if (j != 0)
			    	tmp += img->pixels[(i + 1) * (w * 3) + (j - 1) * 3 + c];
			    tmp += img->pixels[(i + 1) * (w * 3) + j * 3 + c];
			    if (j != w - 1)
				tmp += img->pixels[(i + 1) * (w * 3) + (j + 1) * 3 + c];
			}

		    	tmp /= (double)9;
		    	avg = (int)tmp;
		    	new->pixels[i * (w * 3) + j * 3 + c] = avg;
		    }
		}
	    }
	    break;
	default:
	    printf("wrong option\n");
	    return 0;
    }

    fprintf(fp, "%c%c\n", 'P', '3');
    fprintf(fp, "%d %d\n", w, h);
    fprintf(fp, "%d", 255);

    for (int i = 0; i < size * 3; i++){
	if (i % 3 == 0)
	    fprintf(fp, "\n");
	fprintf(fp, "%d ", new->pixels[i]);
    }

    fclose(fp);
    free(new->pixels);
    free(img->pixels);
    free(new);
    free(img);

	end = clock();

	printf("exec time : %e\n", (double)end - start);

    return 0;
}
