#pragma once
#include "PhysicsObject.h"
#include <glm/ext.hpp>

#include "GameDefines.h"

enum ePlaneResult : int {
	FRONT = 1,
	BACK = -1,
	INTERSECTS = 0
};

class RigidBody;

class Plane : public PhysicsObject
{
protected:
	vec2		m_normal;
	float		m_distanceToOrigin;

	float		m_elasticity;

public:
	Plane();	//Default at ground zero facing up
	Plane(float x, float y, float distance, float elasticity = 0.9f);
	Plane(const glm::vec2 normal, float distance, float elasticity = 0.9f);
	Plane(const glm::vec2 point1, const glm::vec2 point2, float elasticity = 0.9f);
	~Plane();

	void		FixedUpdate(glm::vec2 gravity, float timeStep) override {};
	void		Debug() override {}
	void		MakeGizmo() override;
	void		ResolveCollision(RigidBody* other);

	vec2		normal() { return m_normal; }
	float		distance() { return m_distanceToOrigin; }

	float		distanceTo(const glm::vec2& point) const;
	glm::vec2	closestPoint(const glm::vec2& point) const;

	ePlaneResult testSide(const glm::vec2& point) const;		//Different style from glm
};

