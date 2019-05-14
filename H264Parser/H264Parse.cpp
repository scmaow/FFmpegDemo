#include "H264Parse.h"

const int NALU_BUFFER_MAX_SIZE = 10000000;

FILE *h264_bits_stream = NULL;

int find_start_code2(unsigned char *buf) {
	if (buf[0] != 0 || buf[1] != 0 || buf[2] != 1)
		return -1;
	else
		return 0;
}

int find_start_code3(unsigned char *buf) {
	if (buf[0] != 0 || buf[1] != 0 || buf[2] != 0 || buf[3] != 1)
		return -1;
	else
		return 0;
}

int get_annexb_nalu(nalu_t *nalu) {
	unsigned char *buf;
	if (!(buf = (unsigned char *)calloc(nalu->max_size, sizeof(char)))) {
		printf("Could not allocate buf memory.\n");
		return -1;
	}

	//step1: 寻找nalu的startcode
	nalu->start_code_prefix_len = 3;

	if (3 != fread(buf, 1, 3, h264_bits_stream)) {
		free(buf);
		return -1;
	}

	int pos = 0;
	int start_code2;
	int start_code3;
	start_code2 = find_start_code2(buf);
	if (0 != start_code2) {
		if (1 != fread(buf + 3, 1, 1, h264_bits_stream)) {
			free(buf);
			return -1;
		}
		start_code3 = find_start_code3(buf);
		if (0 != start_code3) {
			free(buf);
			printf("Format error.\n");
			return -1; //格式错误
		}
		else {
			pos = 4;
			nalu->start_code_prefix_len = 4;
		}
	}
	else {
		pos = 3;
		nalu->start_code_prefix_len = 3;
	}

	//step2: 获取nalu的长度，到下一个nalu的startcode或文件结束为止
	int start_code_found = 0; //没找到
	start_code2 = -1;
	start_code3 = -1;
	while (!start_code_found) {
		//文件结束
		if (feof(h264_bits_stream)) {
			nalu->len = (pos - 1) - nalu->start_code_prefix_len;
			memcpy(nalu->buf, &buf[nalu->start_code_prefix_len], nalu->len);
			//nalu header
			nalu->forbidden_bit = nalu->buf[0] & 0x80; //1 bit
			nalu->nalu_reference_idc = nalu->buf[0] & 0x60; //2 bits
			nalu->nalu_unit_type = (nalu->buf[0]) & 0x1f; //5 bits
			free(buf);
			return pos - 1;
		}
		//寻找下一个nalu的开始代码
		buf[pos++] = fgetc(h264_bits_stream);
		start_code3 = find_start_code3(&buf[pos - 4]);
		if (0 != start_code3)
			start_code2 = find_start_code2(&buf[pos - 3]);
		start_code_found = (start_code2 == 0 || start_code3 == 0);
	}

	int rewind = (start_code3 == 0) ? -4 : -3;  //下一个nalu开始代码的长度
	//将文件指针定位到下一个nalu的startcode开始处
	if (0 != fseek(h264_bits_stream, rewind, SEEK_CUR)) {
		free(buf);
		printf("Cannot fseek in the bit stream file.\n");
		return -1;
	}
	//step3: 设置nalu相关域的值
	nalu->len = (pos + rewind) - nalu->start_code_prefix_len; //去掉下一个nalu的开始代码长度
	memcpy(nalu->buf, &buf[nalu->start_code_prefix_len], nalu->len);
	nalu->forbidden_bit = nalu->buf[0] & 0x80;  //1 bit
	nalu->nalu_reference_idc = nalu->buf[0] & 0x60;  // 2 bits
	nalu->nalu_unit_type = (nalu->buf[0] & 0x1f);  // 5 bits
	free(buf);

	return(pos + rewind);
}

int simplest_h264_parser(const char *url) {
	h264_bits_stream = fopen(url, "rb+");
	if (!h264_bits_stream) {
		printf("Open file error.\n");
		return -1;
	}

	nalu_t *nalu = (nalu_t *)calloc(1, sizeof(nalu_t));
	if (!nalu) {
		fclose(h264_bits_stream);
		printf("Alloc nalu error.\n");
		return -1;
	}

	int buf_size = NALU_BUFFER_MAX_SIZE;
	nalu->max_size = buf_size;
	nalu->buf = (char *)calloc(buf_size, sizeof(char));
	if (!nalu->buf) {
		free(nalu->buf);
		free(nalu);
		fclose(h264_bits_stream);
		printf("Alloc nalu buf error.\n");
		return -1;
	}

	int data_offset = 0;
	int nalu_num = 0;
	printf("-----+-------- NALU Table ------+---------+\n");
	printf(" NUM |    POS  |    IDC |  TYPE |   LEN   |\n");
	printf("-----+---------+--------+-------+---------+\n");

	FILE *out = stdout;
	while (!feof(h264_bits_stream)) {
		int data_len = get_annexb_nalu(nalu);
		if (-1 == data_len) {
			free(nalu->buf);
			free(nalu);
			fclose(h264_bits_stream);
			return -1;
		}

		char type_str[20] = { 0 };

		switch (nalu->nalu_unit_type) {
		case nalu_type_slice:sprintf(type_str, "SLICE"); break;
		case nalu_type_dpa:sprintf(type_str, "DPA"); break;
		case nalu_type_dpb:sprintf(type_str, "DPB"); break;
		case nalu_type_dpc:sprintf(type_str, "DPC"); break;
		case nalu_type_idr:sprintf(type_str, "IDR"); break;
		case nalu_type_sei:sprintf(type_str, "SEI"); break;
		case nalu_type_sps:sprintf(type_str, "SPS"); break;
		case nalu_type_pps:sprintf(type_str, "PPS"); break;
		case nalu_type_aud:sprintf(type_str, "AUD"); break;
		case nalu_type_eoseq:sprintf(type_str, "EOSEQ"); break;
		case nalu_type_eostream:sprintf(type_str, "EOSTREAM"); break;
		case nalu_type_fill:sprintf(type_str, "FILL"); break;
		}

		char idc_str[20] = { 0 };
		switch (nalu->nalu_reference_idc >> 5) {
		case nalu_priority_disposable:sprintf(idc_str, "DISPOS"); break;
		case nalu_priority_low:sprintf(idc_str, "LOW"); break;
		case nalu_priority_high:sprintf(idc_str, "HIGH"); break;
		case nalu_priority_highest:sprintf(idc_str, "HIGHEST"); break;
		}

		fprintf(out, "%5d| %8d| %7s| %6s| %8d|\n", nalu_num, data_offset, idc_str, type_str, nalu->len);

		data_offset = data_offset + data_len;
		nalu_num++;
	}

	free(nalu->buf);
	nalu->buf = NULL;
	free(nalu);
	fclose(h264_bits_stream);
	return 0;
}