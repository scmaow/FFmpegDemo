#pragma once
#ifndef DEMUX_DECODE_H
#define DEMUX_DECODE_H

#include <iostream>
#include <string>
extern "C" {
#include "libavutil/imgutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/timestamp.h"
#include "libavformat/avformat.h"
};

using namespace std;

class DemuxDecode {
public:
	DemuxDecode()=default;
	~DemuxDecode();


	void registerAll();
	int getStreamInfo(const std::string fileName);
	int openCodecContext(enum AVMediaType type, int &index);
	int imageAlloc(uint8_t *pointers[4], int lineSizes[4], int w, int h, enum AVPixelFormat pixFmt, int align);
	void dumpFormat(std::string fileName, int isOutput);
	AVFrame* frameAlloc();
	void initPacket(AVPacket *pkt);
	int decodePacket(AVCodecContext *decCtx,AVPacket *pkt, int *gotFrame, int vindex);
	int readFrame(AVPacket *pkt);
	void freePacket(AVPacket *pkt);
	int codecClose(AVCodecContext *avctx);
	void formatCloseInput(AVFormatContext **s);
	void frameFree(AVFrame **frame);
	void memoryFree(void *ptr);

	int testDemuxDecodec(const std::string fileName, const std::string vdstFileName);


private:
	AVFormatContext *avfmtCtx;
	AVPacket		pkt;
	AVFrame			*frame;
	FILE			*vdstFile;

	int	width;
	int	height;
	AVPixelFormat vpixFmt;;

	uint8_t *vdstData[4];
	int vdstLineSize[4];
	int vdstBufSize;
};
#endif // !DEMUX_DECODE_H


