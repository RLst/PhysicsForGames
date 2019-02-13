#include "PhysicsTutorial.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include "GameDefines.h"

#include <glm/ext.hpp>

#include "PhysicsScene.h"
#include "Circle.h"
#include "Plane.h"

#include <iostream>

PhysicsTutorial::PhysicsTutorial() {
}

PhysicsTutorial::~PhysicsTutorial() {

}

void PhysicsTutorial::shutdown() {
	delete m_font;
	delete m_2dRenderer;
	delete m_physicsScene;
}

bool PhysicsTutorial::startup() 
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
	m_physicsScene = new PhysicsScene(false);
	m_physicsScene->setGravity({ 0, -9.81 });
	m_physicsScene->setTimeStep(0.01f);

	////Projectile Motion Tutorial 2
	float mass = 1.0f;
	float radius = 1.0f;
	float speed = 30.0f;
	vec2 startPos(50, 50);
	float inclination = 45.0f * PI / 180.0f;
	vec2 velocity(speed * cosf(inclination), speed * sinf(inclination));

	m_physicsScene->AddActor(new Circle(startPos, velocity, mass, radius, pkr::colour::red));
	setupContinuousDemo(startPos, inclination, speed, -9.81f);

	return true;
}

void PhysicsTutorial::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	//aie::Gizmos::clear();

	//Apply force towards the other actor
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		vec2 startPos = vec2(50, 50);
		//Calc vector of mouse
		float mx = input->getMouseX() - startPos.x;
		float my = input->getMouseY() - startPos.y;
		float speedFactor = 0.090f;
		float speed = sqrtf(mx * mx + my * my) * speedFactor;		//hypotenuse (pythagoras)
		//Calc "inclination" of mouse
		float inclination = atan2f(my, mx) * 180.0f / PI;

		setupContinuousDemo(startPos, inclination, speed, -9.81f);

	}

	m_physicsScene->Update(deltaTime);
	m_physicsScene->UpdateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsTutorial::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(0, 500, 0 / aspectRatio, 500 / aspectRatio, -1, 1));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsTutorial::setupContinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity)
{
	float t = 0;
	float tStep = 0.5f;
	int steps = 12;
	float radius = 1.0f;
	int segments = 12;
	col colour = pkr::colour::yellow;

	vec2 vel0;
	vel0.x = speed * cosf(inclination * PI / 180.0f);
	vel0.y = speed * sinf(inclination * PI / 180.0f);

	while (t <= steps * tStep)
	{
		//Calc the x,y position of the projectile at time t
		vec2 pos;
		pos.x = startPos.x + vel0.x * t;
		pos.y = startPos.y + vel0.y * t + 0.5f * gravity * t * t;

		aie::Gizmos::add2DCircle(pos, radius, segments, colour);

		t += tStep;
	}
}
