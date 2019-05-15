/**
 * UDP-RTP–≠“ÈΩ‚Œˆ
 *
 * 05/16/2019
*/
#ifndef UDP_RTP_PARSE_H
#define UDP_RTP_PARSE_H

#include <cstdio>

#pragma pack(1)


struct rtp_fixed_header {
	/* byte 0 */
	unsigned char csrc_len : 4;       /* expect 0 */
	unsigned char extension : 1;      /* expect 1 */
	unsigned char padding : 1;        /* expect 0 */
	unsigned char version : 2;        /* expect 2 */
									  /* byte 1 */
	unsigned char payload : 7;
	unsigned char marker : 1;        /* expect 1 */
									 /* bytes 2, 3 */
	unsigned short seq_no;
	/* bytes 4-7 */
	unsigned  long timestamp;
	/* bytes 8-11 */
	unsigned long ssrc;            /* stream number is used here. */
};
typedef struct rtp_fixed_header rtp_fixed_header;

struct mpegts_fixed_header {
	unsigned sync_byte : 8;
	unsigned transport_error_indicator : 1;
	unsigned payload_unit_start_indicator : 1;
	unsigned transport_priority : 1;
	unsigned pid : 13;
	unsigned scrambling_control : 2;
	unsigned adaptation_field_exist : 2;
	unsigned continuity_counter : 4;
};
typedef struct mpegts_fixed_header mpegts_fixed_header;

int simplest_udp_parser(int port);


#endif // !UDP_RTP_PARSE_H