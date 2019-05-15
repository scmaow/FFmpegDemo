#include "FLVParse.h"
#include <cstdlib>
#include <cstring>

#define TAG_TYPE_AUDIO  8
#define TAG_TYPE_VIDEO  9
#define TAG_TYPE_SCRIPT 18

#define CHANGE_TIME_STAMP 0

uint reverse_bytes(byte *p, char c) {

	int r = 0;
	for (int i = 0; i < c; i++) {
		r |= (*(p + i) << (((c - 1) * 8) - 8 * i));
	}
	return r;
}


int simplest_flv_parser(std::string path, std::string flv_file_name) {
	int output_a = 1;
	int output_v = 1;

	FILE *pf = NULL;
	FILE *pf_v = NULL;
	FILE *pf_a = NULL;

	FILE *out = stdout;

	flv_header flv_head;
	tag_header tag_head;

	uint previous_tags_size;
	uint previous_tag_size_z = 0;

	uint ts = 0;
	uint ts_new = 0;

	pf = fopen((path + flv_file_name).c_str(), "rb+");
	if (!pf) {
		printf("Failed to open files.\n");
		return -1;
	}
	//读flv file header
	fread((char *)&flv_head, 1, sizeof(flv_header), pf);

	fprintf(out, "============== FLV Header ==============\n");
	fprintf(out, "Signature:  0x %c %c %c\n", flv_head.signature[0], flv_head.signature[1], flv_head.signature[2]);
	fprintf(out, "Version:    0x %X\n", flv_head.version);
	fprintf(out, "Flags  :    0x %X\n", flv_head.flags);
	fprintf(out, "HeaderSize: 0x %X\n", reverse_bytes((byte *)&flv_head.data_offset, sizeof(flv_head.data_offset)));
	fprintf(out, "========================================\n");
	
	//定位到flv file body开始
	fseek(pf, reverse_bytes((byte *)&flv_head.data_offset, sizeof(flv_head.data_offset)), SEEK_SET);

	do {
		previous_tags_size = _getw(pf);
		//读tag header
		fread((void *)&tag_head, sizeof(tag_header), 1, pf);

		int tag_head_data_size = tag_head.data_size[0] * 65536 + tag_head.data_size[1] * 256 + tag_head.data_size[2];
		int tag_head_time_stamp = tag_head.time_stamp[0] * 65536 + tag_head.time_stamp[1] * 256 + tag_head.time_stamp[2];

		char tag_type_str[10];

		switch (tag_head.tag_type) {
		case TAG_TYPE_AUDIO:sprintf(tag_type_str, "AUDIO"); break;
		case TAG_TYPE_VIDEO:sprintf(tag_type_str, "VIDEO"); break;
		case TAG_TYPE_SCRIPT:sprintf(tag_type_str, "SCRIPT"); break;
		default:sprintf(tag_type_str, "UNKNOWN"); break;
		}

		fprintf(out, "[%6s] %6d %6d |", tag_type_str, tag_head_data_size, tag_head_time_stamp);

		if (feof(pf))
			break;

		switch (tag_head.tag_type) {
		case TAG_TYPE_AUDIO: {
			char audio_tag_str[100] = { 0 };
			strcat(audio_tag_str, "| ");
			char tag_data_first_byte;
			tag_data_first_byte = fgetc(pf);   //tag data部分开始  第1个字节是音频数据参数
			int x = tag_data_first_byte & 0xf0; //前 4 bit 音频编码类型
			x = x >> 4;
			switch (x) {
			case 0:strcat(audio_tag_str, "Linear PCM, platform endian"); break;
			case 1:strcat(audio_tag_str, "ADPCM"); break;
			case 2:strcat(audio_tag_str, "MP3"); break;
			case 3:strcat(audio_tag_str, "Linear PCM, little endian"); break;
			case 4:strcat(audio_tag_str, "Nellymoser 16-kHz mono"); break;
			case 5:strcat(audio_tag_str, "Nellymoser 8-kHz mono"); break;
			case 6:strcat(audio_tag_str, "Nellymoser"); break;
			case 7:strcat(audio_tag_str, "G.711 A-law logarithmic PCM"); break;
			case 8:strcat(audio_tag_str, "G.711 mu-law logarithmic PCM"); break;
			case 9:strcat(audio_tag_str, "reserved"); break;
			case 10:strcat(audio_tag_str, "AAC"); break;
			case 11:strcat(audio_tag_str, "Speex"); break;
			case 14:strcat(audio_tag_str, "MP3 8-Khz"); break;
			case 15:strcat(audio_tag_str, "Device-specific sound"); break;
			default:strcat(audio_tag_str, "UNKNOWN"); break;
			}
			strcat(audio_tag_str, "|");

			x = tag_data_first_byte & 0x0c; //音频采样率
			x = x >> 2;
			switch (x)
			{
			case 0:strcat(audio_tag_str, "5.5-kHz"); break;
			case 1:strcat(audio_tag_str, "1-kHz"); break;
			case 2:strcat(audio_tag_str, "22-kHz"); break;
			case 3:strcat(audio_tag_str, "44-kHz"); break;
			default:strcat(audio_tag_str, "UNKNOWN"); break;
			}
			strcat(audio_tag_str, "| ");

			x = tag_data_first_byte & 0x02; //音频采样精度
			x = x >> 1;
			switch (x)
			{
			case 0:strcat(audio_tag_str, "8Bit"); break;
			case 1:strcat(audio_tag_str, "16Bit"); break;
			default:strcat(audio_tag_str, "UNKNOWN"); break;
			}
			strcat(audio_tag_str, "| ");

			x = tag_data_first_byte & 0x01; //音频类型
			switch (x){
			case 0:strcat(audio_tag_str, "Mono"); break;
			case 1:strcat(audio_tag_str, "Stereo"); break;
			default:strcat(audio_tag_str, "UNKNOWN"); break;
			}
			fprintf(out, "%s", audio_tag_str);

			//if the output file hasn't been opened, open it.
			if (output_a != 0 && pf_a == NULL) {
				pf_a = fopen((path + "output.mp3").c_str(), "wb+");
			}

			//TagData - First Byte Data
			int data_size = reverse_bytes((byte *)&tag_head.data_size, sizeof(tag_head.data_size)) - 1; //tag_data第一字节是参数
			if (output_a != 0) {
				//TagData+1 从tag_data的第二字节开始
				for (int i = 0; i < data_size; i++)
					fputc(fgetc(pf), pf_a);

			}
			else {
				for (int i = 0; i < data_size; i++)
					fgetc(pf);
			}
			break;
		}
		case TAG_TYPE_VIDEO: {
			char video_tag_str[100] = { 0 };
			strcat(video_tag_str, "| ");
			char tag_data_first_byte;
			tag_data_first_byte = fgetc(pf); //tag data部分开始  第1个字节是视频数据参数
			int x = tag_data_first_byte & 0xf0; //前 4 bit 帧类型
			x = x >> 4;
			switch (x)
			{
			case 1:strcat(video_tag_str, "key frame  "); break;
			case 2:strcat(video_tag_str, "inter frame"); break;
			case 3:strcat(video_tag_str, "disposable inter frame"); break;
			case 4:strcat(video_tag_str, "generated keyframe"); break;
			case 5:strcat(video_tag_str, "video info/command frame"); break;
			default:strcat(video_tag_str, "UNKNOWN"); break;
			}
			strcat(video_tag_str, "| ");

			x = tag_data_first_byte & 0x0f; //后 4 bit 视频编码类型
			switch (x)
			{
			case 1:strcat(video_tag_str, "JPEG (currently unused)"); break;
			case 2:strcat(video_tag_str, "Sorenson H.263"); break;
			case 3:strcat(video_tag_str, "Screen video"); break;
			case 4:strcat(video_tag_str, "On2 VP6"); break;
			case 5:strcat(video_tag_str, "On2 VP6 with alpha channel"); break;
			case 6:strcat(video_tag_str, "Screen video version 2"); break;
			case 7:strcat(video_tag_str, "AVC"); break;
			default:strcat(video_tag_str, "UNKNOWN"); break;
			}
			fprintf(out, "%s", video_tag_str);

			fseek(pf, -1, SEEK_CUR);
			//if the output file hasn't been opened, open it.
			if (pf_v == NULL && output_v != 0) {
				//write the flv header (reuse the original file's header) and first previoustagsize
				pf_v = fopen((path + "output.flv").c_str(), "wb+");
				fwrite((char *)&flv_head, 1, sizeof(flv_header), pf_v);
				fwrite((char *)&previous_tag_size_z, 1, sizeof(previous_tag_size_z), pf_v);
			}
#if CHANGE_TIME_STAMP
			//Change Timestamp
			//Get Timestamp
			ts = reverse_bytes((byte *)&tag_head.time_stamp, sizeof(tag_head.time_stamp));
			ts = ts * 2;
			//Writeback Timestamp
			ts_new = reverse_bytes((byte *)&ts, sizeof(ts));
			memcpy(&tag_head.time_stamp, ((char *)&ts_new) + 1, sizeof(tag_head.time_stamp));
#endif


			//TagData + Previous Tag Size
			int data_size = reverse_bytes((byte *)&tag_head.data_size, sizeof(tag_head.data_size)) + 4;
			if (output_v != 0) {
				//TagHeader
				fwrite((char *)&tag_head, 1, sizeof(tag_header), pf_v);
				//TagData
				for (int i = 0; i<data_size; i++)
					fputc(fgetc(pf), pf_v);
			}
			else {
				for (int i = 0; i<data_size; i++)
					fgetc(pf);
			}
			//rewind 4 bytes, because we need to read the previoustagsize again for the loop's sake
			fseek(pf, -4, SEEK_CUR);

			break;
		}
		default:
			//skip the data of this tag
			fseek(pf, reverse_bytes((byte *)&tag_head.data_size, sizeof(tag_head.data_size)), SEEK_CUR);
		}

		fprintf(out, "\n");
	} while (!feof(pf));

	_fcloseall();
	return 0;
}
