#include "BasicPhysicsEngine.h"

//aie
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include <imgui.h>

//std

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
	m_physicsScene->AddActor(new Plane());	//Required so that update runs properly

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

//void BasicPhysicsEngine::tempStartup()
//{
//	//m_physicsScene->setGravity(m_gravity);
//
//	//Material densities
//	struct {
//		float hydrogen = 0.0898f;
//		float helium = 0.179f;
//		float aerographite = 0.2f;
//		float air = 1.2f;
//		float tungsten_hexaflouride = 12.4f;
//		float styrofoam = 75;
//		float cork = 240;
//		float pine = 373;
//		float lithium = 535;
//		float wood = 700;
//		float ice = 916.7f;
//		float water = 1000;
//		float salt_water = 1030;
//		float plastics = 1175;
//		float magnesium = 1740;
//		float concrete = 2400;
//		float diamond = 3500;
//		float vanadium = 6100;
//		float brass = 8600;
//		float copper = 8940;
//		float silver = 10500;
//		float lead = 11340;
//		float rhodium = 12410;
//		float mercury = 13546;
//		float tungsten = 19300;
//		float gold = 19320;
//		float platinum = 21450;
//		float iridium = 22420;
//		float osmium = 22570;	//Heaviest Metal: 14.3% denser than gold, 39.99% denser than mercury, around twice a dense as lead
//	} density; //g/cm3
//
//	//Straight cushions
//	int cushionSize = 15;
//	Plane *bottomCushion = new Plane();
//	m_physicsScene->AddActor(bottomCushion);
//	Plane *topCushion = new Plane(vec2(0.0f, -1.0f), (float)-285 + cushionSize);
//	m_physicsScene->AddActor(topCushion);
//	//Plane *leftCushion = new Plane(vec2(1.0f, 0.0f), (float)cushionSize);
//	//m_physicsScene->AddActor(leftCushion);
//	Plane *rightCushion = new Plane(vec2(-1.0f, 0.0f), (float)-500 + cushionSize);
//	m_physicsScene->AddActor(rightCushion);
//	//Angled cushions
//	Plane *angCushionLeft = new Plane(vec2(3, 2.0f), 110.f);	//Normal override
//	m_physicsScene->AddActor(angCushionLeft);
//	Plane *angCushionRight = new Plane(-1, 4, -40);			//ax + by + d override
//	m_physicsScene->AddActor(angCushionRight);
//	Plane *angCushionTop = new Plane(vec2(0, 160), vec2(150, 230), RIGHT);	//2 points override
//	m_physicsScene->AddActor(angCushionTop);
//
//	//////////////////////////
//	//// OBJECT CREATION
//	////////////////////////
//	float initialForce = 300.0f;
//	float elasticity = 0.99f;	//Anything less than 0.98 is boring
//
//	//Circles
//	int circleCount = 0;
//	float minRadius = 2.5f;
//	float maxRadius = 12.5f;
//	float circleDensity = density.gold;
//
//	for (int i = 0; i < circleCount; ++i)	//Circles
//	{
//		float radius = pkr::Random::range(minRadius, maxRadius);
//		float mass = calcMass(radius, circleDensity);
//
//		m_physicsScene->AddActor(new Circle(
//			vec2(50 + 25 * i, 100),
//			pkr::Random::range_v2(-initialForce, initialForce),
//			mass,
//			radius,
//			pkr::colour::nice_random(),
//			elasticity));
//	}
//
//	//AABBs
//	int AABBCount = 0;
//	float aabbDensity = density.copper;
//	float minSize = 5.f;
//	float maxSize = 20.f;
//
//	for (int i = 0; i < AABBCount; ++i)		//AABBs
//	{
//		float width = pkr::Random::range(minSize, maxSize);
//		float height = pkr::Random::range(minSize, maxSize);
//		float mass = calcMass(width, height, aabbDensity);
//
//		m_physicsScene->AddActor(new AABB(
//			vec2(50 + 25 * i, 200),
//			pkr::Random::range_v2(-initialForce, initialForce),
//			mass,
//			width,
//			height,
//			pkr::colour::nice_random()));
//	}
//
//	//SATs
//	int SATCount = 1;
//	float satDensity = density.osmium;
//	float satForce = 100.0f;
//	vec2array trapezoid = { vec2(0,0), vec2(6,12.5f), vec2(24, 12.5f), vec2(30,0) };
//
//	for (int i = 0; i < SATCount; ++i)		//SATs
//	{
//		float size = pkr::Random::range(minSize, maxSize);
//		float mass = calcMass(size, size, satDensity);
//		SAT* newSat = new SAT(
//			vec2(50 + 25 * i, 150),
//			pkr::Random::range_v2(-initialForce, initialForce),
//			mass,
//			pkr::colour::nice_random(),
//			pkr::Random::range(3, 8), 10);
//		newSat->CentralisePosition();
//
//		m_physicsScene->AddActor(newSat);
//	}
//}

