#include "DemuxDecode.h"


DemuxDecode::~DemuxDecode() {

}

void DemuxDecode::registerAll() {
	av_register_all();
}


int DemuxDecode::getStreamInfo(std::string fileName) {
	if (avformat_open_input(&avfmtCtx, fileName.c_str(), NULL, NULL) < 0) {
		std::cerr << "Could not open source file "
			<< fileName
			<< "."
			<< std::endl;
		return -1;
	}

	if (avformat_find_stream_info(avfmtCtx, NULL) < 0) {
		std::cerr << "Could not find stream information."
			<< std::endl;
		return -1;
	}
	return 0;
}


int DemuxDecode::openCodecContext(enum AVMediaType type, int &index) {
	index = av_find_best_stream(avfmtCtx, type, -1, -1, NULL, 0);
	if (index < 0) {
		std::cerr << "Could not find stream in input file."
			<< std::endl;
		return -1;
	}
	else {
		AVStream *stream = avfmtCtx->streams[index];
		AVCodecContext *decCtx = stream->codec;
		AVCodec *dec = avcodec_find_decoder(decCtx->codec_id);
		if (!dec) {
			std::cerr << "Failed to find codec."
				<< std::endl;
			return -1;
		}
		AVDictionary *opts = nullptr;
		if (avcodec_open2(decCtx, dec, &opts) < 0) {
			std::cerr << "Failed to find codec."
				<< std::endl;
			return -1;
		}
	}

	return 0;
}


int DemuxDecode::imageAlloc(uint8_t *pointers[4], int lineSizes[4], int w, int h, enum AVPixelFormat pixFmt, int align) {
	return av_image_alloc(pointers, lineSizes, w, h, pixFmt, align);
}


void DemuxDecode::dumpFormat(std::string fileName, int isOutput) {
	av_dump_format(avfmtCtx, 0, fileName.c_str(), isOutput);
}


AVFrame* DemuxDecode::frameAlloc() {
	return av_frame_alloc();
}


void DemuxDecode::initPacket(AVPacket *pkt) {
	av_init_packet(pkt);
	pkt->data = nullptr;
	pkt->size = 0;
}


int DemuxDecode::decodePacket(AVCodecContext *decCtx, AVPacket *pkt, int *gotFrame, int vindex) {
	int ret = 0;
	int decoded = pkt->size;

	*gotFrame = 0;

	if (pkt->stream_index == vindex) {
		ret = avcodec_decode_video2(decCtx, frame, gotFrame, pkt);
		if (ret < 0) {
			std::cerr << "Error decoding video frame." 
				<< std::endl;
			return -1;
		}

		if (*gotFrame) {
			av_image_copy(vdstData, vdstLineSize,(const uint8_t **)(frame->data), 
				frame->linesize, vpixFmt, width, height);

			fwrite(vdstData[0], 1, vdstBufSize, vdstFile);
		}
	}

	return decoded;
}


int DemuxDecode::readFrame(AVPacket *pkt) {
	return av_read_frame(avfmtCtx, pkt);
}


void DemuxDecode::freePacket(AVPacket *pkt) {
	av_free_packet(pkt);
}


int DemuxDecode::codecClose(AVCodecContext *avctx) {
	return avcodec_close(avctx);
}


void DemuxDecode::formatCloseInput(AVFormatContext **s) {
	avformat_close_input(s);
}


void DemuxDecode::frameFree(AVFrame **frame) {
	av_frame_free(frame);
}


void DemuxDecode::memoryFree(void *ptr) {
	av_free(ptr);
}


int DemuxDecode::testDemuxDecodec(const std::string fileName, const std::string vdstFileName) {
	registerAll();
	if (getStreamInfo(fileName) < 0) {
		return -1;
	}

	int vindex;
	if (openCodecContext(AVMEDIA_TYPE_VIDEO, vindex) < 0) {
		return -1;
	}
	AVStream *vstream = avfmtCtx->streams[vindex];
	AVCodecContext *vdecCtx = vstream->codec;



	vdstFile = fopen(vdstFileName.c_str(), "wb");
	if (!vdstFile) {
		std::cerr << "Could not open destination file."
			<< std::endl;
		return -1;
	}

	
	width = vdecCtx->width;
	height = vdecCtx->height;
	vpixFmt = vdecCtx->pix_fmt;

	if ((vdstBufSize = imageAlloc(vdstData, vdstLineSize, width, height, vpixFmt, 1)) < 0) {
		return -1;
	}
	
	dumpFormat(fileName, 0);

	if (!vstream) {
		std::cerr << "Could not find audio or video stream in the input."
			<< std::endl;
		return -1;
	}
	
	frame = frameAlloc();
	if (!frame) {
		std::cerr << "Could not allocate frame."
			<< std::endl;
	}

	initPacket(&pkt);
	pkt.data = NULL;
	pkt.size = 0;

	if (vstream) {
		std::cout << "Demuxing video from file "
			<< fileName
			<< " into "
			<< vdstFileName
			<< "."
			<< std::endl;
	}

	int gotFrame;
	while (readFrame(&pkt) >= 0) {
		AVPacket origPkt = pkt;
		do {
			int ret = decodePacket(vdecCtx, &pkt, &gotFrame, vindex);
			if (ret < 0)
				break;
			pkt.data += ret;
			pkt.size -= ret;
		} while (pkt.size > 0);
		freePacket(&origPkt);
	}

	pkt.data = NULL;
	pkt.size = 0;
	do {
		decodePacket(vdecCtx, &pkt, &gotFrame, vindex);
	} while (gotFrame);

	std::cout << "Demuxing succeeded." << std::endl;

	codecClose(vdecCtx);
	formatCloseInput(&avfmtCtx);
	if (vdstFile) {
		fclose(vdstFile);
	}
	frameFree(&frame);
	memoryFree(vdstData[0]);
	return 0;
}
