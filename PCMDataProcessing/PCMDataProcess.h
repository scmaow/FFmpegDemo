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

#endif // !PCM_DATA_PROCESS
