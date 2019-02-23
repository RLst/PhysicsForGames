#pragma once
#include "RigidBody.h"
#include <glm/ext.hpp>

using glm::vec2;
using glm::vec4;

class Circle : public RigidBody
{
protected:
	float		m_radius;
	vec4		m_colour;

public:
	Circle(vec2 position, vec2 velocity, float mass, float radius, vec4 colour);
	~Circle();

	void		DrawGizmo() override;

	float		radius() const { return m_radius; }
	vec4		colour() const { return m_colour; }
};

