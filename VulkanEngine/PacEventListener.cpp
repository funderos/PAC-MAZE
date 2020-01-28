#include "VEInclude.h"
#include "math.h"

namespace ve {

	float enemySpeed = 30.0f;

	VESceneNode* pScene;
	VECamera* pCamera;
	VESceneNode* pParent;
	VESceneNode* maze;
	VESceneNode* pills;
	VESceneNode* movable;
	VESceneNode* cat;

	const int pgWidth = 17;
	const int pgHeight = 22;

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
	collectibles pellets[pgWidth][pgHeight];

	int powerPelletX[]{ 1, 1, 15, 15 };
	int powerPelletY[]{ 1, 20, 1, 20 };
	int fillX[2]{ 0, pgWidth - 1 };
	int fillY[2]{ 0, pgHeight - 1 };

	double enDir[5]{ M_PI / 2, 0, M_PI / 2, M_PI, M_PI * 3 / 2 };
	enum headingDirection { STRAIGHT = 0, LEFT = -1, RIGHT = 1, STOP = -2, JUMP = 2 };
	headingDirection headDir[5]{ STRAIGHT, STRAIGHT, STRAIGHT, STRAIGHT, STRAIGHT };

	void PacEventListener::initWorld() {
		pScene = getSceneManagerPointer()->getSceneNode("Level 1");
		pCamera = getSceneManagerPointer()->getCamera();
		pParent = pCamera->getParent();

		maze = getSceneManagerPointer()->createSceneNode("The Grid Parent", pScene, glm::mat4(1.0));
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
		cat->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(8 * 32.0f, 0.5f, 5 * 32.0f)));
		movable->addChild(cat);
		cat->addChild(pCamera);

		pCamera->multiplyTransform(glm::rotate((float)M_PI / 6, glm::vec3(1.0f, 0.0f, 0.0f)));
		pCamera->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.5f, -6.0f)));

		VESceneNode* pinky;
		VECHECKPOINTER(pinky = getSceneManagerPointer()->loadModel("Pinky", "media/models/pacmaze/characters", "pinky.obj"));
		pinky->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(8 * 32.0f, 0.0f, 11 * 32.0f)));
		movable->addChild(pinky);

		VESceneNode* blinky;
		VECHECKPOINTER(blinky = getSceneManagerPointer()->loadModel("Blinky", "media/models/pacmaze/characters", "blinky.obj"));
		blinky->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(8 * 32.0f, 0.0f, 11 * 32.0f)));
		movable->addChild(blinky);

		VESceneNode* inky;
		VECHECKPOINTER(inky = getSceneManagerPointer()->loadModel("Inky", "media/models/pacmaze/characters", "inky.obj"));
		inky->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(8 * 32.0f, 0.0f, 11 * 32.0f)));
		movable->addChild(inky);

		VESceneNode* clyde;
		VECHECKPOINTER(clyde = getSceneManagerPointer()->loadModel("Clyde", "media/models/pacmaze/characters", "clyde.obj"));
		clyde->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(8 * 32.0f, 0.0f, 11 * 32.0f)));
		movable->addChild(clyde);

		initLevel();
	}

	void PacEventListener::initLevel() {
		collectibles tempPellets[pgWidth * pgHeight]
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

		int x = 0;
		int y = 0;

		for (collectibles p : tempPellets) {
			pellets[x][y] = p;
			y++;
			if (y == pgHeight) {
				y = 0;
				x++;
			}
		}

		for (int x = 1; x < pgWidth - 1; x++) {
			for (int y = 1; y < pgHeight - 1; y++) {
				if (pellets[x][y] > 0) {
					if (pellets[x][y + 1] > 0) {
						VESceneNode* pill;
						VECHECKPOINTER(pill = getSceneManagerPointer()->loadModel("Pill-" + std::to_string(x) + "-"
							+ std::to_string(y) + "Y", "media/models/pacmaze/elements", "pill.obj"));
						pill->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
						pill->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(x * 32.0f, 1.5f, y * 32.0f + 16.0f)));
						pills->addChild(pill);
					}
					if (pellets[x + 1][y] > 0) {
						VESceneNode* pill;
						VECHECKPOINTER(pill = getSceneManagerPointer()->loadModel("Pill-" + std::to_string(x) + "-"
							+ std::to_string(y) + "X", "media/models/pacmaze/elements", "pill.obj"));
						pill->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
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
					pill->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
					pill->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(x * 32.0f, 1.5f, y * 32.0f)));
					pills->addChild(pill);
				}
			}
		}
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
	}

	/**
	*
	* \brief Default behavior when the mouse is moved
	*
	* If left button is clicked then is is equivalent of UP/DOWN LEFT/RIGHT keys will rotate the camera.
	* For this we need the previous cursor position so we can determine how the mouse moved, and use this
	* information to move the engine camera.
	*
	* \param[in] event The mouse move event
	* \returns false so event is not consumed
	*
	*/
	bool PacEventListener::onMouseMove(veEvent event) {

		if (!m_rightButtonClicked) return false;		//only do something if left mouse button is pressed

		float x = event.fdata1;
		float y = event.fdata2;

		if (!m_usePrevCursorPosition) {				//can I use the previous cursor position ?
			m_cursorPrevX = x;
			m_cursorPrevY = y;
			m_usePrevCursorPosition = true;
			return true;
		}

		float dx = x - m_cursorPrevX;				//motion of cursor in x and y direction
		float dy = y - m_cursorPrevY;

		m_cursorPrevX = x;							//remember this for next iteration
		m_cursorPrevY = y;

		VECamera* pCamera = getSceneManagerPointer()->getCamera();
		VESceneNode* pParent = pCamera->getParent();

		float slow = 0.5;		//camera rotation speed

		//dx
		float angledx = slow * (float)event.dt * dx;
		glm::vec4 rot4dx = glm::vec4(0.0, 1.0, 0.0, 1.0);
		glm::vec3 rot3dx = glm::vec3(rot4dx.x, rot4dx.y, rot4dx.z);
		glm::mat4 rotatedx = glm::rotate(glm::mat4(1.0), angledx, rot3dx);

		//dy
		float angledy = slow * (float)event.dt * dy;			//pitch angle
		glm::vec4 rot4dy = pCamera->getTransform() * glm::vec4(1.0, 0.0, 0.0, 1.0); //x axis from local to parent space!
		glm::vec3 rot3dy = glm::vec3(rot4dy.x, rot4dy.y, rot4dy.z);
		glm::mat4 rotatedy = glm::rotate(glm::mat4(1.0), angledy, rot3dy);

		pCamera->multiplyTransform(rotatedx * rotatedy);

		return false;
	}

	/**
	*
	* \brief Track buttons of the mouse
	*
	* If a button is clicked or released then this is noted in the engine m_mouse_buttons_clicked set
	*
	* \param[in] event The mouse button event
	* \returns true (event is consumed) or false (event is not consumed)
	*
	*/
	bool PacEventListener::onMouseButton(veEvent event) {

		if (event.idata3 == GLFW_PRESS) {		//just pressed a mouse button
			m_usePrevCursorPosition = false;
			if (event.idata1 == GLFW_MOUSE_BUTTON_RIGHT)
				m_rightButtonClicked = true;
			return true;
		}

		if (event.idata3 == GLFW_RELEASE) {		//just released a mouse button
			m_usePrevCursorPosition = false;
			if (event.idata1 == GLFW_MOUSE_BUTTON_RIGHT)
				m_rightButtonClicked = false;
			return true;
		}

		return false;
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
		int i = 0;
		/*
		for (int i = 1; i < sizeof movable; i++) {
			VESceneNode* mv = movable[i];
			*/

		for (VESceneNode* mv : movable->getChildrenList()) {
			glm::vec3 curPos = mv->getPosition();

			double factor = enemySpeed * event.dt;
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

			}

			curPos.x = curPos.x + factor * sin(enDir[i]);
			curPos.z = curPos.z + factor * cos(enDir[i]);



			if (i) { // enemies
				mv->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f)));
				mv->multiplyTransform(glm::rotate((float)enDir[i], glm::vec3(0.0f, 1.0f, 0.0f)));
				mv->multiplyTransform(glm::translate(glm::mat4(1.0f), curPos));

				glm::vec3 catPos = cat->getPosition();

				if (std::abs(catPos.x - curPos.x) < 15 && std::abs(catPos.z - curPos.z) < 15)
					std::cout << "Collision with " << i;
			}
			else { // cat
				mv->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(8.0f, 8.0f, 8.0f)));
				mv->multiplyTransform(glm::rotate((float)enDir[i], glm::vec3(0.0f, 1.0f, 0.0f)));
				mv->multiplyTransform(glm::translate(glm::mat4(1.0f), curPos));

				for (VESceneNode* pill : pills->getChildrenList()) {
					glm::vec3 pillPos = pill->getPosition();
					if (std::abs(pillPos.x - curPos.x) < 11 && std::abs(pillPos.z - curPos.z) < 11) {
						if (pillPos.y < 0)
							break;
						if (pill->getName().rfind("Yarn", 0) == 0) {
							std::cout << "Got Yarn!";
						}
						pill->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -10.0f, 0.0f)));
						break;
					}
				}				
			}

			i++;
		}
	}
	/**
	*
	* \brief Make a screenshot and save it as PNG
	*
	* If key P has been pressed, transfer the last swapchain image to the host and store it in a PNG
	*
	* \param[in] event The onFrameEnded event
	* \returns false, so event is not consumed
	*
	*/
	void PacEventListener::onFrameEnded(veEvent event) {
		if (m_makeScreenshot) {

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

			m_numScreenshot++;

			std::string name("media/screenshots/screenshot" + std::to_string(m_numScreenshot - 1) + ".jpg");
			stbi_write_jpg(name.c_str(), extent.width, extent.height, 4, dataImage, 4 * extent.width);
			delete[] dataImage;

			m_makeScreenshot = false;
		}

		if (m_makeScreenshotDepth) {

			VETexture* map = getRendererForwardPointer()->getShadowMap(getRendererPointer()->getImageIndex())[0];
			//VkImageLayout layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			VkExtent2D extent = map->m_extent;
			uint32_t imageSize = extent.width * extent.height;
			VkImage image = map->m_image;

			float* dataImage = new float[imageSize];
			/*gli::*/byte* dataImage2 = new /*gli::*/byte[imageSize];

			vh::vhBufCopyImageToHost(getRendererPointer()->getDevice(),
				getRendererPointer()->getVmaAllocator(),
				getRendererPointer()->getGraphicsQueue(),
				getRendererPointer()->getCommandPool(),
				image, map->m_format, VK_IMAGE_ASPECT_DEPTH_BIT, layout,
				(/*gli::*/byte*)dataImage, extent.width, extent.height, imageSize * 4);

			for (uint32_t v = 0; v < extent.height; v++) {
				for (uint32_t u = 0; u < extent.width; u++) {
					dataImage2[v * extent.width + u] = (/*gli::*/byte)((dataImage[v * extent.width + u] - 0.5) * 256.0f * 2.0f);
					//std::cout << dataImage[v*extent.width + u] << " ";
				}
			}

			std::string name("media/screenshots/screenshot" + std::to_string(m_numScreenshot) + ".jpg");
			stbi_write_jpg(name.c_str(), extent.width, extent.height, 1, dataImage2, extent.width);
			delete[] dataImage;
			delete[] dataImage2;

			m_numScreenshot++;
			m_makeScreenshotDepth = false;
		}
	}

	int PacEventListener::getDirection(int x, int y, double angle, bool enemy) {

		bool dir[]{ false, false, false };

		if (x % 32 == 0) {
			if (angle == 0) {
				if (grid[x / 32][(y + 16) / 32] == CAGE) {
					if (grid[x / 32 - 1][(y + 16) / 32] == CAGE)
						dir[0] = true;
					if (grid[x / 32 + 1][(y + 16) / 32] == CAGE)
						dir[2] = true;
					if (grid[x / 32][(y + 48) / 32] == CAGE
						|| (dir[0] && dir[1] && grid[x / 32][(y + 48) / 32] == SPACE))
						dir[1] = true;
				}
				else {
					if (grid[x / 32 - 1][(y + 16) / 32] == SPACE && (x / 32 - 3 < 0
						|| grid[x / 32 - 3][(y + 16) / 32] != TELEPORT || !enemy))
						dir[0] = true;
					if (grid[x / 32 + 1][(y + 16) / 32] == SPACE && (x / 32 + 3 > pgWidth
						|| grid[x / 32 + 3][(y + 16) / 32] != TELEPORT || !enemy))
						dir[2] = true;
					if (grid[x / 32][(y + 48) / 32] == SPACE)
						dir[1] = true;
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
						|| grid[x / 32 - 3][(y - 16) / 32] != TELEPORT || !enemy))
						dir[2] = true;
					if (grid[x / 32 + 1][(y - 16) / 32] == SPACE && (x / 32 + 3 > pgWidth - 1
						|| grid[x / 32 + 3][(y - 16) / 32] != TELEPORT || !enemy))
						dir[0] = true;
					if (grid[x / 32][(y - 48) / 32] == SPACE)
						dir[1] = true;
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
					if (grid[(x + 16) / 32][y / 32 + 1] == CAGE
						|| (dir[2] && dir[1] && grid[(x + 16) / 32][y / 32 + 1] == SPACE))
						dir[0] = true;
				}
				else if (!enemy && grid[(x + 16) / 32][y / 32] == TELEPORT) {
					return 2;
				}
				else {
					if (grid[(x + 16) / 32][y / 32 + 1] == SPACE)
						dir[0] = true;
					if (grid[(x + 16) / 32][y / 32 - 1] == SPACE)
						dir[2] = true;
					if (grid[(x + 48) / 32][y / 32] == TELEPORT || (grid[(x + 48) / 32][y / 32] == SPACE
						&& ((x + 112) / 32 > pgWidth - 1 || grid[(x + 112) / 32][y / 32] != TELEPORT || !enemy)))
						dir[1] = true;
				}
			}
			else {
				if (grid[(x - 16) / 32][y / 32] == CAGE) {
					if (grid[(x - 16) / 32][y / 32 - 1] == CAGE)
						dir[0] = true;
					if (grid[(x - 48) / 32][y / 32] == CAGE)
						dir[1] = true;
					if (grid[(x - 16) / 32][y / 32 + 1] == CAGE
						|| (dir[0] && dir[1] && grid[(x - 16) / 32][y / 32 + 1] == SPACE))
						dir[2] = true;
				}
				else if (!enemy && grid[(x - 16) / 32][y / 32] == TELEPORT) {
					return 2;
				}
				else {
					if (grid[(x - 16) / 32][y / 32 + 1] == SPACE)
						dir[2] = true;
					if (grid[(x - 16) / 32][y / 32 - 1] == SPACE)
						dir[0] = true;
					if (grid[(x - 48) / 32][y / 32] == TELEPORT || grid[(x - 48) / 32][y / 32] == SPACE 
						&& ((x - 112) / 32 < 0 || grid[(x - 112) / 32][y / 32] != TELEPORT || !enemy))
						dir[1] = true;
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
};