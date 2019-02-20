#pragma once

#include "Application.h"
#include "Renderer2D.h"

#include <glm/ext.hpp>
#include <vector>

//Should forward declare pointers, not #include headers
class PhysicsScene;
class Circle;
class Plane;

class BasicPhysicsEngine : public aie::Application {
public:

	BasicPhysicsEngine();
	virtual ~BasicPhysicsEngine();

	bool startup() override;
	void shutdown() override;

	void update(float deltaTime) override;
	void draw() override;

	//Tutorials
	void SetupContinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity);

	//Tests
	void Circle2CircleTest();
	void Circle2PlaneTest();
	void BilliardBallSimulation();
	void AABBTest();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	//The smaller the time step, the more accurate the numerical integration solution will be
	float fixedTimeStep = 0.02f;		//Physics engine time step		
};