#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "PhysicsObject.h"

#include "Circle.h"

class PhysicsBaseApp : public aie::Application {
public:

	PhysicsBaseApp();
	virtual ~PhysicsBaseApp();

	bool startup() override;
	void shutdown() override;

	void update(float deltaTime) override;
	void draw() override;

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	Circle *ballSml, *ballMed, *ballLge;
};