void BasicPhysicsEngine::createMaterials()
{
	float friction = 0.4f;
	float elasticity = 0.8f;
	m_material.hydrogen = new PhysicsMaterial(friction, elasticity, eMaterial::HYDROGEN);
	m_material.air = new PhysicsMaterial(friction, elasticity, eMaterial::AIR);
	m_material.styrofoam = new PhysicsMaterial(friction, elasticity, eMaterial::STYROFOAM);
	m_material.lithium = new PhysicsMaterial(friction, elasticity, eMaterial::LITHIUM);
	m_material.wood = new PhysicsMaterial(friction, elasticity, eMaterial::WOOD);
	m_material.ice = new PhysicsMaterial(friction, elasticity, eMaterial::ICE);
	m_material.water = new PhysicsMaterial(friction, elasticity, eMaterial::WATER);
	m_material.plastics = new PhysicsMaterial(friction, elasticity, eMaterial::PLASTICS);
	m_material.diamond = new PhysicsMaterial(friction, elasticity, eMaterial::DIAMOND);
	m_material.copper = new PhysicsMaterial(friction, elasticity, eMaterial::COPPER);
	m_material.silver = new PhysicsMaterial(friction, elasticity, eMaterial::SILVER);
	m_material.lead = new PhysicsMaterial(friction, elasticity, eMaterial::LEAD);
	m_material.mercury = new PhysicsMaterial(friction, elasticity, eMaterial::MERCURY);
	m_material.gold = new PhysicsMaterial(friction, elasticity, eMaterial::GOLD);
	m_material.platinum = new PhysicsMaterial(friction, elasticity, eMaterial::PLATINUM);
	m_material.osmium = new PhysicsMaterial(friction, elasticity, eMaterial::OSMIUM);
	m_material.grippy = new PhysicsMaterial(1, 0.5f, eMaterial::MAGNESIUM);
	m_material.slippery = new PhysicsMaterial(0, 0.5f, eMaterial::MAGNESIUM);
	m_material.bouncy = new PhysicsMaterial(0.5f, 1, eMaterial::MAGNESIUM);
	m_material.dampened = new PhysicsMaterial(0.5f, 0.3f, eMaterial::MAGNESIUM);
	m_material.dead = new PhysicsMaterial(0, 0, eMaterial::MAGNESIUM);

}

