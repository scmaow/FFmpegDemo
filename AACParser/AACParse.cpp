#include "AACParse.h"
#include <cstdlib>
#include <cstring>


int get_adts_frame(unsigned char *buf, int buf_size, unsigned char *data, int *data_size) {
	if (!buf || !data || !data_size)
		return -1;

	int size = 0;
	while (1) {
		if (buf_size < 7)
			return -1;
		//¼ÆËãframe³¤¶È(frame header + raw data)
		//Sync words
		if ((buf[0] == 0xff) && ((buf[1] & 0xf0) == 0xf0)) {
			size |= ((buf[3] & 0x03) << 11); //high 2 bit
			size |= buf[4] << 3; //middle 8 bit
			size |= ((buf[5] & 0xe0) >> 5); //low 3 bit
			break;
		}
		--buf_size;
		++buf;
	}

	if (buf_size < size)
		return 1;

	memcpy(data, buf, size);
	*data_size = size;
	return 0;
}

int simplest_aac_parser(const char *url) {
	FILE *out = stdout;

	unsigned char *acc_frame = (unsigned char *)malloc(1024 * 5);
	unsigned char *acc_buf = (unsigned char *)malloc(1024 * 1024);

	FILE *pf = fopen(url, "rb");
	if (!pf) {
		printf("Open file error.\n");
		return -1;
	}

	printf("-----+- ADTS Frame Table -+------+\n");
	printf(" NUM | Profile | Frequency| Size |\n");
	printf("-----+---------+----------+------+\n");

	int data_size = 0;
	int size = 0;
	int cnt = 0;
	int offset = 0;

	while (!feof(pf)) {
		data_size = fread(acc_buf + offset, 1, 1024 * 1024 - offset, pf);
		unsigned char *input_data = acc_buf;

		while (1) {
			int ret = get_adts_frame(input_data, data_size, acc_frame, &size);
			if (ret == -1) {
				break;
			}
			else if (ret == 1) {
				memcpy(acc_buf, input_data, data_size);
				offset = data_size;
				break;
			}

			char profile_str[10] = { 0 };
			char frequence_str[10] = { 0 };

			unsigned char profile = acc_frame[2] & 0xc0;
			profile = profile >> 6;

			switch (profile) {
			case 0: sprintf(profile_str, "Main"); break;
			case 1: sprintf(profile_str, "LC"); break;
			case 2: sprintf(profile_str, "SSR"); break;
			default:sprintf(profile_str, "unknown"); break;
			}

			unsigned char sampling_frequency_index = acc_frame[2] & 0x3c;
			sampling_frequency_index = sampling_frequency_index >> 2;
			switch (sampling_frequency_index) {
			case 0: sprintf(frequence_str, "96000Hz"); break;
			case 1: sprintf(frequence_str, "88200Hz"); break;
			case 2: sprintf(frequence_str, "64000Hz"); break;
			case 3: sprintf(frequence_str, "48000Hz"); break;
			case 4: sprintf(frequence_str, "44100Hz"); break;
			case 5: sprintf(frequence_str, "32000Hz"); break;
			case 6: sprintf(frequence_str, "24000Hz"); break;
			case 7: sprintf(frequence_str, "22050Hz"); break;
			case 8: sprintf(frequence_str, "16000Hz"); break;
			case 9: sprintf(frequence_str, "12000Hz"); break;
			case 10: sprintf(frequence_str, "11025Hz"); break;
			case 11: sprintf(frequence_str, "8000Hz"); break;
			default:sprintf(frequence_str, "unknown"); break;
			}
			fprintf(out, "%5d| %8s|  %8s| %5d|\n", cnt, profile_str, frequence_str, size);
			data_size -= size;
			input_data += size;
			cnt++;
		}
	}

	fclose(pf);
	free(acc_buf);
	free(acc_frame);
	return 0;
}