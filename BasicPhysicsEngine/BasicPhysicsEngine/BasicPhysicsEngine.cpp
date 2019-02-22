#include "BasicPhysicsEngine.h"

//aie
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>

//std
#include <iostream>
#include <time.h>

//gl
#include <glm/ext.hpp>

#include "GameDefines.h"
#include "PhysicsScene.h"
#include "Circle.h"
#include "Plane.h"
#include "Box.h"


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
	//Randomize
	srand(unsigned int(time(0)));

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
	//BilliardBallSimulation();
	AABBTest();

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
	col colour = pkr::colour::get(pkr::COLOUR_RED);

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
	m_physicsScene->AddActor(new Circle(vec2(250 - distanceApart, 150), vec2(collideSpeed, 0), mass, radius, pkr::colour::get(pkr::COLOUR_PURPLE)));
	m_physicsScene->AddActor(new Circle(vec2(250 + distanceApart, 150), vec2(-collideSpeed, 0), mass, radius, pkr::colour::get(pkr::COLOUR_ORANGE)));
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
			pkr::colour::colour::random()));
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

	Circle *cueBall = new Circle(glm::vec2(100, 150), vec2(2000.f, 500.0f), 0.170f, radius, pkr::colour::shade(0));
	m_physicsScene->AddActor(cueBall);

	for (int i = 0; i < numberOfBalls; ++i)
	{
		m_physicsScene->AddActor(new Circle(vec2(pkr::random(cushionSize+radius, 500-cushionSize-radius), pkr::random(cushionSize+radius, 285-cushionSize-radius)), vec2(0, 0), 0.160f, radius, pkr::colour::random()));
	}
}

void BasicPhysicsEngine::AABBTest()
{
	//m_physicsScene->setGravity(pkr::zero2);
	m_physicsScene->setGravity(vec2(0, -98.1f));

	int cushionSize = 25;
	//Cushions to stop the balls
	//Plane *bottomCushion = new Plane();
	Plane *bottomCushion = new Plane(vec2(0.0f, 1.0f), (float)cushionSize);
	m_physicsScene->AddActor(bottomCushion);
	Plane *topCushion = new Plane(vec2(0.0f, -1.0f), (float)-285 + cushionSize);
	m_physicsScene->AddActor(topCushion);
	Plane *leftCushion = new Plane(vec2(1.0f, 0.0f), (float)cushionSize);
	m_physicsScene->AddActor(leftCushion);
	Plane *rightCushion = new Plane(vec2(-1.0f, 0.0f), (float)-500 + cushionSize);
	m_physicsScene->AddActor(rightCushion);
	//Plane *leftAngledCushion = new Plane(vec2(0, 100), vec2(200, 0));
	Plane *leftAngledCushion = new Plane(vec2(1.5f, 2.0f), 100.f);
	m_physicsScene->AddActor(leftAngledCushion);
	Plane *rightAngledCushion = new Plane(-1.5f, 2, -200);
	m_physicsScene->AddActor(rightAngledCushion);
	Plane *topLeftAngledCushion = new Plane(vec2(0, 50), vec2(100, 200), RIGHT);
	m_physicsScene->AddActor(topLeftAngledCushion);

	float initialForce = 10.f;

	struct {
		float hydrogen = 0.0898f;
		float helium = 0.179f;
		float aerographite = 0.2;
		float air = 1.2f;
		float tungsten_hexaflouride = 12.4f;
		float styrofoam = 75;
		float cork = 240;
		float pine = 373;
		float lithium = 535;
		float wood = 700;
		float ice = 916.7f;
		float water = 1000;
		float salt_water = 1030;
		float plastics = 1175;
		float magnesium = 1740;
		float concrete = 2400;
		float diamond = 3500;
		float vanadium = 6100;
		float brass = 8600;
		float copper = 8940;
		float silver = 10500;
		float lead = 11340;
		float rhodium = 12410;
		float mercury = 13546;
		float tungsten = 19300;
		float gold = 19320;
		float platinum = 21450;
		float iridium = 22420;
		float osmium = 22570;
	} density; //g/cm3

	//Circles
	int numberOfCircles = 17;
	int minRadius = 2;
	int maxRadius = 15;
	float circleDensity = density.air;

	//Boxes
	int numberOfBoxes = 17;
	int minSize = 5;
	int maxSize = 25;
	float boxDensity = density.osmium;

	//Create circles and boxes
	for (int i = 0; i < numberOfCircles; ++i)
	{
		float radius = pkr::random(minRadius, maxRadius);
		float mass = calcMass(radius, circleDensity);

		m_physicsScene->AddActor(new Circle(
			vec2(50 + 25 * i, 100),
			vec2(pkr::random(-initialForce, initialForce), pkr::random(-initialForce, initialForce)),
			mass,
			radius,
			pkr::colour::nice_random()));
	}
	for (int i = 0; i < numberOfBoxes; ++i)
	{
		float width = pkr::random(minSize, maxSize);
		float height = pkr::random(minSize, maxSize);
		float mass = calcMass(width, height, boxDensity);

		m_physicsScene->AddActor(new Box(
			vec2(50 + 25 * i, 200),
			vec2(pkr::random(-initialForce, initialForce), pkr::random(-initialForce, initialForce)),
			mass,
			width,
			height,
			pkr::colour::nice_random()));
	}
}

float BasicPhysicsEngine::calcMass(float circleRadius, float density)
{
	return PI * circleRadius * circleRadius * density / 1000.f;
}

float BasicPhysicsEngine::calcMass(float boxWidth, float boxHeight, float density)
{
	return boxWidth * boxHeight * density / 1000.f;
}