void BasicPhysicsEngine::demoloop()
{
	//// BEWARE! Spaghetti prototype code

	aie::Input* input = aie::Input::getInstance();

	/////////////////
	//// MOUSE
	///////////////
	static struct {
		glm::ivec2 start;
		glm::ivec2 end;
		glm::ivec2 drag;
	} mouse;
	////Set mouse vars
	//If the mouse WAS pressed then record the start point
	if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		if (!ImGui::IsMouseHoveringAnyWindow())
		{
			input->getMouseXY(&mouse.start.x, &mouse.start.y);
		}
	}
	//If the mouse WAS released then record the mouse drag vector
	if (input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		mouse.drag = mouse.end - mouse.start;
	}

	//If the mouse is being held down then constantly set the mouse end point
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		input->getMouseXY(&mouse.end.x, &mouse.end.y);
	}	
	//if (input->isMouseButtonUp(aie::INPUT_MOUSE_BUTTON_LEFT))
	//{
	//	input->getMouseXY(&mouse.start.x, &mouse.start.y);
	//}


	///////////////
	//// GUI
	/////////////
	static int currentObjectType = 0; //0 Plane, 1 Circle, 2 AABB, 3 SAT
	static struct {
		//Physics Material
		float friction;
		float elasticity;
		float density;
		eMaterial material;

		//Kinematic
		bool isKinematic;

		//Other
		float genericSize1 = 1;
		float genericSize2 = 1;
	} GUI;
	static const char* materials[] = {
		"Hydrogen",
		"Air",
		"Styrofoam",
		"Lithium",
		"Wood",
		"Ice",
		"Water",
		"Plastics",
		"Diamond",
		"Copper",
		"Silver",
		"Lead",
		"Mercury",
		"Gold",
		"Platinum",
		"Osmium",

		"Sticky",
		"Slippery",
		"Bouncy",
		"Dampening"
	};

	ImGui::Begin("2D Physics Demo");
	{
		//Settings
		ImGui::BeginGroup();
		{
			ImGui::Text("Settings");
			ImGui::SliderFloat2("Gravity", glm::value_ptr(m_gravity), -500, 500);
			m_physicsScene->setGravity(m_gravity);
		}
		ImGui::EndGroup();
		ImGui::Spacing();
		ImGui::Spacing();

		//Create Physics Materials
		ImGui::BeginGroup();
		{
			ImGui::Text("Physics Materials");
			/* Layout
			- slider(0, 1): friction
			- slider(-10, 10): elasticity
			- CHOICE between:
				- combo list of preset materials
				- input: density "kg.m-3"
			*/
			ImGui::SliderFloat("Friction", &GUI.friction, -1, 1);
			ImGui::SliderFloat("Elasticity", &GUI.elasticity, -10, 10);

			ImGui::Text("Density");
			static int choice = 0;
			ImGui::RadioButton("Presets", &choice, 0);
			ImGui::RadioButton("Custom", &choice, 1);

			static int currentPreset = 17;
			switch (choice)
			{
			case 0:		//Presets

				ImGui::Combo("Presets", &currentPreset, materials, 15);
				break;
			case 1:		//Custom density
				ImGui::InputFloat("Density", &GUI.density, 10, 100);
				break;
			}
		}
		ImGui::EndGroup();
		ImGui::Spacing();
		ImGui::Spacing();

		//Create Objects
		ImGui::BeginGroup();
		{
			ImGui::Text("Create Objects");

			ImGui::RadioButton("Plane", &currentObjectType, 0); ImGui::SameLine();
			ImGui::RadioButton("Circle", &currentObjectType, 1); ImGui::SameLine();
			ImGui::RadioButton("AABB", &currentObjectType, 2); ImGui::SameLine();
			ImGui::RadioButton("SAT", &currentObjectType, 3);

			switch (currentObjectType)
			{
			case 0:	//Plane
				ImGui::TextWrapped("To create a plane, click and drag to set the plane normal, release to create.");
				//ImGui::text
				break;
			case 1:	//Circle
				ImGui::TextWrapped("To create a circle, click to spawn a random circle at cursor. Click and drag to spawn with velocity. Check Kinematic to create a kinematic object.");
				ImGui::Checkbox("Kinematic", &GUI.isKinematic);
				break;
			case 2:	//AABB
				ImGui::TextWrapped("To create a AABB, click to spawn a random AABB at cursor. Click and drag to spawn with velocity. Check Kinematic to create a kinematic object.");
				ImGui::Checkbox("Kinematic", &GUI.isKinematic);
				break;
			case 3:	//SAT
				ImGui::TextWrapped("To create a SAT, click to spawn a random SAT at cursor. Click and drag to spawn with velocity. Check Kinematic to create a kinematic object.");
				ImGui::Checkbox("Kinematic", &GUI.isKinematic);
				break;
			}
		}
		ImGui::EndGroup();
	}
	ImGui::End();
	
	/////////////////////
	//// Draw tool cursors
	/////////////////////
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
	} cursor;

	if (!ImGui::IsMouseHoveringAnyWindow()) {

		//Drag
		if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
			drawDragCursor(mouse.start, mouse.end, pkr::colour::get(pkr::eColours::COLOUR_DODGERBLUE));

		switch (currentObjectType)
		{
		case 0: //Plane
			//If mouse is in drag mode then draw normal and plane to create
			if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT)) 
			{
				drawPlaneSurfaceCursor(mouse.start, mouse.end, pkr::colour::get(pkr::eColours::COLOUR_ORANGE));
			}

			//If mouse release then create
			if (input->wasMouseButtonReleased(0)) {
				if (mouse.start != mouse.end) {
					//Make the plane
					m_physicsScene->AddActor(new Plane(mouse.start, mouse.end, m_material.gold));
				}
			}
			break;

		case 1: //Circle
			if (input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT))
			{
				//Temps
				glm::ivec2 pos = mouse.start;
				auto vel = mouse.drag * 5;
				float rot = 0;
				float radius = pkr::Random::range(20.f, 40.f);
				col colour = pkr::colour::nice_random();
				m_physicsScene->AddActor(new Circle(pos, vel, rot, radius, colour, m_material.bouncy, GUI.isKinematic));
			}
			break;

		case 2: //AABB
			if (input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT))
			{
				glm::ivec2 pos = mouse.start;
				auto vel = mouse.drag * 5;
				float rot = 0;
				float width = pkr::Random::range(40.f, 80.f);
				float height = pkr::Random::range(40.f, 80.f);
				col colour = pkr::colour::nice_random();
				m_physicsScene->AddActor(new AABB(pos, vel, width, height, colour, m_material.dampened, GUI.isKinematic));
			}
			break;

		case 3: //SAT
			if (input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT))
			{
				glm::ivec2 pos = mouse.start;
				auto vel = mouse.drag * 5;
				float rot = 0;
				float size = pkr::Random::range(40, 80);
				int sides = pkr::Random::range(3, 8);
				col colour = pkr::colour::nice_random();
				m_physicsScene->AddActor(new SAT(pos, vel, rot, size, sides, colour, m_material.osmium, GUI.isKinematic));
			}
			break;
		}
	}

}

