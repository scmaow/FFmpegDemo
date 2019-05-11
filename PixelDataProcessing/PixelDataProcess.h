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

#endif // !PIXEL_DATA_PROCESS

