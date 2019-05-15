#include "UdpRtpParse.h"
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")


int simplest_udp_parser(int port) {
	WSADATA wsa_data;
	WORD socket_ver = MAKEWORD(2, 2);

	FILE *out = stdout;

	FILE *pf_ts = fopen("output_dump.ts", "wb+");

	if (WSAStartup(socket_ver, &wsa_data) != 0)
		return -1;

	SOCKET ser_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ser_socket == INVALID_SOCKET) {
		printf("Socket error.\n");
		return -1;
	}

	sockaddr_in ser_addr;
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(port);
	ser_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(ser_socket, (sockaddr *)&ser_addr, sizeof(ser_addr)) == SOCKET_ERROR) {
		printf("Bind error.\n");
		closesocket(ser_socket);
		return -1;
	}

	sockaddr_in remote_addr;
	int addr_len = sizeof(remote_addr);
	
	int parse_rtp = 1;
	int parse_mpeg_ts = 1;

	printf("Listening on port %d\n", port);

	char recv_data[10000];
	int cnt = 0;
	while (1) {
		int pkt_size = recvfrom(ser_socket, recv_data, 10000, 0, (sockaddr *)&remote_addr, &addr_len);
		if (pkt_size > 0) {
			if (parse_rtp != 0) {
				char payload_str[10] = { 0 };
				rtp_fixed_header rtp_header;
				int rtp_header_size = sizeof(rtp_fixed_header);
				memcpy((void *)&rtp_header, recv_data, rtp_header_size);

				char payload = rtp_header.payload;
				switch (payload) {
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18: sprintf(payload_str, "Audio"); break;
				case 31: sprintf(payload_str, "H.261"); break;
				case 32: sprintf(payload_str, "MPV"); break;
				case 33: sprintf(payload_str, "MP2T"); break;
				case 34: sprintf(payload_str, "H.263"); break;
				case 96: sprintf(payload_str, "H.264"); break;
				default:sprintf(payload_str, "other"); break;
				}

				unsigned int timestamp = ntohl(rtp_header.timestamp);
				unsigned int seq_no = ntohs(rtp_header.seq_no);

				fprintf(out, "[RTP Pkt] %5d| %5s| %10u| %5d| %5d|\n", cnt, payload_str, timestamp, seq_no, pkt_size);

				char *rtp_data = recv_data + rtp_header_size;
				int rtp_data_size = pkt_size - rtp_header_size;
				fwrite(rtp_data, rtp_data_size, 1, pf_ts);

				if (parse_mpeg_ts != 0 && payload == 33) {
					for (int i = 0; i < rtp_data_size; i+=188) {
						if (rtp_data[i] != 0x47)
							break;
						fprintf(out, "  [MPEGTS Pkt]\n");
					}
				}
			}
			else {
				fprintf(out, "[UDP Pkt] %5d| %5d|\n", cnt, pkt_size);
				fwrite(recv_data, pkt_size, cnt, pf_ts);
			}
			cnt++;
		}
	}
	closesocket(ser_socket);
	WSACleanup();
	fclose(pf_ts);
	return 0;
}