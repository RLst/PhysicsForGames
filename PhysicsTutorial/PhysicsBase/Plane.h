#pragma once
#include "PhysicsObject.h"
#include <glm/ext.hpp>

#include "GameDefines.h"

class RigidBody;

class Plane : public PhysicsObject
{
protected:
	vec2 m_normal;
	float m_distanceToOrigin;

public:
	Plane();	//Default at ground zero
	Plane(glm::vec2 normal, float distance);
	~Plane();

	void		FixedUpdate(glm::vec2 gravity, float timeStep) override {};
	void		Debug() override;
	void		MakeGizmo() override;
	void		ResolveCollision(RigidBody* other);

	vec2		GetNormal() { return m_normal; }
	float		GetDistance() { return m_distanceToOrigin; }
};

