/**
 * AAC码流数据解析
 *
 * 05/14/2019
*/
#ifndef AAC_PARSE_H
#define AAC_PARSE_H

#include <cstdio>

int get_adts_frame(unsigned char *buf, int buf_size, unsigned char *data, int *data_size);

int simplest_aac_parser(const char *url);


#endif // !AAC_PARSE_H

