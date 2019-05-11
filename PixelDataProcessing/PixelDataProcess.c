#include "PixelDataProcess.h"
#include <string.h>

void splite_yuv420p(char *path, char *file_name, int w, int h, int num) {
	FILE *pf = fopen(strcat(path,file_name), "rb+");
	FILE *pf_y = fopen(strcat(path,"output_420p_y.y"),"wb+");
	FILE *pf_u = fopen(strcat(path, "output_420p_u.y"), "wb+");
	FILE *pf_v = fopen(strcat(path, "output_420p_v.y"), "wb+");

	unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2);

	for (int i = 0; i < num; i++) {
		fread(pic, 1, w * h * 3 / 2, pf);
		fwrite(pic, 1, w * h, pf_y);
		fwrite(pic + w * h, 1, w * h / 4, pf_u);
		fwrite(pic + w * h * 5 / 4, 1, w * h / 4, pf_v);
	}

	free(pic);
	fclose(pf);
	fclose(pf_y);
	fclose(pf_u);
	fclose(pf_v);
}

void splite_yuv444p(char *path, char *file_name, int w, int h, int num) {
	FILE *pf = fopen(strcat(path, file_name), "rb+");
	FILE *pf_y = fopen(strcat(path, "output_444p_y.y"), "wb+");
	FILE *pf_u = fopen(strcat(path, "output_444p_u.y"), "wb+");
	FILE *pf_v = fopen(strcat(path, "output_444p_v.y"), "wb+");

	unsigned char *pic = (unsigned char *)malloc(w * h * 3);

	for (int i = 0; i < num; i++) {
		fread(pic, 1, w * h * 3, pf);
		fwrite(pic, 1, w * h, pf_y);
		fwrite(pic + w * h, 1, w * h, pf_u);
		fwrite(pic + w * h * 2, 1, w * h, pf_v);
	}

	free(pic);
	fclose(pf);
	fclose(pf_y);
	fclose(pf_u);
	fclose(pf_v);
}


void yuv420p_gray(char *path, char *file_name, int w, int h, int num) {
	FILE *pf = fopen(strcat(path, file_name), "rb+");
	FILE *pf_gray = fopen(stract(path, "output_gray.yuv"), "+wb");

	unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2);

	for (int i = 0; i < num; i++) {
		fread(pic, 1, w * h * 3 / 2, pf);
		memset(pic + w * h, 128, w * h / 2);
		fwrite(pic, 1, w * h * 3 / 2, pf_gray);
	}

	free(pic);
	fclose(pf);
	fclose(pf_gray);
}

void yuv420p_half_y(char *path, char *file_name, int w, int h, int num) {
	FILE *pf = fopen(strcat(path, file_name), "rb+");
	FILE *pf_h = fopen(strcat(path, "output_half_y.yuv"), "wb+");

	unsigned char *pic = (unsigned char *)molloc(w * h * 3 / 2);

	for (int i = 0; i < num; i++) {
		fread(pic, 1, w * h * 3 / 2, pf);
		for (int j = 0; j < w*h; j++)
			pic[j] /= 2;
		fwrite(pic, 1, w * h * 3 / 2, pf_h);
	}

	free(pic);
	fclose(pf);
	fclose(pf_h);
}