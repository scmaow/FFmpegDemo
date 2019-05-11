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
	FILE *pf_gray = fopen(strcat(path, "output_gray.yuv"), "wb+");

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

	unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2);

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

void yuv420p_border(char *path, char *file_name, int w, int h, int border, int num) {
	FILE *pf = fopen(strcat(path, file_name), "rb+");
	FILE *pf_b = fopen(strcat(path, "output_border.yuv"), "wb+");

	unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2);

	for (int i = 0; i < num; i++) {
		fread(pic, 1, w * h * 3 / 2, pf);
		for (int j = 0; j < h; j++) {
			for (int k = 0; k < w; k++) {
				if (k<border || k>(w - border) || j<border || j>(h - border)) {
					pic[j * w + k] = 255;
				}
			}
		}
		fwrite(pic, 1, w * h * 3 / 2, pf);
	}

	free(pic);
	fclose(pf);
	fclose(pf_b);
}


void yuv420p_graybar(char *path, int w, int h, int ymin, int ymax, int bar_num) {
	int bar_w;
	float lum_inc;
	unsigned char lum_tmp;
	int uv_w;
	int uv_h;
	FILE *pf = NULL;
	unsigned char *data_y = NULL;
	unsigned char *data_u = NULL;
	unsigned char *data_v = NULL;

	bar_w = w / bar_num;  //每个灰度条的宽度
	lum_inc = ((float)(ymax - ymin)) / ((float)(bar_num - 1)); //每个灰度条的亮度
	uv_w = w / 2;
	uv_h = h / 2;

	data_y = (unsigned char *)malloc(w * h);
	data_u = (unsigned char *)malloc(uv_w * uv_h);
	data_v = (unsigned char *)malloc(uv_w * uv_h);

	if ((pf = open(strcat(path, "output_yuv_bar.yuv"), "wb+")) == NULL) {
		printf("Cannot create file.\n");
		return;
	}

	printf("Y,U,V value from picture's left to right:\n");
	for (int t = 0; t < bar_num; t++) {
		lum_tmp = ymin + (char)(t * lum_inc);
		printf("%3d,128,128\n", lum_tmp);
	}

	//计算亮度Y
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {
			int t = i / bar_w;
			lum_tmp = ymin + (char)(t * lum_inc);
			data_y[j * w + i] = lum_tmp;
		}
	}

	for (int j = 0; j < uv_h; j++) {
		for (int i = 0; i < uv_w; i++) {
			data_u[j * uv_w + i] = 128;
		}
	}

	for (int j = 0; j < uv_h; j++) {
		for (int i = 0; i < uv_w; i++) {
			data_v[j * uv_w + i] = 128;
		}
	}

	fwrite(data_y, w * h, 1, pf);
	fwrite(data_u, uv_w * uv_h, 1, pf);
	fwrite(data_v, uv_w * uv_h, 1, pf);

	fclose(pf);
	free(data_y);
	free(data_u);
	free(data_v);
}

void yuv420p_psnr(char *path, char *file_name1, char *file_name2, int w, int h, int num) {
	FILE *pf1 = fopen(strcat(path, file_name1), "rb+");
	FILE *pf2 = fopen(strcat(path, file_name2), "rb+");

	unsigned char *pic1 = (unsigned char *)malloc(w * h);
	unsigned char *pic2 = (unsigned char *)malloc(w * h);

	for (int i = 0; i < num; i++) {
		fread(pic1, 1, w * h, pf1);
		fread(pic2, 1, w * h, pf2);

		double mse_sum = 0;
		double mse = 0;
		double psnr = 0;

		for (int j = 0; j < w * h; j++) {
			mse_sum += pow((double)(pic1[j] - pic2[j]), 2);
		}

		mse = mse_sum / (w * h);
		psnr = 10 * log10(255.0 * 255.0 / mse);
		printf("%5.3f\n", psnr);
		fseek(pf1, w * h / 2, SEEK_CUR);
		fseek(pf2, w * h / 2, SEEK_CUR);
	}

	free(pic1);
	free(pic2);
	fclose(pf1);
	fclose(pf2);
}

void splite_rgb24(char *path, char *file_name, int w, int h, int num) {
	FILE *pf = fopen(strcat(path, file_name), "rb+");
	FILE *pf_r = fopen(strcat(path, "output_r.y"), "wb+");
	FILE *pf_g = fopen(strcat(path, "output_g.y"), "wb+");
	FILE *pf_b = fopen(strcat(path, "output_b.y"), "wb+");

	unsigned char *pic = (unsigned char *)malloc(w * h * 3);

	for (int i = 0; i < num; i++) {
		fread(pic, 1, w * h * 3, pf);
		for (int j = 0; j < w * h * 3; j = j + 3) {
			fwrite(pic + j, 1, 1, pf_r);
			fwrite(pic + j + 1, 1, 1, pf_g);
			fwrite(pic + j + 2, 1, 1, pf_b);
		}
	}

	free(pic);
	fclose(pf);
	fclose(pf_r);
	fclose(pf_g);
	fclose(pf_b);
}

void rgb24_to_bmp(char *path, char *rgb24_file_name, int w, int h, char *bmp_file_name) {
	bmp_header bmp_head = { 0 };
	info_header info_head = { 0 };
	char bf_type[2] = { 'B','M' };
	int header_size = sizeof(bf_type) + sizeof(bmp_header) + sizeof(info_header);
	unsigned char *rgb24_buffer = NULL;
	FILE *pf_rgb24 = NULL;
	FILE *pf_bmp = NULL;

	if ((pf_rgb24 = open(strcat(path, rgb24_file_name), "rb+")) == NULL) {
		printf("Cannot open input RGB24 file.\n");
		return;
	}

	if ((pf_bmp = open(strcat(path, bmp_file_name), "wb+")) == NULL) {
		printf("Cannot open input BMP file.\n");
		return;
	}

	rgb24_buffer = (unsigned char *)malloc(w * h * 3);
	fread(rgb24_buffer, 1, w * h * 3, pf_rgb24);

	bmp_head.image_size = 3 * w * h + header_size;
	bmp_head.start_postion = header_size;

	info_head.length = sizeof(info_header);
	info_head.width = w;
	info_head.height = h;
	info_head.color_plane = 1;
	info_head.bit_color = 24;
	info_head.real_size = 3 * w * h;

	fwrite(bf_type, 1, sizeof(bf_type), pf_bmp);
	fwrite(&bmp_head, 1, sizeof(bmp_head), pf_bmp);
	fwrite(&info_head, 1, sizeof(info_head), pf_bmp);

	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {
			char tmp = rgb24_buffer[(j * w + i) * 3 + 2];
			rgb24_buffer[(j * w + i) * 3 + 2] = rgb24_buffer[(j * w + i) * 3 + 0];
			rgb24_buffer[(j * w + i) * 3 + 0] = tmp;
		}
	}

	fwrite(rgb24_buffer, 3 * w * h, 1, pf_bmp);

	fclose(pf_rgb24);
	fclose(pf_bmp);
	free(rgb24_buffer);
}

unsigned char clip_value(unsigned char x, unsigned char min_val, unsigned char max_val) {
	if (x > max_val) {
		return max_val;
	}
	else if (x < min_val) {
		return min_val;
	}
	else {
		return x;
	}
}


bool rgb24_to_yuv420p(unsigned char *rgb_buf, int w, int h, unsigned char *yuv_buf) {


	return true;
}

void rgb24_convert_yuv420p(char *path, char *file_name, int w, int h, int num) {

}