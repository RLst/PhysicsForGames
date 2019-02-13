#pragma once

#include "Application.h"
#include "Renderer2D.h"

#include <glm/ext.hpp>
#include <vector>

//Should forward declare pointers, not #include headers
class PhysicsScene;
class Circle;
class Plane;

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
};