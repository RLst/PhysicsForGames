#pragma once
#include "RigidBody.h"

class Circle : public RigidBody
{
protected:
	float		m_radius;
	vec4		m_colour;

public:
	Circle(vec2 position, vec2 velocity, float mass, float radius, vec4 colour, float elasticity = 1.0f);
	~Circle();

	void		DrawGizmo() override;

	float		radius() const { return m_radius; }
	vec4		colour() const { return m_colour; }
};

