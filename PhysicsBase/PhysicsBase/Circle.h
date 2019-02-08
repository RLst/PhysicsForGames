#pragma once
#include "RigidBody.h"

class Circle : public RigidBody
{
protected:
	float		m_radius;
	glm::vec4	m_colour;

public:
	Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour);
	~Circle();

	void		MakeGizmo() override;
	bool		CheckCollision(PhysicsObject* other) override;

	float		getRadius() { return m_radius; }
	glm::vec4	getColour() { return m_colour; }
};

