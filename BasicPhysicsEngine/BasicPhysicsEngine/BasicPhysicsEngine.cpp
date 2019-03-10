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
	m_material.names = "Hydrogen\0Air\0Styrofoam\0Lithium\0Wood\0Ice\0Water\0Plastics\0Diamond\0Copper\0Silver\0Lead\0Mercury\0Gold\0Platinum\0Osmium\0Grippy\0Slippery\0Bouncy\0Dampened\0Dead";
}

BasicPhysicsEngine::~BasicPhysicsEngine() {
}

//const char* BasicPhysicsEngine::m_material

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
	m_physicsScene = new PhysicsScene(m_fixedTimeStep, m_gravity);
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
	playground();

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

bool BasicPhysicsEngine::createMaterials()
{
	float friction = 0.5f;
	float elasticity = 0.5f;
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

	m_material.custom = new PhysicsMaterial(friction, elasticity, 1000);

	return true;
}

void BasicPhysicsEngine::playground()
{
	//// BEWARE! Spaghetti prototype code

	aie::Input* input = aie::Input::getInstance();

	/////////////////
	//// MOUSE
	///////////////
	static struct {
		int velocityFineTune = 5;
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

	///////////////
	//// GUI
	/////////////
	static int currentObjectType = 0; //0 Plane, 1 Circle, 2 AABB, 3 SAT
	static struct {
		int choiceDensity = 0;	//0 Presets, 1 Custom
		int choiceColour = 0;	//0 Presets, 1 Custom

		//Physics Material
		float friction = 0.4f;
		float elasticity = 0.9f;
		float density = 1000;
		PhysicsMaterial* material /*= new PhysicsMaterial(friction, elasticity, eMaterial::NIL)*/;

		//Colour
		vec4 colour = vec4(1,1,1,1);

		//Creation
		bool isKinematic = false;
		float genericfloat1 = 25;
		float genericfloat2 = 50;
		int genericInt1 = 3;
	} GUI;

	ImGui::Begin("Physics Playground"/*, &test, ImGuiWindowFlags_AlwaysAutoResize*/);
	{
		//Settings
		ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
		ImGui::BeginChild("World", ImVec2(0, 55), true);
		{
			ImGui::Text("General");
			ImGui::SliderFloat2("Gravity", glm::value_ptr(m_gravity), -500, 500);
			m_physicsScene->setGravity(m_gravity);
		}
		ImGui::EndChild();

		//Physics Materials
		ImGui::BeginChild("PhysicsMaterials", ImVec2(0, 119), true);
		{
			static int currentPreset = 0;

			ImGui::Text("Physics Materials");
			ImGui::RadioButton("Presets", &GUI.choiceDensity, 0); ImGui::SameLine();
			ImGui::RadioButton("Custom", &GUI.choiceDensity, 1);

			switch (GUI.choiceDensity)
			{
			case 0:		//Presets
				ImGui::Combo("Presets", &currentPreset, m_material.names, m_material.count);
				ImGui::SameLine(); ImGui::Text("(%i)", currentPreset);
				switch (currentPreset)
				{
				case 0: GUI.material = m_material.hydrogen; break;
				case 1: GUI.material = m_material.air; break;
				case 2: GUI.material = m_material.styrofoam; break;
				case 3: GUI.material = m_material.lithium; break;
				case 4: GUI.material = m_material.wood; break;
				case 5: GUI.material = m_material.ice; break;
				case 6: GUI.material = m_material.water; break;
				case 7: GUI.material = m_material.plastics; break;
				case 8: GUI.material = m_material.diamond; break;
				case 9: GUI.material = m_material.copper; break;
				case 10: GUI.material = m_material.silver; break;
				case 11: GUI.material = m_material.lead; break;
				case 12: GUI.material = m_material.mercury; break;
				case 13: GUI.material = m_material.gold; break;
				case 14: GUI.material = m_material.platinum; break;
				case 15: GUI.material = m_material.osmium; break;
				case 16: GUI.material = m_material.grippy; break;
				case 17: GUI.material = m_material.slippery; break;
				case 18: GUI.material = m_material.bouncy; break;
				case 19: GUI.material = m_material.dampened; break;
				case 20: GUI.material = m_material.dead; break;
				//case 0: m_material.main->setMaterial(eMaterial::HYDROGEN); break;
				//case 1: m_material.main->setMaterial(eMaterial::STYROFOAM); break;
				//case 2: m_material.main->setMaterial(eMaterial::LITHIUM); break;
				//case 3: m_material.main->setMaterial(eMaterial::WOOD); break;
				//case 4: m_material.main->setMaterial(eMaterial::ICE); break;
				//case 5: m_material.main->setMaterial(eMaterial::WATER); break;
				//case 6: m_material.main->setMaterial(eMaterial::PLASTICS); break;
				//case 7: m_material.main->setMaterial(eMaterial::DIAMOND); break;
				//case 8: m_material.main->setMaterial(eMaterial::COPPER); break;
				//case 9: m_material.main->setMaterial(eMaterial::SILVER); break;
				//case 10: m_material.main->setMaterial(eMaterial::LEAD); break;
				//case 11: m_material.main->setMaterial(eMaterial::MERCURY); break;
				//case 12: m_material.main->setMaterial(eMaterial::GOLD); break;
				//case 13: m_material.main->setMaterial(eMaterial::PLATINUM); break;
				//case 14: m_material.main->setMaterial(eMaterial::OSMIUM); break;
				default: assert(false);	//Invalid!!!
				}

				//Point the final material appropriately
				m_material.final = GUI.material;
				break;
			case 1:		//Custom density
				ImGui::SliderFloat("Friction", &GUI.friction, 0, 1);
				ImGui::SliderFloat("Elasticity", &GUI.elasticity, 0, 1);
				ImGui::InputFloat("Density", &GUI.density, 10, 200);
				
				//Set custom material
				m_material.custom->friction = GUI.friction;
				m_material.custom->elasticity = GUI.elasticity;
				m_material.custom->setDensity(GUI.density);

				//Point the final material appropriately
				m_material.final = m_material.custom;
				break;
			}
		}
		ImGui::EndChild();

		//Colours
		ImGui::BeginChild("Colour", ImVec2(0, 77), true);
		{
			static int currentPreset = 0;
			static const char* colour_names= "White\0Red\0Green\0Blue\0Cyan\0Magenta\0Yellow\0Orange\0Lime Green\0Mint\0Dodge Blue\0Indigo\0Purple\0Fuschia\0Grey";

			ImGui::Text("Colour");
			ImGui::RadioButton("Presets", &GUI.choiceColour, 0); ImGui::SameLine();
			ImGui::RadioButton("Custom", &GUI.choiceColour, 1);

			switch (GUI.choiceColour)
			{
			case 0:	//Presets
				ImGui::Combo("Presets", &currentPreset, colour_names, static_cast<int>(pkr::eColours::COLOUR_COUNT));
				GUI.colour = pkr::colour::get(static_cast<pkr::eColours>(currentPreset));
				break;
			case 1:	//Custom
				ImGui::SliderFloat4("RGBA", glm::value_ptr(GUI.colour), 0, 1);
				break;
			}
		}
		ImGui::EndChild();

		//Create
		ImGui::BeginChild("Create", ImVec2(0, 177), true);
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
				ImGui::TextWrapped("Click to spawn a Circle at cursor. Click and drag to spawn with velocity. Check Kinematic to create a kinematic object.");
				ImGui::Checkbox("Kinematic", &GUI.isKinematic);
				ImGui::SliderFloat("Diameter", &GUI.genericfloat1, 10, 100);
				break;
			case 2:	//AABB
				ImGui::TextWrapped("Click to spawn a AABB at cursor. Click and drag to spawn with velocity. Check Kinematic to create a kinematic object.");
				ImGui::Checkbox("Kinematic", &GUI.isKinematic);
				ImGui::SliderFloat("Width", &GUI.genericfloat1, 10, 100);
				ImGui::SliderFloat("Height", &GUI.genericfloat2, 10, 100);
				break;
			case 3:	//SAT
				ImGui::TextWrapped("Click to spawn a SAT at cursor. Click and drag to spawn with velocity. Check Kinematic to create a kinematic object.");
				ImGui::Checkbox("Kinematic", &GUI.isKinematic);
				ImGui::SliderFloat("Size", &GUI.genericfloat1, 10, 100);
				ImGui::SliderInt("Sides", &GUI.genericInt1, 3, 12);
				break;
			}
		}
		ImGui::EndChild();
		ImGui::PopStyleVar();
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

	if (!ImGui::IsMouseHoveringAnyWindow()) {	//Prevents unintended objects being created

		//Drag
		if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
			drawDragCursor(mouse.start, mouse.end, pkr::colour::get(pkr::eColours::RED));

		switch (currentObjectType)
		{
		case 0: //Plane
			//If mouse is in drag mode then draw normal and plane to create
			if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
				drawPlaneSurfaceCursor(mouse.start, mouse.end, pkr::colour::get(pkr::eColours::ORANGE));

			//If mouse release then create
			if (input->wasMouseButtonReleased(0)) {
				if (mouse.start != mouse.end) {
					//Make the plane
					m_physicsScene->AddActor(new Plane(static_cast<vec2>(mouse.start), static_cast<vec2>(mouse.end), GUI.colour, m_material.final));
				}
			}
			break;

		case 1: //Circle
			if (input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT))
			{
				auto vel = mouse.drag * mouse.velocityFineTune;
				m_physicsScene->AddActor(new Circle(mouse.start, vel, 0, GUI.genericfloat1 * 0.5f, GUI.colour, m_material.final, GUI.isKinematic));
			}
			break;

		case 2: //AABB
			if (input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT))
			{
				auto vel = mouse.drag * mouse.velocityFineTune;
				m_physicsScene->AddActor(new AABB(mouse.start, vel, GUI.genericfloat1, GUI.genericfloat2, GUI.colour, m_material.final, GUI.isKinematic));
			}
			break;

		case 3: //SAT
			if (input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT))
			{
				auto vel = mouse.drag * mouse.velocityFineTune;
				m_physicsScene->AddActor(new SAT(mouse.start, vel, 0, GUI.genericfloat1, GUI.genericInt1, GUI.colour, m_material.final, GUI.isKinematic));
			}
			break;
		}
	}

}

///////////////////////////////////
//// Playground only functions
////////////////////////////////
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
