/**
 * 像素数据处理
 *
 * 05/11/2019
*/
#ifndef PIXEL_DATA_PROCESS_H
#define PIXEL_DATA_PROCESS_H

#include <cstdio>
#include <cstdbool>

/**
 * 
 *
*/
void splite_yuv420p(char *path, char *file_name, int w, int h, int num);


/**
 *
 *
*/
void splite_yuv444p(char *path, char *file_name, int w, int h, int num);


/**
 *
 *
*/
void yuv420p_gray(char *path, char *file_name, int w, int h, int num);

/**
 *
 *
*/
void yuv420p_half_y(char *path, char *file_name, int w, int h, int num);

/**
 *
 *
*/
void yuv420p_border(char *path, char *file_name, int w, int h, int border, int num);

/**
 *
 *
*/
void yuv420p_graybar(char *path, int w, int h, int ymin, int ymax, int bar_num);

/**
 *
 *
*/
void yuv420p_psnr(char *path, char *file_name1, char *file_name2, int w, int h,int num);

//BITMAPFILEHEADER
typedef struct {
	long image_size;
	long blank;
	long start_postion;
} bmp_header;

//BITMAPINFOHEADER
typedef struct {
	long length;
	long width;
	long height;
	unsigned short color_plane;
	unsigned short bit_color;
	long zip_format;
	long real_size;
	long x_pels;
	long y_pels;
	long color_use;
	long color_important;
} info_header;


/**
 *
 *
*/
void splite_rgb24(char *path, char *file_name, int w, int h, int num);

/**
 *
 *
*/
void rgb24_to_bmp(char *path, char *rgb24_file_name, int w, int h, char *bmp_file_name);

unsigned char clip_value(unsigned char x, unsigned char min_val, unsigned char max_val);

bool rgb24_to_yuv420p(unsigned char *rgb_buf, int w, int h, unsigned char *yuv_buf);

/**
 *
 *
*/
void rgb24_convert_yuv420p(char *path, char *file_name, int w, int h, int num);

#endif // !PIXEL_DATA_PROCESS_H

