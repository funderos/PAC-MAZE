#ifndef PACEVENTLISTENER_H
#define PACEVENTLISTENER_H

namespace ve {
	class PacEventListener : public VEEventListener {
	protected:
		bool  m_usePrevCursorPosition = false;			///<Can I use the previous cursor position for moving the camera?
		bool  m_rightButtonClicked = false;				///<Is the left button currently clicked?
		float m_cursorPrevX = 0;						///<Previous X position of cursor
		float m_cursorPrevY = 0;						///<Previous Y position of cursor
		bool  m_makeScreenshot = false;					///<Should I make a screeshot after frame is done?
		bool  m_makeScreenshotDepth = false;			///<Should I make a screeshot after frame is done?
		uint32_t m_numScreenshot = 0;					///<Screenshot ID		
		

		virtual void onFrameEnded(veEvent event);
		virtual void onFrameStarted(veEvent event);
		virtual bool onKeyboard(veEvent event);
		virtual bool onMouseMove(veEvent event);
		virtual bool onMouseButton(veEvent event);
		virtual bool onMouseScroll(veEvent event);

		virtual void initWorld();
		virtual void initLevel();
		virtual int getDirection(int x, int y, double angle);
		virtual int getOpenWays(int x, int y);
		virtual std::string getMeshName(int x, int y);
		virtual float getMeshRotation(int x, int y);


	public:
		///Constructor
		PacEventListener(std::string name) : VEEventListener(name) { initWorld(); };
		///Destructor
		virtual ~PacEventListener() {};
	};
}

#endif