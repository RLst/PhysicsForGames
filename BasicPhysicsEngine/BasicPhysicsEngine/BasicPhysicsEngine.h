#pragma once

#include "Application.h"
#include "Renderer2D.h"

#include <glm/ext.hpp>

//Should forward declare pointers, not #include headers
class PhysicsScene;
class Circle;
class Plane;
class PhysicsMaterial;

using glm::vec2;

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
	void demoloop();

	//Utilities
	float calcMass(float radius, float density);					//Calc mass for circles
	float calcMass(float width, float height, float density);		//Calc mass for AABBs
	void tempStartup();

protected:
	//The smaller the time step, the more accurate the numerical integration solution will be
	float				fixedTimeStep = 0.02f;			//Physics engine time step		
	vec2				 m_gravity = vec2(0, -98.1f);	//9.81 too slow

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene*		m_physicsScene;

	PhysicsMaterial*	mat_wooden;
	PhysicsMaterial*	mat_metal;
	PhysicsMaterial*	mat_bouncy;
	PhysicsMaterial*	mat_slippery;
};

/* References
- https://gamedevelopment.tutsplus.com/tutorials/collision-detection-using-the-separating-axis-theorem--gamedev-169
- http://www.dyn4j.org/2010/01/sat/


*/