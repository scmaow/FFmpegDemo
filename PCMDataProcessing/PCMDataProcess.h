/**
 * ��Ƶ�������ݴ���
 *
 * 05/13/2019
*/
#pragma once
#ifndef PCM_DATA_PROCESS
#define PCM_DATA_PROCESS

#include <cstdio>

/**
 * ��PCM16LE˫�����������������������������ݷ���������ļ�
 *
*/
void split_pcm16le(char *path, char *file_name);


/**
 * ��PCM16LE˫��������������������������һ��
 *
*/
void pcm16le_half_volume_left(char *path, char *file_name);

/**
 * ͨ�������ķ�ʽ��PCM16LE˫�������ݵ��ٶ����һ��
 *
*/
void  simplest_pcm16le_double_speed(char *path, char *file_name);

/**
 * PCM16LE˫������Ƶ��������ת��ΪPCM8��Ƶ��������
 *
*/
void  simplest_pcm16le_to_pcm8(char *path, char *file_name);

/**
 * ��PCM16LE��������Ƶ���������н�ȡһ��������
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
 * PCM16LE˫������Ƶ��������ת��ΪWAVE��ʽ��Ƶ����
 *
*/
int simplest_pcm16le_to_wave(char *path, char *pcm_file_name, char *wave_file_name, int channels, int sample_rate);


#endif // !PCM_DATA_PROCESS
