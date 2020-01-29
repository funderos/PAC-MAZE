#include "VEInclude.h"
#include "math.h"

namespace ve {
	///Register an event listener to interact with the user
	void PacEngine::registerEventListeners() {
		registerEventListener(new PacEventListener("PacListener"));
	};



	///Load the first level into the game engine
	///The engine uses Y-UP, Left-handed
	void PacEngine::loadLevel(uint32_t numLevel) {

		//camera parent is used for translations
		VESceneNode* cameraParent = getSceneManagerPointer()->createSceneNode("StandardCameraParent", getRoot(),
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

		//camera can only do yaw (parent y-axis) and pitch (local x-axis) rotations
		VkExtent2D extent = getWindowPointer()->getExtent();
		VECameraProjective* camera = (VECameraProjective*)getSceneManagerPointer()->createCamera("StandardCamera", VECamera::VE_CAMERA_TYPE_PROJECTIVE, cameraParent);
		camera->m_nearPlane = 0.1f;
		camera->m_farPlane = 300.1f;
		camera->m_aspectRatio = extent.width / (float)extent.height;
		camera->m_fov = 45.0f;
		camera->lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		getSceneManagerPointer()->setCamera(camera);

		VELight* light4 = (VESpotLight*)getSceneManagerPointer()->createLight("StandardAmbientLight", VELight::VE_LIGHT_TYPE_AMBIENT, camera);
		light4->m_col_ambient = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);

		//use one light source
		VELight* light1 = (VEDirectionalLight*)getSceneManagerPointer()->createLight("StandardDirLight", VELight::VE_LIGHT_TYPE_DIRECTIONAL, getRoot());     //new VEDirectionalLight("StandardDirLight");
		light1->lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		light1->m_col_diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		light1->m_col_specular = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

		VESceneNode* pScene;
		VECHECKPOINTER(pScene = getSceneManagerPointer()->createSceneNode("Level 1", getRoot()));

		//scene models

		VESceneNode* e4;
		VECHECKPOINTER(e4 = getSceneManagerPointer()->loadModel("The Plane", "media/models/test", "plane_t_n_s.obj", 0, pScene));
		e4->setTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1.0f, 1000.0f)));

		VEEntity* pE4;
		VECHECKPOINTER(pE4 = (VEEntity*)getSceneManagerPointer()->getSceneNode("The Plane/plane_t_n_s.obj/plane/Entity_0"));
		pE4->setParam(glm::vec4(1000.0f, 1000.0f, 0.0f, 0.0f));
		

		VESceneNode* e1, * eParent;
		eParent = getSceneManagerPointer()->createSceneNode("The Logo Parent", pScene, glm::mat4(1.0));
		VECHECKPOINTER(e1 = getSceneManagerPointer()->loadModel("The Logo", "media/models/pacmaze/static", "logo.obj"));
		eParent->multiplyTransform(glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05)));
		eParent->multiplyTransform(glm::rotate((float)M_PI / 2, glm::vec3(1.0, 0.0, 0.0)));
		eParent->multiplyTransform(glm::translate(glm::mat4(1.0f), glm::vec3(8.0 * 3.2, 80.0, 12.0 * 3.2)));
		eParent->addChild(e1);

		VECamera* pCamera = getSceneManagerPointer()->getCamera();
		VESceneNode* pParent = pCamera->getParent();

		glm::vec4 translate = glm::vec4(0.0, 0.0, 0.0, 1.0);	//total translation
		glm::vec4 rot4 = glm::vec4(1.0);						//total rotation around the axes, is 4d !

		translate = pCamera->getTransform() * glm::vec4(8.0 * 3.2, 0.0, 11.0 * 3.2, 1.0);
		glm::vec3 trans = glm::vec3(translate.x, translate.y, translate.z);
		pParent->setTransform(glm::translate(glm::mat4(1.0f), trans));

		translate = glm::vec4(0.0, 100.0, 0.0, 1.0);
		trans = glm::vec3(translate.x, translate.y, translate.z);
		pParent->multiplyTransform(glm::translate(glm::mat4(1.0f), trans));
		
		rot4 = pCamera->getTransform() * glm::vec4(1.0, 0.0, 0.0, 1.0); //x axis from local to parent space!
		///combination of yaw and pitch, both wrt to parent space
		glm::vec3  rot3 = glm::vec3(rot4.x, rot4.y, rot4.z);
		glm::mat4  rotate = glm::rotate(glm::mat4(1.0), (float)M_PI/2, rot3);
		pCamera->multiplyTransform(rotate);

		//getSceneManagerPointer()->getSceneNode("StandardCameraParent")->setTransform(cat->getTransform());

		/*
		VEMesh* pMesh;
		VECHECKPOINTER(pMesh = getSceneManagerPointer()->getMesh("media/models/test/crate0/cube.obj/cube"));

		VEMaterial* pMat;
		VECHECKPOINTER(pMat = getSceneManagerPointer()->getMaterial("media/models/test/crate0/cube.obj/cube"));

		for (uint32_t i = 0; i < n; i++) {
			VESceneNode* pNode;
			VECHECKPOINTER(pNode = getSceneManagerPointer()->createSceneNode("The Node" + std::to_string(i), parent));
			pNode->setTransform(glm::translate(glm::mat4(1.0f), glm::vec3(d(e) - stride / 2.0f, d(e) / 2.0f, d(e) - stride / 2.0f)));
			registerEventListener(new RotatorListener("RotatorListener" + std::to_string(i), pNode, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f)), { veEvent::VE_EVENT_FRAME_STARTED, veEvent::VE_EVENT_DELETE_NODE });

			VEEntity* e2;
			VECHECKPOINTER(e2 = getSceneManagerPointer()->createEntity("The Cube" + std::to_string(i), pMesh, pMat, pNode));
			registerEventListener(new RotatorListener("RotatorListener2" + std::to_string(i), e2, v(e), glm::vec3(r(e), r(e), r(e))), { veEvent::VE_EVENT_FRAME_STARTED, veEvent::VE_EVENT_DELETE_NODE });
			//registerEventListener( new BlinkListener("BlinkListener" + std::to_string(i), e2, dur(e) ), { veEvent::VE_EVENT_FRAME_STARTED, veEvent::VE_EVENT_DELETE_NODE });
		}
		*/

		registerEventListeners();
	}
}