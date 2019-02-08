#include "PhysicsBaseApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include <glm/ext.hpp>
#include "GameDefines.h"

#include <iostream>

PhysicsBaseApp::PhysicsBaseApp() {
}

PhysicsBaseApp::~PhysicsBaseApp() {

}

void PhysicsBaseApp::shutdown() {
	delete m_font;
	delete m_2dRenderer;
	delete m_physicsScene;
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
	m_physicsScene = new PhysicsScene(false);	//Collision off
	m_physicsScene->setGravity({ 0, -0 });
	m_physicsScene->setTimeStep(0.01f);

	////Test Newton's first law
	//ball10 = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 10, glm::vec4(1, 0, 0, 1));
	//m_physicsScene->AddActor(ball10);

	////Test Newton's Third Law
	//ballSml = new Circle(glm::vec2(-50, 20), glm::vec2(0,0), 5.0f, 5, pkr::colour::red);
	//ballMed = new Circle(glm::vec2(50, -20), glm::vec2(0,0), 10.0f, 10, pkr::colour::orange);
	//ballLge = new Circle(glm::vec2(0, 0), glm::vec2(0, 0), 20.0f, 20, pkr::colour::cyan);
	//m_physicsScene->AddActor(ballSml);
	//m_physicsScene->AddActor(ballMed);
	//m_physicsScene->AddActor(ballLge);

	//Simulating a Rocket
	fuelMass = 10.0f;
	fuelTank = 10000.0f;
	fuelSize = 5.0f;
	rocket = new Circle(glm::vec2(500, 500), glm::vec2(0, 0), 1000 + fuelTank, 20, pkr::colour::blue);
	m_physicsScene->AddActor(rocket);

	return true;
}

void PhysicsBaseApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	//Apply force towards the other actor
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		////Seek (destination - current)
		//auto force = ballMed->getPosition() - ballSml->getPosition();
		//ballSml->ApplyForce(force);
		////ballMed->ApplyForceToActor(ballLge, force);

		//Rocket test
		///Reduce the mass of the rocket by M to sim fuel being used as long as there is fuel left
		if (rocket->getMass() > fuelTank)
			rocket->reduceMass(fuelMass);

		///Create thrust particle
		particles.push_back(new Circle(rocket->getPosition(), v2(0, 0), fuelMass, fuelSize, pkr::colour::orange));
		m_physicsScene->AddActor(particles.back());

		///Apply force to exhaust gas
		static int mx, my;
		input->getMouseXY(&mx, &my);
		auto mousePos = v2(mx, my);
		auto thrust = rocket->getPosition() - mousePos;
		particles.back()->ApplyForceToActor(rocket, thrust);

		std::cout << "MousePos: " << mousePos.x << ", " << mousePos.y << std::endl;
		std::cout << "RocketMass: " << rocket->getMass() << std::endl;
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
	aie::Gizmos::draw2D(glm::ortho<float>(0, 1440, 0 / aspectRatio, 840, -1.0f, 1.0f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}