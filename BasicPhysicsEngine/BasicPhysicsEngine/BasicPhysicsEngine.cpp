#include "BasicPhysicsEngine.h"

//aie
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>

//std
#include <iostream>

//local
#include "GameDefines.h"
#include "PhysicsScene.h"
#include "Circle.h"
#include "Plane.h"
#include "AABB.h"
#include "SAT.h"

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

	//Initiate the demo
	Demo(m_gravity);

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
	
	char fps[10];
	sprintf_s(fps, 10, "%i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, (float)(getWindowHeight()-22));

	// done drawing sprites
	m_2dRenderer->end();
}

float BasicPhysicsEngine::calcMass(float radius, float density)
{
	return PI * radius * radius * density / 1000.f;
}

float BasicPhysicsEngine::calcMass(float width, float height, float density)
{
	return width * height * density / 1000.f;
}

void BasicPhysicsEngine::SetupContinuousDemo(vec2 startPos, float inclination, float speed, float gravity)
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

void BasicPhysicsEngine::Demo(float gravity)
{
	m_physicsScene->setGravity(vec2(0, gravity));

	//Material densities
	struct {
		float hydrogen = 0.0898f;
		float helium = 0.179f;
		float aerographite = 0.2f;
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
		float osmium = 22570;	//Heaviest Metal: 14.3% denser than gold, 39.99% denser than mercury, around twice a dense as lead
	} density; //g/cm3

	//Straight cushions
	int cushionSize = 15;
	Plane *bottomCushion = new Plane();
	m_physicsScene->AddActor(bottomCushion);
	Plane *topCushion = new Plane(vec2(0.0f, -1.0f), (float)-285 + cushionSize);
	m_physicsScene->AddActor(topCushion);
	//Plane *leftCushion = new Plane(vec2(1.0f, 0.0f), (float)cushionSize);
	//m_physicsScene->AddActor(leftCushion);
	Plane *rightCushion = new Plane(vec2(-1.0f, 0.0f), (float)-500 + cushionSize);
	m_physicsScene->AddActor(rightCushion);
	//Angled cushions
	Plane *angCushionLeft = new Plane(vec2(3, 2.0f), 110.f);	//Normal override
	m_physicsScene->AddActor(angCushionLeft);
	Plane *angCushionRight = new Plane(-1, 4, -40);			//ax + by + d override
	m_physicsScene->AddActor(angCushionRight);
	Plane *angCushionTop = new Plane(vec2(0, 160), vec2(150, 230), RIGHT);	//2 points override
	m_physicsScene->AddActor(angCushionTop);

	//////////////////////////
	//// OBJECT CREATION
	////////////////////////
	float initialForce = 300.0f;

	//Circles
	int circleCount = 15;
	float minRadius = 2.5f;
	float maxRadius = 10.f;
	float circleDensity = density.hydrogen;
	
	for (int i = 0; i < circleCount; ++i)	//Circles
	{
		float radius = pkr::Random::range(minRadius, maxRadius);
		float mass = calcMass(radius, circleDensity);

		m_physicsScene->AddActor(new Circle(
			vec2(50 + 25 * i, 100),
			pkr::Random::range_v2(-initialForce, initialForce),
			mass,
			radius,
			pkr::colour::nice_random()));
	}

	//AABBs
	int AABBCount = 5;
	float aabbDensity = density.copper;
	float minSize = 5.f;
	float maxSize = 20.f;
	
	for (int i = 0; i < AABBCount; ++i)		//AABBs
	{
		float width = pkr::Random::range(minSize, maxSize);
		float height = pkr::Random::range(minSize, maxSize);
		float mass = calcMass(width, height, aabbDensity);

		m_physicsScene->AddActor(new AABB(
			vec2(50 + 25 * i, 200),
			pkr::Random::range_v2(-initialForce, initialForce),
			mass,
			width,
			height,
			pkr::colour::nice_random()));
	}

	//SATs
	int SATCount = 5;
	float satDensity = density.osmium;
	float satForce = 100.0f;
	vec2array satVextents = { vec2(0, 0), vec2(-5, 7.5f), vec2(0, 15), vec2(8, 15), vec2(13, 7.5f), vec2(8, 0)};
	
	for (int i = 0; i < SATCount; ++i)		//SATs
	{
		float size = pkr::Random::range(minSize, maxSize);
		float mass = calcMass(size, size, satDensity);
		SAT* newSat = new SAT(
			vec2(50 + 25 * i, 150),
			pkr::Random::range_v2(-initialForce, initialForce),
			mass,
			pkr::colour::nice_random(),
			satVextents);
		newSat->CentralisePosition();

		m_physicsScene->AddActor(newSat);

		//m_physicsScene->AddActor(new SAT(
		//	vec2(50 + 25 * i, 150),
		//	pkr::Random::range_v2(-initialForce, initialForce),
		//	mass,
		//	pkr::colour::nice_random(),
		//	satVextents));
	}
}