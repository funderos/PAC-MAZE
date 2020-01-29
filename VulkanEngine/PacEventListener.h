#ifndef PACEVENTLISTENER_H
#define PACEVENTLISTENER_H

namespace ve {
	class PacEventListener : public VEEventListener {
	protected:
		float speed = 50.0f;
		float rageTime = 0.0f;
		float cageTime = 10.0f;
		float playTime = 0.0f;

		int buttonDirection = 0;
		int mode = 1;
		//bool paused = true;
		//bool intro = false;
		//bool gameOver = false;
		//bool startscreen = false;
		int lives = 2;
		int level = 1;
		int pillScore;
		int enemyScore;
		int rageScore = 0;

		virtual void onFrameStarted(veEvent event);
		virtual bool onKeyboard(veEvent event);
		virtual bool onMouseScroll(veEvent event);
		virtual void onDrawOverlay(veEvent event);

		virtual void initWorld();
		virtual void initLevel(bool pillRestore);
		virtual int getDirection(int x, int y, double angle, bool enemy);
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