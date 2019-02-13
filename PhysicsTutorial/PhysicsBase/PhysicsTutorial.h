#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "PhysicsObject.h"

#include "Circle.h"
#include <vector>

class PhysicsTutorial : public aie::Application {
public:

	PhysicsTutorial();
	virtual ~PhysicsTutorial();

	bool startup() override;
	void shutdown() override;

	void update(float deltaTime) override;
	void draw() override;

	//Tutorials
	void setupContinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	//Circle *ballSml, *ballMed, *ballLge;
	
	//Rocket test
	//float fuelMass;
	//float fuelTankMass;		//in kgs
	//float rocketMass;
	//Circle *rocket;
	//std::vector<Circle*> particles;
};