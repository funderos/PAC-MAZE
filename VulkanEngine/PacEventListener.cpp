#include "VEInclude.h"
#include "math.h"
#include "future"
#include "iostream"
#pragma comment(lib, "Ws2_32.lib")

namespace ve {

	VESceneNode* pills;
	VESceneNode* movable;
	VESceneNode* cat;
	VESceneNode* pCamera;

	const int pgWidth = 17;
	const int pgHeight = 22;
	int buttonDirection = 0;
	int mode = 1;
	unsigned int __stdcall receiveFrameOverUDP(void* socket);

	int lives = 2;
	int level = 1;
	int pillScore;
	int enemyScore;

	enum playground { SPACE, WALL, TELEPORT, CAGE };
	playground grid[pgWidth][pgHeight]
	{
		{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, SPACE, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
		{WALL, SPACE, SPACE, SPACE, WALL, SPACE, SPACE, SPACE, WALL, WALL, WALL, TELEPORT, WALL, WALL, WALL, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, WALL},
		{WALL, SPACE, WALL, SPACE, SPACE, SPACE, WALL, SPACE, WALL, WALL, WALL, SPACE, WALL, WALL, WALL, SPACE, WALL, SPACE, WALL, WALL, SPACE, WALL},
		{WALL, SPACE, WALL, SPACE, WALL, WALL, WALL, SPACE, WALL, WALL, WALL, SPACE, WALL, WALL, WALL, SPACE, WALL, SPACE, WALL, WALL, SPACE, WALL},
		{WALL, SPACE, WALL, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, WALL},
		{WALL, SPACE, WALL, WALL, WALL, SPACE, WALL, SPACE, WALL, WALL, WALL, SPACE, WALL, WALL, WALL, WALL, WALL, SPACE, WALL, WALL, SPACE, WALL},
		{WALL, SPACE, WALL, SPACE, SPACE, SPACE, WALL, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, WALL, SPACE, SPACE, SPACE, WALL, WALL, SPACE, WALL},
		{WALL, SPACE, SPACE, SPACE, WALL, SPACE, SPACE, SPACE, WALL, SPACE, CAGE, CAGE, CAGE, SPACE, SPACE, SPACE, WALL, SPACE, SPACE, SPACE, SPACE, WALL},
		{WALL, SPACE, WALL, WALL, WALL, SPACE, WALL, WALL, WALL, SPACE, CAGE, CAGE, CAGE, SPACE, WALL, WALL, WALL, SPACE, WALL, WALL, WALL, WALL},
		{WALL, SPACE, SPACE, SPACE, WALL, SPACE, SPACE, SPACE, WALL, SPACE, CAGE, CAGE, CAGE, SPACE, SPACE, SPACE, WALL, SPACE, SPACE, SPACE, SPACE, WALL},
		{WALL, SPACE, WALL, SPACE, SPACE, SPACE, WALL, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, WALL, SPACE, SPACE, SPACE, WALL, WALL, SPACE, WALL},
		{WALL, SPACE, WALL, WALL, WALL, SPACE, WALL, SPACE, WALL, WALL, WALL, SPACE, WALL, WALL, WALL, WALL, WALL, SPACE, WALL, WALL, SPACE, WALL},
		{WALL, SPACE, WALL, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, WALL},
		{WALL, SPACE, WALL, SPACE, WALL, WALL, WALL, SPACE, WALL, WALL, WALL, SPACE, WALL, WALL, WALL, SPACE, WALL, SPACE, WALL, WALL, SPACE, WALL},
		{WALL, SPACE, WALL, SPACE, SPACE, SPACE, WALL, SPACE, WALL, WALL, WALL, SPACE, WALL, WALL, WALL, SPACE, WALL, SPACE, WALL, WALL, SPACE, WALL},
		{WALL, SPACE, SPACE, SPACE, WALL, SPACE, SPACE, SPACE, WALL, WALL, WALL, TELEPORT, WALL, WALL, WALL, SPACE, SPACE, SPACE, SPACE, SPACE, SPACE, WALL},
		{WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, SPACE, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL}
	};

	enum collectibles { NONE, PILL, YARN };
	collectibles pellets[pgWidth][pgHeight]
	{
		NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
		NONE, PILL, PILL, PILL, NONE, YARN, PILL, PILL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, PILL, PILL, PILL, PILL, YARN, PILL, NONE,
		NONE, PILL, NONE, PILL, PILL, PILL, NONE, PILL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, PILL, NONE, PILL, NONE, NONE, PILL, NONE,
		NONE, PILL, NONE, PILL, NONE, NONE, NONE, PILL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, PILL, NONE, PILL, NONE, NONE, PILL, NONE,
		NONE, PILL, NONE, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, NONE,
		NONE, PILL, NONE, NONE, NONE, PILL, NONE, PILL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, PILL, NONE, NONE, PILL, NONE,
		NONE, PILL, NONE, PILL, PILL, PILL, NONE, PILL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, PILL, PILL, PILL, NONE, NONE, PILL, NONE,
		NONE, PILL, PILL, PILL, NONE, PILL, PILL, PILL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, PILL, NONE, PILL, PILL, PILL, PILL, NONE,
		NONE, PILL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, PILL, NONE, NONE, NONE, NONE,
		NONE, PILL, PILL, PILL, NONE, PILL, PILL, PILL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, PILL, NONE, PILL, PILL, PILL, PILL, NONE,
		NONE, PILL, NONE, PILL, PILL, PILL, NONE, PILL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, PILL, PILL, PILL, NONE, NONE, PILL, NONE,
		NONE, PILL, NONE, NONE, NONE, PILL, NONE, PILL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, PILL, NONE, NONE, PILL, NONE,
		NONE, PILL, NONE, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, PILL, NONE,
		NONE, PILL, NONE, PILL, NONE, NONE, NONE, PILL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, PILL, NONE, PILL, NONE, NONE, PILL, NONE,
		NONE, PILL, NONE, PILL, PILL, PILL, NONE, PILL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, PILL, NONE, PILL, NONE, NONE, PILL, NONE,
		NONE, PILL, PILL, PILL, NONE, YARN, PILL, PILL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, PILL, PILL, PILL, PILL, YARN, PILL, NONE,
		NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE
	};

	int powerPelletX[]{ 1, 1, 15, 15 };
	int powerPelletY[]{ 1, 20, 1, 20 };
	int fillX[2]{ 0, pgWidth - 1 };
	int fillY[2]{ 0, pgHeight - 1 };

	double enDir[5];
	double cameraAngle;
	enum headingDirection { STRAIGHT = 0, LEFT = -1, RIGHT = 1, STOP = -2, JUMP = 2 };
	headingDirection headDir[5];

	void PacEventListener::initWorld() {
		pillScore = 0;
		enemyScore = 0;
		lives = 2;
		level = 1;

		VESceneNode* pScene = getSceneManagerPointer()->getSceneNode("Level 1");
		pCamera = getSceneManagerPointer()->getCamera();

		VESceneNode* maze = getSceneManagerPointer()->createSceneNode("The Grid Parent", pScene, glm::mat4(1.0));
		maze->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)));
		maze->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f)));

		pills = getSceneManagerPointer()->createSceneNode("The Pills Parent", pScene, glm::mat4(1.0));
		pills->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)));
		pills->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f)));

		movable = getSceneManagerPointer()->createSceneNode("The Movable Parent", pScene, glm::mat4(1.0));
		movable->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)));
		movable->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f)));

		for (int x = 1; x < pgWidth - 1; x++) {
			for (int y = 1; y < pgHeight - 1; y++) {
				std::string mesh = getMeshName(x, y);
				if (mesh == "")
					continue;
				VESceneNode* tile;
				VECHECKPOINTER(tile = getSceneManagerPointer()->loadModel("Tile-" + std::to_string(x) + "-"
					+ std::to_string(y), "media/models/pacmaze/static", mesh));
				tile->multiplyTransform(glm::rotate(getMeshRotation(x, y), glm::vec3(0.0f, 1.0f, 0.0f)));
				tile->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(x * 32.0f, 0.0f, y * 32.0f)));
				maze->addChild(tile);
			}
		}

		for (int x : fillX) {
			for (int y = fillY[0]; y <= fillY[1]; y++) {
				VESceneNode* tile;
				if (y == 11) {
					VECHECKPOINTER(tile = getSceneManagerPointer()->loadModel("Tile-" + std::to_string(x) + "-" + std::to_string(y),
						"media/models/pacmaze/static", "block_straight.obj"));
					tile->multiplyTransform(glm::rotate((float)M_PI / 2, glm::vec3(0.0f, 1.0f, 0.0f)));
				}
				else {
					VECHECKPOINTER(tile = getSceneManagerPointer()->loadModel("Tile-" + std::to_string(x) + "-" + std::to_string(y),
						"media/models/pacmaze/static", "block_wall.obj"));
				}

				tile->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(x * 32.0f, 0.0f, y * 32.0f)));
				maze->addChild(tile);
			}
		}

		for (int y : fillY) {
			for (int x = 1; x <= pgWidth - 2; x++) {
				VESceneNode* tile;
				VECHECKPOINTER(tile = getSceneManagerPointer()->loadModel("Tile-" + std::to_string(x) + "-" + std::to_string(y),
					"media/models/pacmaze/static", "block_wall.obj"));
				tile->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(x * 32.0f, 0.0f, y * 32.0f)));
				maze->addChild(tile);
			}
		}

		VECHECKPOINTER(cat = getSceneManagerPointer()->loadModel("Cat", "media/models/pacmaze/characters", "Mimi.obj"));
		movable->addChild(cat);

		VESceneNode* pinky;
		VECHECKPOINTER(pinky = getSceneManagerPointer()->loadModel("Pinky", "media/models/pacmaze/characters", "pinky.obj"));
		movable->addChild(pinky);

		VESceneNode* pinkyWhite;
		VECHECKPOINTER(pinkyWhite = getSceneManagerPointer()->loadModel("PinkyWhite", "media/models/pacmaze/characters", "white.obj"));
		//pinkyWhite->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f)));
		pinkyWhite->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 200.0f, 0.0f)));
		pinky->addChild(pinkyWhite);

		VESceneNode* pinkyBlue;
		VECHECKPOINTER(pinkyBlue = getSceneManagerPointer()->loadModel("PinkyBlue", "media/models/pacmaze/characters", "blue.obj"));
		//pinkyBlue->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f)));
		pinkyBlue->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -200.0f, 0.0f)));
		pinky->addChild(pinkyBlue);

		VESceneNode* blinky;
		VECHECKPOINTER(blinky = getSceneManagerPointer()->loadModel("Blinky", "media/models/pacmaze/characters", "blinky.obj"));
		movable->addChild(blinky);

		VESceneNode* blinkyWhite;
		VECHECKPOINTER(blinkyWhite = getSceneManagerPointer()->loadModel("blinkyWhite", "media/models/pacmaze/characters", "white.obj"));
		blinkyWhite->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 200.0f, 0.0f)));
		blinky->addChild(blinkyWhite);

		VESceneNode* blinkyBlue;
		VECHECKPOINTER(blinkyBlue = getSceneManagerPointer()->loadModel("blinkyBlue", "media/models/pacmaze/characters", "blue.obj"));
		blinkyBlue->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -200.0f, 0.0f)));
		blinky->addChild(blinkyBlue);

		VESceneNode* inky;
		VECHECKPOINTER(inky = getSceneManagerPointer()->loadModel("Inky", "media/models/pacmaze/characters", "inky.obj"));
		movable->addChild(inky);

		VESceneNode* inkyWhite;
		VECHECKPOINTER(inkyWhite = getSceneManagerPointer()->loadModel("inkyWhite", "media/models/pacmaze/characters", "white.obj"));
		inkyWhite->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 200.0f, 0.0f)));
		inky->addChild(inkyWhite);

		VESceneNode* inkyBlue;
		VECHECKPOINTER(inkyBlue = getSceneManagerPointer()->loadModel("inkyBlue", "media/models/pacmaze/characters", "blue.obj"));
		inkyBlue->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -200.0f, 0.0f)));
		inky->addChild(inkyBlue);

		VESceneNode* clyde;
		VECHECKPOINTER(clyde = getSceneManagerPointer()->loadModel("Clyde", "media/models/pacmaze/characters", "clyde.obj"));
		movable->addChild(clyde);

		VESceneNode* clydeWhite;
		VECHECKPOINTER(clydeWhite = getSceneManagerPointer()->loadModel("clydeWhite", "media/models/pacmaze/characters", "white.obj"));
		clydeWhite->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 200.0f, 0.0f)));
		clyde->addChild(clydeWhite);

		VESceneNode* clydeBlue;
		VECHECKPOINTER(clydeBlue = getSceneManagerPointer()->loadModel("clydeBlue", "media/models/pacmaze/characters", "blue.obj"));
		clydeBlue->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -200.0f, 0.0f)));
		clyde->addChild(clydeBlue);

		for (int x = 1; x < pgWidth - 1; x++) {
			for (int y = 1; y < pgHeight - 1; y++) {
				if (pellets[x][y] > 0) {
					if (pellets[x][y + 1] > 0) {
						VESceneNode* pill;
						VECHECKPOINTER(pill = getSceneManagerPointer()->loadModel("Pill-" + std::to_string(x) + "-"
							+ std::to_string(y) + "Y", "media/models/pacmaze/elements", "pill.obj"));
						//pill->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
						pill->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(x * 32.0f, 1.5f, y * 32.0f + 16.0f)));
						pills->addChild(pill);
					}
					if (pellets[x + 1][y] > 0) {
						VESceneNode* pill;
						VECHECKPOINTER(pill = getSceneManagerPointer()->loadModel("Pill-" + std::to_string(x) + "-"
							+ std::to_string(y) + "X", "media/models/pacmaze/elements", "pill.obj"));
						//pill->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
						pill->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(x * 32.0f + 16.0f, 1.5f, y * 32.0f)));
						pills->addChild(pill);
					}
					std::string mesh;
					std::string name;

					if (pellets[x][y] == 1) {
						name = "Pill-";
						mesh = "pill.obj";
					}
					else {
						name = "Yarn-";
						mesh = "yarn.obj";
					}

					VESceneNode* pill;
					VECHECKPOINTER(pill = getSceneManagerPointer()->loadModel(name + std::to_string(x) + "-"
						+ std::to_string(y), "media/models/pacmaze/elements", mesh));
					//pill->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
					pill->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(x * 32.0f, 1.5f, y * 32.0f)));
					pills->addChild(pill);
				}
			}
		}

		mode = 2;

		encodeStart();
	}

	void initLevel(bool pillRestore) {

		while (getEnginePointer()->m_irrklangEngine->isCurrentlyPlaying("media/sounds/pacmaze/death_1.wav")) {};
		double angle = 0;
		int i = 0;

		
		//pCamera->setTransform(glm::rotate((float)M_PI / 2, glm::vec3(0.0f, 1.0f, 0.0f)));
		pCamera->setTransform(glm::rotate((float)M_PI / 6, glm::vec3(1.0f, 0.0f, 0.0f)));
		pCamera->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 7.0f, -8.0f)));
		//pCamera->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 100.0f, 0.0f)));


		for (VESceneNode* mv : movable->getChildrenList()) {
			headDir[i] = STRAIGHT;
			if (i) {
				enDir[i] = angle;
				mv->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f)));
				mv->multiplyTransform(glm::rotate((float)enDir[i], glm::vec3(0.0f, 1.0f, 0.0f)));
				mv->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(8 * 32.0f, 0.0f, 11 * 32.0f)));
			}
			else {
				enDir[i] = M_PI / 2;
				mv->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(8.0f, 8.0f, 8.0f)));
				mv->multiplyTransform(glm::rotate((float)enDir[i], glm::vec3(0.0f, 1.0f, 0.0f)));
				mv->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(8 * 32.0f, 0.5f, 5 * 32.0f)));
			}
			angle = angle + M_PI / 2; i++;
		}
		if (pillRestore) {
			for (VESceneNode* pill : pills->getChildrenList()) {
				glm::vec3 pos = pill->getPosition();
				pill->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
				pill->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, 1.5f, pos.z)));
			}
		}
		
		cameraAngle = 0;
		mode = -2;
	}

	bool PacEventListener::onKeyboard(veEvent event) {
		if (event.idata1 == GLFW_KEY_ESCAPE) {				//ESC pressed - end the engine
			getEnginePointer()->end();
			return true;
		}
		if (event.idata3 == GLFW_RELEASE) {
			buttonDirection = 0;
		}

		if (event.idata1 == GLFW_KEY_LEFT && event.idata3 == GLFW_PRESS) {
			buttonDirection = -1;
			return false;
		}
		if (event.idata1 == GLFW_KEY_RIGHT && event.idata3 == GLFW_PRESS) {
			buttonDirection = 1;
			return false;
		}
		if (event.idata1 == GLFW_KEY_SPACE && event.idata3 == GLFW_PRESS) {
			if (mode == 2) {
				cat->addChild(pCamera);
				initLevel(true);
				return false;
			}

			if (mode == -1) {
				lives = 2;
				enemyScore = 0;
				pillScore = 0;
				level = 1;
				initLevel(true);
				return false;
			}

		}
	}

	/**
	*
	* \brief React to mouse scroll events
	*
	* They are the same as key W/S
	*
	* \param[in] event The mouse scroll event
	* \returns false, so event is not consumed
	*
	*/
	bool PacEventListener::onMouseScroll(veEvent event) {

		float xoffset = event.fdata1;
		float yoffset = event.fdata2;

		VECamera* pCamera = getSceneManagerPointer()->getCamera();
		VESceneNode* pParent = pCamera->getParent();
		glm::vec4 translate = 1000 * yoffset * glm::vec4(0.0, 0.0, 1.0, 1.0);

		if (pParent == nullptr) {
			pParent = pCamera;
		}
		else {
			//so far the translation vector was defined in cam local space. But the camera frame of reference 
			//is defined wrt its parent space, so we must transform this vector to parent space
			translate = pCamera->getTransform() * translate;	//transform from local camera space to parent space
		}

		//add the new translation vector to the previous one
		glm::vec3 trans = glm::vec3(translate.x, translate.y, translate.z);
		pParent->setTransform(glm::translate(pParent->getTransform(), (float)event.dt * trans));

		return false;
	}

	void PacEventListener::onFrameStarted(veEvent event) {
		switch (mode) {
		case -2:
			getEnginePointer()->m_irrklangEngine->play2D("media/sounds/pacmaze/game_start.wav", false);
			mode = 1;
			return;
		case -1:
			if (getEnginePointer()->m_irrklangEngine->isCurrentlyPlaying("media/sounds/pacmaze/game_start.wav")) {
				while (getEnginePointer()->m_irrklangEngine->isCurrentlyPlaying("media/sounds/pacmaze/game_start.wav")) {}
				getEnginePointer()->m_irrklangEngine->play2D("media/sounds/pacmaze/death_2.wav", true);
			}
			return;
		case 0: break;
		case 1:
			if (!getEnginePointer()->m_irrklangEngine->isCurrentlyPlaying("media/sounds/pacmaze/game_start.wav"))
			{
				pCamera->setTransform(glm::rotate((float)M_PI / 6, glm::vec3(1.0f, 0.0f, 0.0f)));
				pCamera->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 7.0f, -8.0f)));
				std::string name = "media/sounds/pacmaze/siren_" + std::to_string(std::min(level, 5)) + ".wav";
				char char_array[33];
				strcpy(char_array, name.c_str());
				getEnginePointer()->m_irrklangEngine->play2D(char_array, true);
				mode = 0;
			}
			else 
			{
				if (cameraAngle < 2 * M_PI) {
					cameraAngle = cameraAngle + event.dt * 1.5;
					pCamera->multiplyTransform(glm::rotate((float)(event.dt * 1.5), glm::vec3(0.0, 1.0, 0.0)));
				}
				else {
					pCamera->setTransform(glm::rotate((float)M_PI / 6, glm::vec3(1.0f, 0.0f, 0.0f)));
					pCamera->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 7.0f, -8.0f)));
				}
				
			}
			return;
		default: return;
		}
		int i = 0;
		int rage = 0;

		if (rageTime > 0) {
			rage = 1;
			if (rageTime < 3) {
				rage = (int)(rageTime * 3) % 2;
				if (!rage)
					rage--;
			}
			rageTime -= event.dt;
			if (rageTime <= 0) {
				getEnginePointer()->m_irrklangEngine->removeAllSoundSources();
				std::string name = "media/sounds/pacmaze/siren_" + std::to_string(std::min(level, 5)) + ".wav";
				char char_array[33];
				strcpy(char_array, name.c_str());
				getEnginePointer()->m_irrklangEngine->play2D(char_array, true);
			}
		}

		if (cageTime > 0) {
			cageTime -= event.dt;
		}


		for (VESceneNode* mv : movable->getChildrenList()) {
			glm::vec3 curPos = mv->getPosition();

			double factor = speed * event.dt;
			if (!i && rageTime > 0)
				factor = factor * 1.25;
			bool decision = false;

			if (headDir[i] == STRAIGHT) {
				if ((int)(curPos.x + 16) / 32 != (int)(curPos.x + factor * sin(enDir[i]) + 16) / 32) {
					float x = curPos.x;
					curPos.x = ((int)(curPos.x + 16) / 32 + .5 * sin(enDir[i])) * 32;
					factor = x - curPos.x;
					decision = true;
				}
				else if ((int)(curPos.z + 16) / 32 != (int)(curPos.z + factor * cos(enDir[i]) + 16) / 32) {
					float z = curPos.z;
					curPos.z = ((int)(curPos.z + 16) / 32 + .5 * cos(enDir[i])) * 32;
					factor = z - curPos.z;
					decision = true;
				}
			}
			else if (headDir[i] != STOP) {
				if (cos(enDir[i] + factor * headDir[i] / 16) * cos(enDir[i]) < 0
					|| sin(enDir[i] + factor * headDir[i] / 16) * sin(enDir[i]) < 0) {
					double roundedAngle;

					if (cos(enDir[i]) > 0.72) {
						curPos.z = (((int)(curPos.z + 8) / 32) + .5) * 32;
						curPos.x = ((int)(curPos.x + 16) / 32) * 32;
						roundedAngle = 0;
					}
					if (cos(enDir[i]) < -0.72) {
						curPos.z = (((int)(curPos.z + 24) / 32) - .5) * 32;
						curPos.x = ((int)(curPos.x + 16) / 32) * 32;
						roundedAngle = M_PI;
					}
					if (sin(enDir[i]) > 0.72) {
						curPos.x = (((int)(curPos.x + 8) / 32) + .5) * 32;
						curPos.z = ((int)(curPos.z + 16) / 32) * 32;
						roundedAngle = M_PI / 2;
					}
					if (sin(enDir[i]) < -0.72) {
						curPos.x = (((int)(curPos.x + 24) / 32) - .5) * 32;
						curPos.z = ((int)(curPos.z + 16) / 32) * 32;
						roundedAngle = M_PI * 3 / 2;
					}

					while (enDir[i] > 2 * M_PI)
						enDir[i] = enDir[i] - 2 * M_PI;

					while (enDir[i] < -2 * M_PI)
						enDir[i] = enDir[i] + 2 * M_PI;

					factor = (enDir[i] - roundedAngle) / 16;
					enDir[i] = roundedAngle;
					decision = true;
				}
				else {
					enDir[i] = enDir[i] + factor * headDir[i] / 16;
				}
			}
			else {
				decision = true;
			}

			if (decision) {
				//if (i)
				//	std::cout << i << ": " << curPos.x / 32.0f << ", " << curPos.y / 32.0f << ", "
				//	<< curPos.z / 32.0f << ", " << enDir[i] << ", " << headDir[i] << std::endl;
				if (factor < 0)
					factor = factor * -1;
				if (factor == 0)
					factor = 0.001;

				headDir[i] = headingDirection(getDirection(curPos.x + 0.5, curPos.z + 0.5, enDir[i], i));
				switch (headDir[i]) {
				case STOP: factor = 0; break;
				case JUMP: curPos.x = curPos.x - 13 * 32 * sin(enDir[i]); headDir[i] = STRAIGHT; break;
				}
				enDir[i] = enDir[i] + factor * headDir[i] / 16;
				//if (i)
				//	std::cout << i << ": " << curPos.x / 32.0f << ", " << curPos.y / 32.0f << ", "
				//	<< curPos.z / 32.0f << ", " << enDir[i] << ", " << headDir[i] << std::endl;

			}

			curPos.x = curPos.x + factor * sin(enDir[i]);
			curPos.z = curPos.z + factor * cos(enDir[i]);



			if (i) { // enemies
				curPos.y = 160.0f * rage;
				mv->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f)));
				mv->multiplyTransform(glm::rotate((float)enDir[i], glm::vec3(0.0f, 1.0f, 0.0f)));
				mv->multiplyTransform(glm::translate(glm::mat4(1.0f), curPos));

				glm::vec3 catPos = cat->getPosition();

				if (std::abs(catPos.x - curPos.x) < 12 && std::abs(catPos.z - curPos.z) < 12) {
					if (rageTime > 0) {
						enDir[i] = M_PI;
						headDir[i] = LEFT;
						mv->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f)));
						mv->multiplyTransform(glm::rotate((float)enDir[i], glm::vec3(0.0f, 1.0f, 0.0f)));
						mv->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(8 * 32.0f, 0.0f, 11 * 32.0f)));
						enemyScore = enemyScore + rageScore;
						rageScore = rageScore * 2;
						getEnginePointer()->m_irrklangEngine->play2D("media/sounds/pacmaze/eat_ghost.wav", false);

					}
					else {
						getEnginePointer()->m_irrklangEngine->removeAllSoundSources();
						getEnginePointer()->m_irrklangEngine->play2D("media/sounds/pacmaze/death_1.wav", false);
						if (lives == 0) { // Game Over
							mode = -1;
						}
						else {
							lives--;
							initLevel(false);
						}
					}
				}
			}
			else { // cat
				mv->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(8.0f, 8.0f, 8.0f)));
				mv->multiplyTransform(glm::rotate((float)enDir[i], glm::vec3(0.0f, 1.0f, 0.0f)));
				mv->multiplyTransform(glm::translate(glm::mat4(1.0f), curPos));

				for (VESceneNode* pill : pills->getChildrenList()) {
					glm::vec3 pillPos = pill->getPosition();
					if (std::abs(pillPos.x - curPos.x) < 11 && std::abs(pillPos.z - curPos.z) < 11
						&& pillPos.y > 0) {
						if (pill->getName().rfind("Yarn", 0) == 0) {
							if (rageTime <= 0) {
								rageScore = 200;
							}
							rageTime = 1000.0f / speed;
							pillScore = pillScore + 40;
							for (int j = 1; j < 5; j++) {
								if (enDir[j] < M_PI)
									enDir[j] = enDir[j] + M_PI;
								else
									enDir[j] = enDir[j] - M_PI;
								headDir[j] = headingDirection(headDir[j] * -1);
								getEnginePointer()->m_irrklangEngine->removeAllSoundSources();
								getEnginePointer()->m_irrklangEngine->play2D("media/sounds/pacmaze/power_pellet.wav", true);
							}
						}
						getEnginePointer()->m_irrklangEngine->play2D("media/sounds/pacmaze/munch.wav", false);
						pillScore = pillScore + 10;
						if (pillScore % 3120 == 0) {
							level++;
							getEnginePointer()->m_irrklangEngine->removeAllSoundSources();
							initLevel(true);
						}
						pill->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -10.0f, 0.0f)));
						break;

					}
				}
			}
			i++;
		}
	}

	void PacEventListener::onFrameEnded(veEvent event) {
		timeCache += event.dt;

		if (timeCache < 1.0f / 30.0f) { //ensure framerate is under 30fps
			return;
		}

		timeCache = 0;
		framenumber++;

		VkExtent2D extent = getWindowPointer()->getExtent();
		uint32_t imageSize = extent.width * extent.height * 4;
		VkImage image = getRendererPointer()->getSwapChainImage();

		uint8_t* dataImage = new uint8_t[imageSize];

		vh::vhBufCopySwapChainImageToHost(getRendererPointer()->getDevice(),
			getRendererPointer()->getVmaAllocator(),
			getRendererPointer()->getGraphicsQueue(),
			getRendererPointer()->getCommandPool(),
			image, VK_FORMAT_R8G8B8A8_UNORM,
			VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			dataImage, extent.width, extent.height, imageSize);

		ret = av_frame_make_writable(picture);
		if (ret < 0) {
			fprintf(stderr, "Cannot make frame writeable\n");
			exit(1);
		}

		uint8_t* inData[1] = { dataImage };
		int      inLinesize[1] = { 4 * c->width };
		sws_scale(ctx, inData, inLinesize, 0, c->height,
			picture->data, picture->linesize);

		//picture->pts = framenumber++;

		// encode the image
		encode(c, picture, pkt);

		delete[] dataImage;
	}

	void PacEventListener::onDrawOverlay(veEvent event) {
		VESubrenderFW_Nuklear* pSubrender = (VESubrenderFW_Nuklear*)getRendererPointer()->getOverlay();
		if (pSubrender == nullptr) return;

		struct nk_context* ctx = pSubrender->getContext();

		VkExtent2D extent = getEnginePointer()->getWindow()->getExtent();

		switch (mode) {

		case 0:
			if (nk_begin(ctx, "", nk_rect(extent.width - 200, 0, 200, 122), NK_WINDOW_BORDER)) {
				char outbuffer[100];
				nk_layout_row_dynamic(ctx, 30, 1);
				sprintf(outbuffer, "Score: %d", pillScore + enemyScore);
				nk_label(ctx, outbuffer, NK_TEXT_RIGHT);

				nk_layout_row_dynamic(ctx, 30, 1);
				sprintf(outbuffer, "Lives: %d", lives);
				nk_label(ctx, outbuffer, NK_TEXT_RIGHT);

				nk_layout_row_dynamic(ctx, 30, 1);
				sprintf(outbuffer, "Level: %d", level);
				nk_label(ctx, outbuffer, NK_TEXT_RIGHT);
			}
			break;
		case 2:
			if (nk_begin(ctx, "", nk_rect(extent.width / 2 - 250, extent.height * 3 / 4 - 30, 500, 110), NK_WINDOW_BORDER)) {
				char outbuffer[100];
				nk_layout_row_dynamic(ctx, 80, 1);
				sprintf(outbuffer, "Press SPACE to start playing!");
				nk_label(ctx, outbuffer, NK_TEXT_CENTERED);
			}
			break;
		case -1:
			if (nk_begin(ctx, "", nk_rect(extent.width / 2 - 150, extent.height / 2 - 110, 300, 220), NK_WINDOW_BORDER)) {
				nk_layout_row_dynamic(ctx, 60, 1);
				nk_label(ctx, "Game Over", NK_TEXT_CENTERED);
				char outbuffer[100];
				sprintf(outbuffer, "Final Score: %d", pillScore + enemyScore);
				nk_label(ctx, outbuffer, NK_TEXT_CENTERED);
				sprintf(outbuffer, "Press SPACE to restart!");
				nk_label(ctx, outbuffer, NK_TEXT_CENTERED);
				
				/*
				if (nk_button_label(ctx, "Restart")) {
					lives = 2;
					enemyScore = 0;
					pillScore = 0;
					level = 1;
					initLevel(true);
				}
				*/
			}
		}

		nk_end(ctx);
	}

	int PacEventListener::getDirection(int x, int y, double angle, bool enemy) {

		bool dir[]{ false, false, false };
		glm::vec3 catPos = cat->getPosition();
		float lengths[3]{ 100.0f };

		if (x % 32 == 0) {
			if (angle == 0 || angle == 2 * M_PI) {
				if (grid[x / 32][(y + 16) / 32] == CAGE) {
					if (grid[x / 32 - 1][(y + 16) / 32] == CAGE)
						dir[0] = true;
					if (grid[x / 32 + 1][(y + 16) / 32] == CAGE)
						dir[2] = true;
					if (grid[x / 32][(y + 48) / 32] == CAGE)
						dir[1] = true;
					if ((dir[0] && dir[1] && grid[x / 32][(y + 48) / 32] == SPACE && cageTime <= 0)) {
						cageTime = 500 / speed;
						return STRAIGHT;
					}

				}
				else {
					if (grid[x / 32 - 1][(y + 16) / 32] == SPACE && (x / 32 - 3 < 0
						|| grid[x / 32 - 3][(y + 16) / 32] != TELEPORT || !enemy)) {
						dir[0] = true;
						lengths[0] = glm::length(catPos - glm::vec3(x - 32, 0, y + 16));
					}

					if (grid[x / 32 + 1][(y + 16) / 32] == SPACE && (x / 32 + 3 > pgWidth
						|| grid[x / 32 + 3][(y + 16) / 32] != TELEPORT || !enemy)) {
						dir[2] = true;
						lengths[2] = glm::length(catPos - glm::vec3(x + 32, 0, y + 16));
					}

					if (grid[x / 32][(y + 48) / 32] == SPACE) {
						dir[1] = true;
						lengths[1] = glm::length(catPos - glm::vec3(x, 0, y + 48));
					}

				}
			}
			else {
				if (grid[x / 32][(y - 16) / 32] == CAGE) {
					if (grid[x / 32 - 1][(y - 16) / 32] == CAGE)
						dir[2] = true;
					if (grid[x / 32 + 1][(y - 16) / 32] == CAGE)
						dir[0] = true;
					if (grid[x / 32][(y - 48) / 32] == CAGE)
						dir[1] = true;
				}
				else {
					if (grid[x / 32 - 1][(y - 16) / 32] == SPACE && (x / 32 - 3 < 0
						|| grid[x / 32 - 3][(y - 16) / 32] != TELEPORT || !enemy)) {
						dir[2] = true;
						lengths[2] = glm::length(catPos - glm::vec3(x - 32, 0, y - 16));
					}

					if (grid[x / 32 + 1][(y - 16) / 32] == SPACE && (x / 32 + 3 > pgWidth - 1
						|| grid[x / 32 + 3][(y - 16) / 32] != TELEPORT || !enemy)) {
						dir[0] = true;
						lengths[0] = glm::length(catPos - glm::vec3(x + 32, 0, y - 16));
					}

					if (grid[x / 32][(y - 48) / 32] == SPACE) {
						dir[1] = true;
						lengths[1] = glm::length(catPos - glm::vec3(x, 0, y - 48));
					}

				}
			}
		}
		else {
			if (angle == M_PI / 2) {
				if (grid[(x + 16) / 32][y / 32] == CAGE) {
					if (grid[(x + 16) / 32][y / 32 - 1] == CAGE)
						dir[2] = true;
					if (grid[(x + 48) / 32][y / 32] == CAGE)
						dir[1] = true;
					if (grid[(x + 16) / 32][y / 32 + 1] == CAGE)
						dir[0] = true;
					if ((dir[2] && dir[1] && grid[(x + 16) / 32][y / 32 + 1] == SPACE && cageTime <= 0)) {
						cageTime = 500 / speed;
						return LEFT;
					}
				}
				else if (!enemy && grid[(x + 16) / 32][y / 32] == TELEPORT) {
					return 2;
				}
				else {
					if (grid[(x + 16) / 32][y / 32 + 1] == SPACE) {
						dir[0] = true;
						lengths[0] = glm::length(catPos - glm::vec3(x + 16, 0, y + 32));
					}

					if (grid[(x + 16) / 32][y / 32 - 1] == SPACE) {
						dir[2] = true;
						lengths[2] = glm::length(catPos - glm::vec3(x + 16, 0, y - 32));
					}

					if (grid[(x + 48) / 32][y / 32] == TELEPORT || (grid[(x + 48) / 32][y / 32] == SPACE
						&& ((x + 112) / 32 > pgWidth - 1 || grid[(x + 112) / 32][y / 32] != TELEPORT || !enemy))) {
						dir[1] = true;
						lengths[1] = glm::length(catPos - glm::vec3(x + 48, 0, y));
					}

				}
			}
			else {
				if (grid[(x - 16) / 32][y / 32] == CAGE) {
					if (grid[(x - 16) / 32][y / 32 - 1] == CAGE)
						dir[0] = true;
					if (grid[(x - 48) / 32][y / 32] == CAGE)
						dir[1] = true;
					if (grid[(x - 16) / 32][y / 32 + 1] == CAGE)
						dir[2] = true;
					if ((dir[0] && dir[1] && grid[(x - 16) / 32][y / 32 + 1] == SPACE && cageTime <= 0)) {
						cageTime = 500 / speed;
						return RIGHT;
					}
				}
				else if (!enemy && grid[(x - 16) / 32][y / 32] == TELEPORT) {
					return 2;
				}
				else {
					if (grid[(x - 16) / 32][y / 32 + 1] == SPACE) {
						dir[2] = true;
						lengths[2] = glm::length(catPos - glm::vec3(x - 16, 0, y + 32));
					}

					if (grid[(x - 16) / 32][y / 32 - 1] == SPACE) {
						dir[0] = true;
						lengths[0] = glm::length(catPos - glm::vec3(x - 16, 0, y - 32));
					}

					if (grid[(x - 48) / 32][y / 32] == TELEPORT || grid[(x - 48) / 32][y / 32] == SPACE
						&& ((x - 112) / 32 < 0 || grid[(x - 112) / 32][y / 32] != TELEPORT || !enemy)) {
						dir[1] = true;
						lengths[1] = glm::length(catPos - glm::vec3(x - 48, 0, y));
					}

				}

			}
		}

		if (!enemy) {
			if (buttonDirection == -1 && dir[0])
				return -1;
			if (buttonDirection == 1 && dir[2])
				return 1;
			if (dir[1])
				return 0;
			if (dir[0] == dir[2])
				return -2;
		}

		if (enemy && rand() % 100 < std::min(level * 10 + 23, 66)) {
			int index = 0;
			if (dir[1] && lengths[0] > lengths[1])
				index = 1;
			if (dir[2] && lengths[index] > lengths[2])
				index = 2;
			if (dir[index])
				return index - 1;
			if (dir[2] && lengths[1] > lengths[2])
				return 1;
			if (dir[1])
				return 0;
			return 1;
		}
		
		int d = rand() % 3;

		while (!dir[d])
			d = (d + 1) % 3;
		return d - 1;
	}

	int PacEventListener::getOpenWays(int x, int y) {
		int i = 0;
		boolean straight = true;
		if (grid[x - 1][y] == SPACE || grid[x - 1][y] == TELEPORT) {
			straight = !straight;
			i++;
		}

		if (grid[x + 1][y] == SPACE || grid[x + 1][y] == TELEPORT) {
			straight = !straight;
			i++;
		}

		if (grid[x][y - 1] == SPACE || grid[x][y - 1] == TELEPORT
			|| (grid[x][y - 1] == CAGE && grid[x - 1][y - 1] == CAGE && grid[x + 1][y - 1] == CAGE))
			i++;

		if (grid[x][y + 1] == SPACE || grid[x][y + 1] == TELEPORT)
			i++;

		if (i < 2)
			return -1;

		if (i == 2 && straight)
			return 1;

		return i;
	}

	std::string PacEventListener::getMeshName(int x, int y) {
		switch (grid[x][y]) {
		case SPACE:
			switch (getOpenWays(x, y)) {
			case 1:
				return "block_straight.obj";
			case 2:
				return "block_curve.obj";
			case 3:
				return "block_t.obj";
			case 4:
				return "block_cross.obj";
			default:
				return "";
			}
		case WALL:
			return "block_wall.obj";
		case CAGE:
			if (grid[x - 1][y] == CAGE && grid[x + 1][y] == CAGE
				&& grid[x][y - 1] == CAGE && grid[x][y + 1] == CAGE)
				return "block_cage.obj";
			else
				return "";
		case TELEPORT:
			return "block_straight.obj";
		default:
			return "";
		}
	}

	float PacEventListener::getMeshRotation(int x, int y) {
		switch (getOpenWays(x, y)) {
		case 1:
			if (grid[x - 1][y] == SPACE || grid[x - 1][y] == TELEPORT)
				return M_PI / 2;
			else
				return 0;
		case 2:
			if (grid[x][y + 1] == SPACE || grid[x][y + 1] == TELEPORT) {
				if (grid[x - 1][y] == SPACE || grid[x - 1][y] == TELEPORT)
					return 0;
				else
					return M_PI / 2;
			}
			else {
				if (grid[x - 1][y] == SPACE || grid[x - 1][y] == TELEPORT)
					return M_PI * 3 / 2;
				else
					return M_PI;
			}
		case 3:
			if (grid[x + 1][y] == WALL || grid[x + 1][y] == CAGE)
				return 0;
			if (grid[x - 1][y] == WALL || grid[x - 1][y] == CAGE)
				return M_PI;
			if (grid[x][y + 1] == WALL || grid[x][y + 1] == CAGE)
				return M_PI * 3 / 2;
			if (grid[x][y - 1] == WALL || grid[x][y - 1] == CAGE)
				return M_PI / 2;
		default:
			return 0;
		}
	}

	// Network Config

	struct Header {
		unsigned long frameNumber;
		unsigned long fragmentNumber;
		unsigned long totalFragments;
	};

	void PacEventListener::encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt)
	{
		// send the frame to the encoder */
		ret = avcodec_send_frame(enc_ctx, frame);
		if (ret < 0) {
			fprintf(stderr, "error sending a frame for encoding\n");
			exit(1);
		}

		while (ret >= 0) {
			int ret = avcodec_receive_packet(enc_ctx, pkt);
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
				return;
			else if (ret < 0) {
				fprintf(stderr, "error during encoding\n");
				exit(1);
			}
			sendFrameOverUDP((char*)pkt->data, pkt->size);
			// fwrite(pkt->data, 1, pkt->size, outfile);
			av_packet_unref(pkt);
		}
	};

	unsigned int __stdcall receiveFrameOverUDP(void* socket) {
		SOCKET* client = (SOCKET*)socket;
		SOCKET Client = *client;
		long rc;
		enum Command { SPACE, ESC, LEFT, RIGHT, RELEASED, OTHER };
		Command rcCommand;

		while (rc != SOCKET_ERROR)
		{
			rc = recv(Client, (char*)&rcCommand, sizeof(rcCommand), 0);
			if (rc == 0)
			{
				printf("Server hat die Verbindung getrennt..\n");
				break;
			}
			if (rc == SOCKET_ERROR)
			{
				printf("Fehler: recv, fehler code: %d\n", WSAGetLastError());
				break;
			}

			switch (rcCommand) {
			case ESC: getEnginePointer()->end(); break;
			case RELEASED: buttonDirection = 0; break;
			case LEFT: buttonDirection = -1; break;
			case RIGHT: buttonDirection = 1; break;
			case SPACE:
				if (mode == 2) {
					cat->addChild(pCamera);
					initLevel(true);
					break;
				}

				if (mode == -1) {
					lives = 2;
					enemyScore = 0;
					pillScore = 0;
					level = 1;
					initLevel(true);
					break;
				}
			default: break;
			}
		}
		return 0;
	}

	void PacEventListener::sendFrameOverUDP(char* data, int size) {
		long rc;
		const int maxSize = 1400;
		char sendbuffer[65000];

		//printf("Sending frame %4d (size=%5d)\n", framenumber, size);

		Header header;
		header.frameNumber = framenumber;
		header.fragmentNumber = 1;
		header.totalFragments = size / maxSize + int(size % maxSize > 0);

		rc = 0;
		int bytes = 0;
		for (int i = 0; i < header.totalFragments; i++) {
			memcpy(sendbuffer, &header, sizeof(header));
			memcpy(sendbuffer + sizeof(header), data + bytes, std::min(maxSize, size - bytes));

			rc = sendto(udp_sock, sendbuffer, std::min(maxSize, size - bytes) + sizeof(header), 0, (SOCKADDR*)&udp_addr, sizeof(SOCKADDR_IN));

			if (rc == SOCKET_ERROR) {
				printf("Send UDP Fragment Error, Code: %d\n", WSAGetLastError());
				exit(1);
			}
			else {
				rc = rc - sizeof(header);
			}
			bytes += rc;
			header.fragmentNumber++;
		}
	}


	void PacEventListener::encodeStart() {
		avcodec_register_all();
		framenumber = 0;

		const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_MPEG2VIDEO);
		if (!codec) {
			fprintf(stderr, "codec not found\n");
			exit(1);
		}

		c = avcodec_alloc_context3(codec);

		picture = av_frame_alloc();

		pkt = av_packet_alloc();
		if (!pkt) {
			fprintf(stderr, "Cannot alloc packet\n");
			exit(1);
		}

		c->bit_rate = 1000000;

		// resolution must be a multiple of two

		VkExtent2D extent = getEnginePointer()->getWindow()->getExtent();

		c->width = extent.width;
		c->height = extent.height;
		// frames per second
		c->time_base.num = 1;
		c->time_base.den = 25;
		c->framerate.num = 25;
		c->framerate.den = 1;

		c->gop_size = 10; // emit one intra frame every ten frames
		c->max_b_frames = 1;
		c->pix_fmt = AV_PIX_FMT_YUV420P;

		// open it
		if (avcodec_open2(c, codec, NULL) < 0) {
			fprintf(stderr, "could not open codec\n");
			exit(1);
		}

		// Init UDP Sender
		initUDP();

		picture->format = c->pix_fmt;
		picture->width = c->width;
		picture->height = c->height;

		ctx = sws_getContext(c->width, c->height,
			AV_PIX_FMT_RGBA,
			c->width, c->height,
			AV_PIX_FMT_YUV420P,
			0, 0, 0, 0);

		int ret = av_frame_get_buffer(picture, 32);
		if (ret < 0) {
			fprintf(stderr, "could not alloc the frame data\n");
			exit(1);
		}

		// receive control inputs from client in seperate thread
		_beginthreadex(0, 0, receiveFrameOverUDP, (void*)&tcp_connectedSocket, 0, 0);
	}

	void PacEventListener::initUDP() {
		WSADATA wsa;
		long rc;
		int remoteAddrLen = sizeof(SOCKADDR_IN);
		rc = WSAStartup(MAKEWORD(2, 0), &wsa);
		if (rc != 0)
		{
			fprintf(stderr, "Error: Winsock init failed (Error Code: %d)\n", WSAGetLastError());
			exit(1);
		}
		else
		{
			printf("Winsock started!\n");
		}

		// Create UDP Socket
		udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (udp_sock == INVALID_SOCKET)
		{
			fprintf(stderr, "Error: UDP Socket creation failed (Error Code: %d)\n", WSAGetLastError());
			exit(1);
		}
		else
		{
			printf("UDP Socket created!\n");
		}

		udp_addr.sin_family = AF_INET;
		udp_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		udp_addr.sin_port = htons(1234);

		// Create TCP Socket, bind, listen and wait to accept connection
		tcp_acceptSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (tcp_acceptSocket == INVALID_SOCKET)
		{
			printf("Error: TCP Socket creation failed (Error Code: %d)\n", WSAGetLastError());
			exit(1);
		}
		else
		{
			printf("TCP Socket created!\n");
		}

		memset(&tcp_addr, 0, sizeof(SOCKADDR_IN));
		tcp_addr.sin_family = AF_INET;
		tcp_addr.sin_port = htons(12345);
		tcp_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		rc = bind(tcp_acceptSocket, (SOCKADDR*)&tcp_addr, sizeof(SOCKADDR_IN));
		if (rc == SOCKET_ERROR)
		{
			printf("Error: Binding TCP Socket failed  (Error Code: %d)\n", WSAGetLastError());
			exit(1);
		}
		else
		{
			printf("TCP Socket bound to Port 12345!\n");
		}

		rc = listen(tcp_acceptSocket, 10);
		if (rc == SOCKET_ERROR)
		{
			printf("Error: Listening on TCP Socket failed (Error Code: %d)\n", WSAGetLastError());
			exit(1);
		}
		else
		{
			printf("TCP Socket is listening now...\n");
		}

		tcp_connectedSocket = accept(tcp_acceptSocket, NULL, NULL);
		if (tcp_connectedSocket == INVALID_SOCKET)
		{
			printf("Error: Accepting connection on TCP Socket (Error Code: %d)\n", WSAGetLastError());
			exit(1);
		}
		else
		{
			printf("New connection on TCP Port accepted!\n");
		}
	}

	PacEventListener::~PacEventListener() {
		// uint8_t endcode[] = { 0, 0, 1, 0xb7 };

		// flush the encoder
		encode(c, NULL, pkt);

		// add sequence end code to have a real MPEG file
		//fwrite(endcode, 1, sizeof(endcode), outfile);
		// fclose(outfile);

		avcodec_free_context(&c);
		av_frame_free(&picture);
		av_packet_free(&pkt);

		closesocket(tcp_acceptSocket);
		closesocket(tcp_connectedSocket);
		closesocket(udp_sock);
		udp_sock = 0;
	}
};