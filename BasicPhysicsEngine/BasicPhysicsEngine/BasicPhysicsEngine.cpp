#include "BasicPhysicsEngine.h"

//aie
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include <imgui.h>

//std
#include <iostream>

//local
#include "GameDefines.h"
#include "PhysicsScene.h"
#include "Circle.h"
#include "Plane.h"
#include "AABB.h"
#include "SAT.h"
#include "PhysicsMaterial.h"

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
	m_physicsScene = new PhysicsScene(fixedTimeStep, m_gravity);
	//Just create a ground plane so there's something for it to render and refresh
	m_physicsScene->AddActor(new Plane());

	createMaterials();

	return true;
}

void BasicPhysicsEngine::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->UpdateGizmos();

	//Run demo
	demoloop();

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
	aie::Gizmos::draw2D(glm::ortho<float>(0, 1440, 0, 900, -1, 1));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	char fps[10];
	sprintf_s(fps, 10, "%i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, (float)(getWindowHeight() - 22));

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
void BasicPhysicsEngine::tempStartup()
{
	//m_physicsScene->setGravity(m_gravity);

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
	float elasticity = 0.99f;	//Anything less than 0.98 is boring

	//Circles
	int circleCount = 0;
	float minRadius = 2.5f;
	float maxRadius = 12.5f;
	float circleDensity = density.gold;
	
	for (int i = 0; i < circleCount; ++i)	//Circles
	{
		float radius = pkr::Random::range(minRadius, maxRadius);
		float mass = calcMass(radius, circleDensity);

		m_physicsScene->AddActor(new Circle(
			vec2(50 + 25 * i, 100),
			pkr::Random::range_v2(-initialForce, initialForce),
			mass,
			radius,
			pkr::colour::nice_random(),
			elasticity));
	}

	//AABBs
	int AABBCount = 0;
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
	int SATCount = 1;
	float satDensity = density.osmium;
	float satForce = 100.0f;
	vec2array trapezoid = { vec2(0,0), vec2(6,12.5f), vec2(24, 12.5f), vec2(30,0) };

	for (int i = 0; i < SATCount; ++i)		//SATs
	{
		float size = pkr::Random::range(minSize, maxSize);
		float mass = calcMass(size, size, satDensity);
		SAT* newSat = new SAT(
			vec2(50 + 25 * i, 150),
			pkr::Random::range_v2(-initialForce, initialForce),
			mass,
			pkr::colour::nice_random(),
			pkr::Random::range(3, 8), 10);
		newSat->CentralisePosition();

		m_physicsScene->AddActor(newSat);
	}
}

void BasicPhysicsEngine::createMaterials()
{
	mat_bouncy = new PhysicsMaterial(0.7f, 0.98f, eMaterial::CORK);
	mat_metal = new PhysicsMaterial(0.2f, 0.3f, eMaterial::BRASS);
	mat_slippery = new PhysicsMaterial(0.01f, 0.2f, eMaterial::PLASTICS);
	mat_wooden = new PhysicsMaterial(0.5f, 0.2f, eMaterial::PINE);
}

void BasicPhysicsEngine::demoloop()
{
	aie::Input* input = aie::Input::getInstance();
	static int editMode = 0;	//0 Select, 1 Create, 2 Remove
	static int currentObjectType = 0; //0 Plane, 1 Circle, 2 AABB, 3 SAT
	static vec2 initForce;
	static bool isKinematic;
	static float size1 = 1;
	static float size2 = 1;

	//Mouse stuff
	/* Need: 
	- start point
	- end point
	- drag vector */
	static bool onMouseHold = input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT);
	static bool mouseLHold = false;
	static glm::ivec2 mouseStart(0, 0);
	static glm::ivec2 mouseEnd(0, 0);
	static glm::ivec2 mouseDrag(0, 0);

	//MOUSE START AND END DRAGS
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT) && mouseLHold == false)
	{
		if (!ImGui::IsMouseHoveringAnyWindow())
		{
			input->getMouseXY(&mouseStart.x, &mouseStart.y);
			mouseLHold = true;
		}
	}
	if (input->isMouseButtonUp(aie::INPUT_MOUSE_BUTTON_LEFT) && mouseLHold == true)
	{
		mouseLHold = false;
	}
	if (mouseLHold)
	{
		input->getMouseXY(&mouseEnd.x, &mouseEnd.y);
	}
	if (mouseStart != mouseEnd)
	{
		mouseDrag = mouseEnd - mouseStart;
	}

	ImGui::Begin("2D Physics Demo");
	{
		ImGui::BeginGroup();
		{
			ImGui::Text("Settings");
			ImGui::SliderFloat2("Gravity", glm::value_ptr(m_gravity), -200, 200);
			m_physicsScene->setGravity(m_gravity);
		}
		ImGui::EndGroup();
		ImGui::Spacing();

		ImGui::BeginGroup();
		{
			ImGui::Text("Create");

			ImGui::RadioButton("Plane", &currentObjectType, 0); ImGui::SameLine();
			ImGui::RadioButton("Circle", &currentObjectType, 1); ImGui::SameLine();
			ImGui::RadioButton("AABB", &currentObjectType, 2); ImGui::SameLine();
			ImGui::RadioButton("SAT", &currentObjectType, 3);

			switch (currentObjectType)
			{
			case 0:	//Plane
				ImGui::Text("To create a plane, click and drag to set the plane normal, release to create.");
				break;
			case 1:	//Circle
				ImGui::TextWrapped("To create a circle, click to spawn a random circle at cursor. Click and drag to spawn with velocity. Check Kinematic to create a kinematic object.");
				ImGui::SliderFloat2("Force", glm::value_ptr(initForce), -500, 500);
				ImGui::Checkbox("Kinematic", &isKinematic);
				break;
			case 2:	//AABB
				ImGui::TextWrapped("To create a AABB, click to spawn a random AABB at cursor. Click and drag to spawn with velocity. Check Kinematic to create a kinematic object.");
				ImGui::SliderFloat2("Force", glm::value_ptr(initForce), -500, 500);
				ImGui::Checkbox("Kinematic", &isKinematic);
				break;
			case 3:	//SAT
				ImGui::TextWrapped("To create a SAT, click to spawn a random SAT at cursor. Click and drag to spawn with velocity. Check Kinematic to create a kinematic object.");
				ImGui::SliderFloat2("Force", glm::value_ptr(initForce), -500, 500);
				ImGui::Checkbox("Kinematic", &isKinematic);
				break;
			}
		}
		ImGui::EndGroup();
	}
	ImGui::End();


	//Always in create mode for now
	//If in create mode...
	{
		//Init working vars
		static struct {
			struct {
				float length = 2000.0f;
				vec2 normal;
				vec2 surface;
				vec2 start;
				vec2 end;
			} plane;

			struct {
			} dynamic;
		} paint;

		switch (currentObjectType)
		{
		case 0: //Plane
			//Draw plane stuff

			//If mouse is in drag mode then draw normal and plane to create
			if (mouseLHold)	{
				if (mouseStart != mouseEnd)	{
					aie::Gizmos::add2DLine(mouseStart, mouseEnd, pkr::colour::get(pkr::eColours::COLOUR_RED));
					paint.plane.normal = vec2(mouseEnd - mouseStart);
					paint.plane.surface = vec2(paint.plane.normal.y, -paint.plane.normal.x);
					paint.plane.start = static_cast<vec2>(mouseStart) + (paint.plane.surface * paint.plane.length);
					paint.plane.end = static_cast<vec2>(mouseStart) - (paint.plane.surface * paint.plane.length);
					aie::Gizmos::add2DLine(paint.plane.start, paint.plane.end, pkr::colour::get(pkr::eColours::COLOUR_ORANGE));
				}
			}

			//If mouse has just gone out of drag then create 
			if (input->wasMouseButtonReleased(0)) {
				if (mouseStart != mouseEnd) {
					//Make the plane
					m_physicsScene->AddActor(new Plane(mouseStart, mouseEnd, mat_metal));
				}
			}

			//If left mouse button has been released then create a plane based on the mouse drag
			break;

		case 1: //Circle
			//Draw drag vector
			if (mouseLHold) {
				if (mouseStart != mouseEnd) {
					aie::Gizmos::add2DLine(mouseStart, mouseEnd, pkr::colour::get(pkr::eColours::COLOUR_RED));
				}
			}
			if (input->wasMouseButtonReleased(0))
			{
				if (!ImGui::IsMouseHoveringAnyWindow())
				{
					glm::ivec2 pos; input->getMouseXY(&pos.x, &pos.y);
					auto vel = mouseDrag * 5;
					float rot = 0;
					float radius = pkr::Random::range(20.f, 40.f);
					col colour = pkr::colour::nice_random();
					m_physicsScene->AddActor(new Circle(pos, vel, rot, radius, colour, mat_bouncy, isKinematic));
				}
			}
			break;

		case 2: //AABB
			//Draw drag vector
			if (mouseLHold) {
				if (mouseStart != mouseEnd) {
					aie::Gizmos::add2DLine(mouseStart, mouseEnd, pkr::colour::get(pkr::eColours::COLOUR_RED));
				}
			}
			//Draw shape
			if (input->wasMouseButtonReleased(0))
			{
				if (!ImGui::IsMouseHoveringAnyWindow())
				{
					glm::ivec2 pos; input->getMouseXY(&pos.x, &pos.y);
					auto vel = mouseDrag * 5;
					float rot = 0;
					float width = pkr::Random::range(40.f, 80.f);
					float height = pkr::Random::range(40.f, 80.f);
					col colour = pkr::colour::nice_random();
					m_physicsScene->AddActor(new AABB(pos, vel, width, height, colour, mat_wooden, isKinematic));
				}
			}
			break;

		case 3: //SAT
			//Draw drag vector
			if (mouseLHold) {
				if (mouseStart != mouseEnd) {
					aie::Gizmos::add2DLine(mouseStart, mouseEnd, pkr::colour::get(pkr::eColours::COLOUR_RED));
				}
			}
			if (input->wasMouseButtonReleased(0))
			{
				if (!ImGui::IsMouseHoveringAnyWindow())
				{
					glm::ivec2 pos;
					input->getMouseXY(&pos.x, &pos.y);
					auto vel = mouseDrag * 5;
					float rot = 0;
					float size = pkr::Random::range(40, 80);
					int sides = pkr::Random::range(3, 8);
					col colour = pkr::colour::nice_random();
					m_physicsScene->AddActor(new SAT(pos, vel, rot, size, sides, colour, mat_metal, isKinematic));
				}
			}
			break;
		}
	}


}