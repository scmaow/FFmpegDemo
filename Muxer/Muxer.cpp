// Muxer.cpp: 定义控制台应用程序的入口点。
//

#include <iostream>
#include <string>
#include <cstdio>
extern "C"{
#include "libavformat/avformat.h"
};

using namespace std;

#define USE_H264BSF 0
#define USE_AACBSF 0

int main() {

	AVOutputFormat *ofmt = nullptr;
	AVFormatContext *ifmtCtxVideo = nullptr;
	AVFormatContext *ifmtCtxAudio = nullptr;
	AVFormatContext *ofmtCtx = nullptr;
	AVPacket pkt;

	int vindex = -1;
	int aindex = -1;
	int ovindex = -1;
	int oaindex = -1;

	int indexFrame = 0;

	int64_t curVideoPts = 0;
	int64_t curAudioPts = 0;

	const char *inVideoFileName = "";
	const char *inAudioFileName = "";
	const char *outFileName = "";

	av_register_all();

	if (avformat_open_input(&ifmtCtxVideo, inVideoFileName, 0, 0) < 0) {
		std::cout << "Could not open input file." << std::endl;
		goto end;
	}

	if (avformat_find_stream_info(ifmtCtxVideo, 0) < 0) {
		std::cout << "Failed to retrieve input stream information." << std::endl;
		goto end;
	}

	if (avformat_open_input(&ifmtCtxAudio, inAudioFileName, 0, 0) < 0) {
		std::cout << "Could not open input file." << std::endl;
		goto end;
	}

	if (avformat_find_stream_info(ifmtCtxAudio, 0) < 0) {
		std::cout << "Failed to retrieve input stream information." << std::endl;
		goto end;
	}

	std::cout << "===========Input Information==========" << std::endl;
	av_dump_format(ifmtCtxVideo, 0, inVideoFileName, 0);
	av_dump_format(ifmtCtxAudio, 0, inAudioFileName, 0);
	std::cout << "===========Input Information==========" << std::endl;
	
	avformat_alloc_output_context2(&ofmtCtx, NULL, NULL, outFileName);
	if (!ofmtCtx) {
		std::cout << "Could not create output context." << std::endl;
		goto end;
	}
	ofmt = ofmtCtx->oformat;

	for (unsigned int i = 0; i < ifmtCtxVideo->nb_streams; i++) {
		if (ifmtCtxVideo->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {

		}
	}





end:

    return 0;
}

