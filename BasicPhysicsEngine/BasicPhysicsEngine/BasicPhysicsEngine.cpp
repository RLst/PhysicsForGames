#include "BasicPhysicsEngine.h"

//aie
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>

//std
#include <iostream>

//gl
#include <glm/ext.hpp>

#include "GameDefines.h"
#include "PhysicsScene.h"
#include "Circle.h"
#include "Plane.h"


BasicPhysicsEngine::BasicPhysicsEngine() {
}

BasicPhysicsEngine::~BasicPhysicsEngine() {

}

void BasicPhysicsEngine::shutdown() {
	delete m_font;
	delete m_2dRenderer;
	delete m_physicsScene;
}

bool BasicPhysicsEngine::startup() 
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
	m_physicsScene->setTimeStep(fixedTimeStep);

	//Circle2CircleTest();
	//Circle2PlaneTest();
	BilliardBallSimulation();

	return true;
}

void BasicPhysicsEngine::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	//Apply force towards the other actor
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
	}

	m_physicsScene->Update(deltaTime);
	m_physicsScene->UpdateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void BasicPhysicsEngine::draw() {

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

void BasicPhysicsEngine::SetupContinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity)
{
	//Analytical solution
	///Circle properties
	float radius = 3.0f;
	int segments = 12;
	col colour = pkr::colour::yellow;

	//Time and steps
	float t = 0;
	float tStep = fixedTimeStep;
	int steps = 300;

	vec2 vel0;
	vel0.x = speed * cosf(pkr::DegsToRad(inclination));
	vel0.y = speed * sinf(pkr::DegsToRad(inclination));

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

void BasicPhysicsEngine::Circle2CircleTest()
{
	float collideSpeed = 100;
	float distanceApart = 100;
	float mass = 10;
	float radius = 10;

	//spawn in two circles and make them collide
	m_physicsScene->setGravity(pkr::zero2);
	m_physicsScene->AddActor(new Circle(vec2(250 - distanceApart, 150), vec2(collideSpeed, 0), mass, radius, pkr::colour::violet));
	m_physicsScene->AddActor(new Circle(vec2(250 + distanceApart, 150), vec2(-collideSpeed, 0), mass, radius, pkr::colour::orange));
}

void BasicPhysicsEngine::Circle2PlaneTest()
{
	int numberOfCircles = 10;
	float mass = 10;
	float radius = 10;
	
	glm::ivec2 spawnRangeX(10, 490);
	glm::ivec2 spawnRangeY(0, 250);

	m_physicsScene->setGravity(vec2(0, -9.81f));

	//Make the plane
	m_physicsScene->AddActor(new Plane(vec2(0,1), 50));
	
	//Spawn in a bunch of circles in random places and let them drop on the plane
	std::vector<Circle*> circles;
	for (int i = 0; i < numberOfCircles; ++i)
	{
		circles.push_back(new Circle(
			vec2(pkr::random(spawnRangeX.x, spawnRangeX.y), pkr::random(spawnRangeY.x, spawnRangeY.y)),
			pkr::zero2,
			mass,
			radius,
			pkr::colour::random()));
		m_physicsScene->AddActor(circles.back());
	}
}

void BasicPhysicsEngine::BilliardBallSimulation()
{
	m_physicsScene->setGravity(pkr::zero2);		//Balls sitting on a table, no gravity

	int cushionSize = 25;
	//Cushions to stop the balls
	Plane *topCushion = new Plane(vec2(0.0f, -1.0f), (float)-285 + cushionSize);
	Plane *bottomCushion = new Plane(vec2(0.0f, 1.0f), (float)cushionSize);
	Plane *leftCushion = new Plane(vec2(1.0f, 0.0f), (float)cushionSize);
	Plane *rightCushion = new Plane(vec2(-1.0f, 0.0f), (float)-500 + cushionSize);
	m_physicsScene->AddActor(topCushion);
	m_physicsScene->AddActor(bottomCushion);
	m_physicsScene->AddActor(leftCushion);
	m_physicsScene->AddActor(rightCushion);

	//Cue balls
	float radius = 7.5f;
	int numberOfBalls = 20;

	Circle *cueBall = new Circle(glm::vec2(100, 150), vec2(900.0f, 0.0f), 0.170f, radius, pkr::colour::white);
	m_physicsScene->AddActor(cueBall);

	for (int i = 0; i < numberOfBalls; ++i)
	{
		m_physicsScene->AddActor(new Circle(vec2(pkr::random(cushionSize+radius, 500-cushionSize-radius), pkr::random(cushionSize+radius, 285-cushionSize-radius)), vec2(0, 0), 0.160f, radius, pkr::colour::random()));
	}
}