void drawDragCursor(const glm::ivec2 & start, const glm::ivec2 & end, const glm::vec4 & colour)
{
	if (start != end) {
		aie::Gizmos::add2DLine(start, end, colour);
	}
}

void drawPlaneSurfaceCursor(const glm::ivec2 & normalStart, const glm::ivec2 & normalEnd, const glm::vec4 & colour)
{
	//if (normalStart != normalEnd)
	{
		float drawlength = 5000.f;
		vec2 planeNormal = vec2(normalEnd - normalStart);		//Get the normal
		vec2 plane = vec2(planeNormal.y, -planeNormal.x);	//Get the plane's surface parallel
		vec2 planeStart = static_cast<vec2>(normalStart) + (plane * drawlength);		//Get start point of cursor
		vec2 planeEnd = static_cast<vec2>(normalStart) - (plane * drawlength);		//Get end point of cursor
		aie::Gizmos::add2DLine(planeStart, planeEnd, colour);
	}
}

//if (mouseStart != mouseEnd) {
//	aie::Gizmos::add2DLine(mouseStart, mouseEnd, pkr::colour::get(pkr::eColours::COLOUR_RED));
//	paint.plane.normal = vec2(mouseEnd - mouseStart);
//	paint.plane.surface = vec2(paint.plane.normal.y, -paint.plane.normal.x);
//	paint.plane.start = static_cast<vec2>(mouseStart) + (paint.plane.surface * paint.plane.length);
//	paint.plane.end = static_cast<vec2>(mouseStart) - (paint.plane.surface * paint.plane.length);
//	aie::Gizmos::add2DLine(paint.plane.start, paint.plane.end, pkr::colour::get(pkr::eColours::COLOUR_ORANGE));
//}

//void drawDragCursor(glm::ivec2& start, glm::ivec2& end, col& colour) 
//{
//	if (start != end) {
//		aie::Gizmos::add2DLine(start, end, colour);
//	}
//}
//
//void drawPlaneSurfaceCursor(glm::ivec2& mouseStart, glm::ivec2& mouseEnd, col& colour) 
//{
//	if (mouseStart != mouseEnd)
//	{
//		float drawlength = 5000;
//		vec2 planeNormal = vec2(mouseEnd - mouseStart);		//Get the normal
//		vec2 plane = vec2(planeNormal.y, -planeNormal.x);	//Get the plane's surface parallel
//		vec2 planeStart = static_cast<vec2>(mouseStart) + (plane * drawlength);		//Get start point of cursor
//		vec2 planeEnd = static_cast<vec2>(mouseStart) + (plane * drawlength);		//Get end point of cursor
//		aie::Gizmos::add2DLine(planeStart, planeEnd, colour);
//	}
//}

//////////////////
// Paint Gizmo Definitions
//////////////////
//
//void PaintGizmo::drawMouseDragLine(const vec2& mouseStart, const vec2& mouseEnd, int mouseButton)
//{
//	if (m_input->isMouseButtonDown(mouseButton))
//	{
//		if (mouseStart != mouseEnd)
//		{
//			aie::Gizmos::add2DLine(mouseStart, mouseEnd, pkr::colour::get(pkr::eColours::COLOUR_RED));
//		}
//	}
//}
//
//void PaintGizmo::drawBlueprintPlaneSurface(const vec2 & mouseStart, const vec2 & mouseEnd) const
//{
//	if (mouseLHold) {
//		if (mouseStart != mouseEnd) {
//			aie::Gizmos::add2DLine(mouseStart, mouseEnd, pkr::colour::get(pkr::eColours::COLOUR_RED));
//			paint.plane.normal = vec2(mouseEnd - mouseStart);
//			paint.plane.surface = vec2(paint.plane.normal.y, -paint.plane.normal.x);
//			paint.plane.start = static_cast<vec2>(mouseStart) + (paint.plane.surface * paint.plane.length);
//			paint.plane.end = static_cast<vec2>(mouseStart) - (paint.plane.surface * paint.plane.length);
//			aie::Gizmos::add2DLine(paint.plane.start, paint.plane.end, pkr::colour::get(pkr::eColours::COLOUR_ORANGE));
//		}
//	}
//}

