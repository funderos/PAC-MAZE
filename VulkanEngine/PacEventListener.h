#ifndef PACEVENTLISTENER_H
#define PACEVENTLISTENER_H

#include <iostream>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
}

namespace ve {
	class PacEventListener : public VEEventListener {
	protected:
		float speed = 50.0f;
		float rageTime = 0.0f;
		float cageTime = 10.0f;
		float playTime = 0.0f;

		//bool paused = true;
		//bool intro = false;
		//bool gameOver = false;
		//bool startscreen = false;
		int rageScore = 0;

		AVCodecContext* c;
		SwsContext* ctx;
		AVFrame* picture;
		AVPacket* pkt;

		SOCKET udp_sock;
		SOCKADDR_IN udp_addr;
		SOCKADDR_IN udp_remoteAddr;

		SOCKET tcp_acceptSocket;
		SOCKET tcp_connectedSocket;
		SOCKADDR_IN tcp_addr;

		int ret;
		unsigned long framenumber;
		double timeCache = 0;

		virtual void onFrameStarted(veEvent event);
		virtual void onFrameEnded(veEvent event);
		virtual bool onKeyboard(veEvent event);
		virtual bool onMouseScroll(veEvent event);
		virtual void onDrawOverlay(veEvent event);

		virtual void initWorld();
		virtual int getDirection(int x, int y, double angle, bool enemy);
		virtual int getOpenWays(int x, int y);
		virtual std::string getMeshName(int x, int y);
		virtual float getMeshRotation(int x, int y);

		virtual void encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt);
		virtual void encodeStart();
		virtual void initUDP();
		virtual void sendFrameOverUDP(char* data, int size);


	public:
		///Constructor
		PacEventListener(std::string name) : VEEventListener(name) { initWorld(); };
		///Destructor
		virtual ~PacEventListener();
	};
}

#endif