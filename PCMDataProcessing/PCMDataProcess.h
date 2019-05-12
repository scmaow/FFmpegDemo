/**
 * 音频采样数据处理
 *
 * 05/13/2019
*/
#pragma once
#ifndef PCM_DATA_PROCESS
#define PCM_DATA_PROCESS

#include <cstdio>

/**
 * 将PCM16LE双声道数据中左声道和右声道的数据分离成两个文件
 *
*/
void split_pcm16le(char *path, char *file_name);


/**
 * 将PCM16LE双声道数据中左声道的音量降低一半
 *
*/
void pcm16le_half_volume_left(char *path, char *file_name);

#endif // !PCM_DATA_PROCESS
