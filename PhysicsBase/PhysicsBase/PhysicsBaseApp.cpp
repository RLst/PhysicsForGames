#include "PhysicsBaseApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include <glm/ext.hpp>
#include "GameDefines.h"

PhysicsBaseApp::PhysicsBaseApp() {
}

PhysicsBaseApp::~PhysicsBaseApp() {

}

bool PhysicsBaseApp::startup() 
{
	//Increase 2D line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
#ifdef _DEBUG
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);
#elif NDEBUG
	m_font = new aie::Font("./font/consolas.ttf", 32);
#endif

	//Initialize the physics scene
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity({ 0, 0 });
	m_physicsScene->setTimeStep(0.01f);

	////Test Newton's first law
	//ball10 = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 10, glm::vec4(1, 0, 0, 1));
	//m_physicsScene->AddActor(ball10);

	//Test Newton's Third Law
	ballSml = new Circle(glm::vec2(-50, 20), glm::vec2(0,0), 5.0f, 5, pkr::colour::red);
	ballMed = new Circle(glm::vec2(50, -20), glm::vec2(0,0), 10.0f, 10, pkr::colour::orange);
	ballLge = new Circle(glm::vec2(0, 0), glm::vec2(0, 0), 20.0f, 20, pkr::colour::violet);
	m_physicsScene->AddActor(ballSml);
	m_physicsScene->AddActor(ballMed);
	m_physicsScene->AddActor(ballLge);

	return true;
}

void PhysicsBaseApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
	delete m_physicsScene;
}

void PhysicsBaseApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	//Apply force towards the other actor
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		//Seek (destination - current)
		auto force = ballMed->getPosition() - ballSml->getPosition();
		//ballMed->ApplyForceToActor(ballSml, force);
		ballSml->ApplyForce(force);
	}

	m_physicsScene->Update(deltaTime);
	m_physicsScene->UpdateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsBaseApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}