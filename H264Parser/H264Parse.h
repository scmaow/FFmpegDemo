/**
 * H.264码流数据解析
 *
 * 05/14/2019
*/
#pragma once
#ifndef H264_PARSE_H
#define H264_PARSE_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

enum nalu_type {
	nalu_type_slice    = 1,
	nalu_type_dpa      = 2,
	nalu_type_dpb      = 3,
	nalu_type_dpc      = 4,
	nalu_type_idr      = 5,
	nalu_type_sei      = 6,
	nalu_type_sps      = 7,
	nalu_type_pps      = 8,
	nalu_type_aud      = 9,
	nalu_type_eoseq    = 10,
	nalu_type_eostream = 11,
	nalu_type_fill     = 12
};
typedef enum nalu_type nalu_type;

enum nalu_priority {
	nalu_priority_disposable = 0,
	nalu_priority_low        = 1,
	nalu_priority_high       = 2,
	nalu_priority_highest    = 3
};
typedef enum nalu_priority nalu_priority;

struct nalu_t {
	int start_code_prefix_len;  //! 4 for parameter sets and first slice in picture, 3 for everything else (suggested)
	unsigned int len;           //! Length of the NAL unit (Excluding the start code, which does not belong to the NALU)
	unsigned int max_size;		//! Nal Unit Buffer size
	int forbidden_bit;			//! should be always FALSE
	int nalu_reference_idc;		//! NALU_PRIORITY_xxxx
	int nalu_unit_type;			//! NALU_TYPE_xxxx
	char *buf;					//! contains the first byte followed by the EBSP
};
typedef struct nalu_t nalu_t;

int find_start_code2(unsigned char *buf);

int find_start_code3(unsigned char *buf);

int get_annexb_nalu(nalu_t *nalu);

int simplest_h264_parser(const char *url);



#endif // !H264_PARSE_H

