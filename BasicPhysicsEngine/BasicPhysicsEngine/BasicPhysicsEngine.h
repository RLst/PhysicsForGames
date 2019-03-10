#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include <glm/ext.hpp>
#include <string>

//Should forward declare pointers, not #include headers
class PhysicsScene;
class Circle;
class Plane;
class PhysicsMaterial;

using glm::vec2;

/* References
- https://gamedevelopment.tutsplus.com/tutorials/collision-detection-using-the-separating-axis-theorem--gamedev-169
- http://www.dyn4j.org/2010/01/sat/
*/

class BasicPhysicsEngine : public aie::Application {
public:

	BasicPhysicsEngine();
	virtual ~BasicPhysicsEngine();

	bool startup() override;
	void shutdown() override;

	void update(float deltaTime) override;
	void draw() override;

	//Demos
	void createMaterials();
	void playground();

	//Utilities
	float calcMass(float radius, float density);					//Calc mass for circles
	float calcMass(float width, float height, float density);		//Calc mass for AABBs

protected:
	//The smaller the time step, the more accurate the numerical integration solution will be
	float				fixedTimeStep = 0.02f;			//Physics engine time step		
	vec2				m_gravity = vec2(0, -98.1f);	//9.81 too slow

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene*		m_physicsScene;

	struct {
		PhysicsMaterial* hydrogen;
		PhysicsMaterial* air;
		PhysicsMaterial* styrofoam;
		PhysicsMaterial* lithium;
		PhysicsMaterial* wood;
		PhysicsMaterial* ice;
		PhysicsMaterial* water;
		PhysicsMaterial* plastics;
		PhysicsMaterial* diamond;
		PhysicsMaterial* copper;
		PhysicsMaterial* silver;
		PhysicsMaterial* lead;
		PhysicsMaterial* mercury;
		PhysicsMaterial* gold;
		PhysicsMaterial* platinum;
		PhysicsMaterial* osmium;

		PhysicsMaterial* grippy;
		PhysicsMaterial* slippery;
		PhysicsMaterial* bouncy;
		PhysicsMaterial* dampened;
		PhysicsMaterial* dead;
		unsigned int	count = 21;
		
		const char* names;
		PhysicsMaterial* custom;
		PhysicsMaterial* final;	//Points to the material to be used when creating objects
	} m_material;
};

//Playground assist functions
void drawDragCursor(const glm::ivec2 & start, const glm::ivec2 & end, const glm::vec4 & colour);
void drawPlaneSurfaceCursor(const glm::ivec2 & normalStart, const glm::ivec2 & normalEnd, const glm::vec4 & colour);