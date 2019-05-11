#ifndef PIXEL_DATA_PROCESS  
#define PIXEL_DATA_PROCESS

#include <stdio.h>

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

//BITMAPFILEHEADER
struct bmp_header {
	long image_size;
	long blank;
	long start_postion;
};
typedef bmp_header bmp_header;

//BITMAPINFOHEADER
struct info_header {
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
};
typedef info_header info_header;

#endif // !PIXEL_DATA_PROCESS

