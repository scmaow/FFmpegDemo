#include "PCMDataProcess.h"
#include <cstdlib>
#include <cstring>


void split_pcm16le(char *path, char *file_name) {
	FILE *pf = fopen(strcat(path, file_name), "rb+");
	FILE *pf_l = fopen(strcat(path, "output_left.pcm"), "wb+");
	FILE *pf_r = fopen(strcat(path, "output_right.pcm"), "wb+");

	unsigned char *sample = (unsigned char *)malloc(4);

	while (!feof(pf)) {
		fread(sample, 1, 4, pf);
		fwrite(sample, 1, 2, pf_l);
		fwrite(sample + 2, 1, 2, pf_r);
	}
	free(sample);
	fclose(pf);
	fclose(pf_l);
	fclose(pf_r);
}

void pcm16le_half_volume_left(char *path, char *file_name) {
	FILE *pf = fopen(strcat(path, file_name), "rb+");
	FILE *pf_half = fopen(strcat(path, "output_halfleft.pcm"), "wb+");

	int cnt = 0;

	unsigned char *sample = (unsigned char *)malloc(4);

	while (!feof(pf)) {
		short *sample_num = NULL;
		fread(sample, 1, 4, pf);
		sample_num = (short *)sample;
		*sample_num = *sample_num / 2;
		fwrite(sample, 1, 2, pf_half);
		fwrite(sample + 2, 1, 2, pf_half);
		cnt++;
	}
	printf("Sample cnt:%d\n", cnt);

	free(sample);
	fclose(pf);
	fclose(pf_half);
}

void  simplest_pcm16le_double_speed(char *path, char *file_name) {
	FILE *pf = fopen(strcat(path, file_name), "rb+");
	FILE *pf_d = fopen(strcat(path, "output_double_speed.pcm"), "wb+");

	int cnt = 0;

	unsigned char *sample = (unsigned char *)malloc(4);

	while (!feof(pf)) {
		fread(sample, 1, 4, pf);
		if (cnt % 2 != 0) {
			//left
			fwrite(sample, 1, 2, pf_d);
			//right
			fwrite(sample + 2, 1, 2, pf_d);
		}
		cnt++;
	}

	printf("Sample cnt:%d\n", cnt);

	free(sample);
	fclose(pf);
	fclose(pf_d);
}

void  simplest_pcm16le_to_pcm8(char *path, char *file_name) {
	FILE *pf = fopen(strcat(path, file_name), "rb+");
	FILE *pf_8b = fopen(strcat(path, "output_8b.pcm"), "wb+");

	int cnt = 0;

	unsigned char *sample = (unsigned char *)malloc(4);

	while (!feof(pf)) {
		short *sampl_num_16b = NULL;
		char sample_num_8b = 0;
		unsigned char sample_num_8bu = 0;

		fread(sample, 1, 4, pf);
		//left
		sampl_num_16b = (short *)sample;
		sample_num_8b = (*sampl_num_16b) >> 8;
		sample_num_8bu = sample_num_8b + 128;
		fwrite(&sample_num_8bu, 1, 1, pf_8b);
		//right
		sampl_num_16b = (short *)(sample + 2);
		sample_num_8b = (*sampl_num_16b) >> 8;
		sample_num_8bu = sample_num_8b + 128;
		fwrite(&sample_num_8bu, 1, 1, pf_8b);

		cnt++;
	}
	printf("Sample cnt:%d\n", cnt);

	free(sample);
	fclose(pf);
	fclose(pf_8b);
}

void simplest_pcm16le_cut_single_channel(char *path, char *file_name, int start_num, int dur_num) {
	FILE *pf = fopen(strcat(path, file_name), "rb+");
	FILE *pf_cut = fopen(strcat(path, "output_cut.pam"), "wb+");
	FILE *pf_start = fopen(strcat(path, "output_cut.txt"), "wb+");

	unsigned char *sample = (unsigned char *)malloc(2);

	int cnt = 0;

	while (!feof(pf)) {
		fread(sample, 1, 2, pf);
		if (cnt > start_num && cnt < (start_num + dur_num)) {
			fwrite(sample, 1, 2, pf_cut);

			short sample_num = sample[1];
			sample_num = sample_num * 256;
			sample_num = sample_num + sample[0];

			fprintf(pf_start, "%-6d,", sample_num);

			if (cnt % 10 == 0) {
				fprintf(pf_start, "\n", sample_num);
			}
		}
		cnt++;
	}

	free(sample);
	fclose(pf);
	fclose(pf_cut);
	fclose(pf_start);
}

int simplest_pcm16le_to_wave(char *path, char *pcm_file_name, char *wave_file_name, int channels, int sample_rate) {
	if (channels == 0 || sample_rate == 0) {
		channels = 2;
		sample_rate = 44100;
	}

	int bits = 16;

	wave_header pcm_head;
	wave_format pcm_fmt;
	wave_data pcm_data;

	unsigned short m_pcm_data;

	FILE *pf;
	FILE *pf_out;

	pf = fopen(strcat(path, pcm_file_name), "rb+");
	if (!pf) {
		printf("Open pcm file error.\n");
		return -1;
	}

	pf_out = fopen(strcat(path, wave_file_name), "wb+");
	if (!pf_out) {
		printf("Create wave file error.\n");
		return -1;
	}

	memcpy(pcm_head.fcc_id, "RIFF", strlen("RIFF"));
	memcpy(pcm_head.fcc_type, "WAVE", strlen("WAVE"));
	fseek(pf_out, sizeof(wave_header), SEEK_CUR);

	pcm_fmt.dw_samples_per_sec = sample_rate;
	pcm_fmt.dw_avg_bytes_per_sec = pcm_fmt.dw_samples_per_sec * sizeof(m_pcm_data);
	pcm_fmt.ui_bits_per_sample = bits;
	memcpy(pcm_fmt.fcc_id, "fmt ", strlen("fmt "));
	pcm_fmt.dw_size = 16;
	pcm_fmt.wblock_align = 2;
	pcm_fmt.wchannels = channels;
	pcm_fmt.wformat_tag = 1;

	fwrite(&pcm_fmt, sizeof(wave_format), 1, pf_out);

	memcpy(pcm_data.fcc_id, "data", strlen("data"));
	pcm_data.dw_size = 0;
	fseek(pf_out, sizeof(wave_data), SEEK_CUR);
	pcm_data.dw_size = 0;
	fseek(pf_out, sizeof(wave_data), SEEK_CUR);

	fread(&m_pcm_data, sizeof(unsigned short), 1, pf);

	while (!feof(pf)) {
		pcm_data.dw_size += 2;
		fwrite(&m_pcm_data, sizeof(unsigned short), 1, pf_out);
		fread(&m_pcm_data, sizeof(unsigned short), 1, pf);
	}

	pcm_head.dw_size = sizeof(wave_header) + sizeof(wave_format) + sizeof(wave_data) + pcm_data.dw_size;

	rewind(pf_out);
	fwrite(&pcm_head, sizeof(wave_header), 1, pf_out);
	fseek(pf_out, sizeof(wave_format), SEEK_CUR);
	fwrite(&pcm_data, sizeof(wave_data), 1, pf_out);

	fclose(pf);
	fclose(pf_out);
	return 0;
}