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

/**
 * 通过采样的方式将PCM16LE双声道数据的速度提高一倍
 *
*/
void  simplest_pcm16le_double_speed(char *path, char *file_name);

/**
 * PCM16LE双声道音频采样数据转换为PCM8音频采样数据
 *
*/
void  simplest_pcm16le_to_pcm8(char *path, char *file_name);

/**
 * 从PCM16LE单声道音频采样数据中截取一部分数据
 *
*/
void simplest_pcm16le_cut_single_channel(char *path, char *file_name, int start_num, int dur_num);


struct wave_header {
	char fcc_id[4];
	char fcc_type[4];
	unsigned long dw_size;
};
typedef struct wave_header wave_header;

struct wave_format {
	char  fcc_id[4];
	unsigned long  dw_size;
	unsigned short wformat_tag;
	unsigned short wchannels;
	unsigned long  dw_samples_per_sec;
	unsigned long  dw_avg_bytes_per_sec;
	unsigned short wblock_align;
	unsigned short ui_bits_per_sample;
};
typedef struct wave_format wave_format;

struct wave_data {
	char fcc_id[4];
	unsigned long dw_size;
};
typedef struct wave_data wave_data;

/**
 * PCM16LE双声道音频采样数据转换为WAVE格式音频数据
 *
*/
int simplest_pcm16le_to_wave(char *path, char *pcm_file_name, char *wave_file_name, int channels, int sample_rate);


#endif // !PCM_DATA_PROCESS
