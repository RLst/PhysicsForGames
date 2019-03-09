#pragma once
#include "PhysicsObject.h"
#include "PhysicsMaterial.h"

class RigidBody;

enum ePerpDirection
{
	LEFT,
	RIGHT 
};

class Plane : public PhysicsObject
{
protected:
	vec2				m_normal;
	float				m_distanceToOrigin;
	vec4				m_colour = vec4(1,1,1,1);
	PhysicsMaterial*	m_material = new PhysicsMaterial(0.4f, 0.9f, 1000);

public:
	Plane();													//Default at ground zero facing up
	Plane(const vec2& normal, float distance);										//normal + dist
	Plane(float x, float y, float distance);										//ax2 + by + dist
	Plane(const vec2& point1, const vec2& point2, ePerpDirection pdir = LEFT);		//2 points
	Plane(const vec2& normalStart, const vec2& normalEnd, const vec4& colour, float friction = 0.4f, float elasticity = 0.9f);
	Plane(const vec2& normalStart, const vec2& normalEnd, const vec4& colour, PhysicsMaterial* material);
	~Plane();

	void		FixedUpdate(const vec2& gravity, float timeStep) override {};
	void		Debug() override {}
	void		DrawGizmo() override;
	void		ResolveCollision(RigidBody* other);

	vec2		normal() { return m_normal; }
	float		distance() { return m_distanceToOrigin; }

	float		distanceTo(const vec2& point) const;
	vec2		closestPoint(const vec2& point) const;
};

