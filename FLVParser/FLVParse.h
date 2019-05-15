/**
 * FLV封装格式解析
 *
 * 05/15/2019
*/


#ifndef FLV_PARSE_H
#define FLV_PARSE_H

#include <cstdio>
#include <string>

#pragma pack(1)

typedef unsigned char byte;
typedef unsigned int uint;


//flv file header
struct flv_header {
	byte signature[3];
	byte version;
	byte flags;
	uint data_offset;
};
typedef struct flv_header flv_header;

//tag header
struct tag_header {
	byte tag_type;
	byte data_size[3];
	byte time_stamp[3];
	uint reserved;
};
typedef struct tag_header tag_header;

uint reverse_bytes(byte *p, char c);

int simplest_flv_parser(std::string path, std::string flv_file_name);






#endif // !FLV_PARSE_H
