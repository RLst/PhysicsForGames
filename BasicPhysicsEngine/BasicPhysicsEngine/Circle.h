#pragma once
#include "RigidBody.h"

class PhysicsMaterial;

class Circle : public RigidBody
{
protected:
	float				m_radius;
	vec4				m_colour;

public:
	Circle(const vec2& pos, const vec2& vel, float rot, float radius, const vec4& colour, float mass, float friction, float elasticity, bool isKinematic = false);
	Circle(const vec2& pos, const vec2& vel, float rot, float radius, const vec4& colour, PhysicsMaterial* material, bool isKinematic = false);
	~Circle();

	void		DrawGizmo() override;

	float		radius() const { return m_radius; }
	vec4		colour() const { return m_colour; }
};

