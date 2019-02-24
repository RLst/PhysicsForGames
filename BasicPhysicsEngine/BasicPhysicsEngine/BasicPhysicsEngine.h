#pragma once

#include "Application.h"
#include "Renderer2D.h"

#include <glm/ext.hpp>
#include <vector>

//Should forward declare pointers, not #include headers
class PhysicsScene;
class Circle;
class Plane;

using glm::vec2;

class BasicPhysicsEngine : public aie::Application {
public:

	BasicPhysicsEngine();
	virtual ~BasicPhysicsEngine();

	bool startup() override;
	void shutdown() override;

	void update(float deltaTime) override;
	void draw() override;

	//Tutorials
	void SetupContinuousDemo(vec2 startPos, float inclination, float speed, float gravity);

	//Utilities
	float calcMass(float radius, float density);					//Calc mass for circles
	float calcMass(float width, float height, float density);		//Calc mass for AABBs

	//Tests
	void Demo(float gravity);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene*		m_physicsScene;

	//The smaller the time step, the more accurate the numerical integration solution will be
	float fixedTimeStep = 0.02f;		//Physics engine time step		
	float m_gravity = -0.0f;
};