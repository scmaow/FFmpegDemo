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
