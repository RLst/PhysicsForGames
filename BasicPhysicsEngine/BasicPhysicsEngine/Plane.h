#pragma once
#include "PhysicsObject.h"

//enum ePlaneResult : int {
//	FRONT = 1,
//	BACK = -1,
//	INTERSECTS = 0
//};

enum ePerpDirection
{
	LEFT,
	RIGHT 
};

class RigidBody;
class PhysicsMaterial;

class Plane : public PhysicsObject
{
protected:
	vec2		m_normal;
	float		m_distanceToOrigin;
	
	float		m_friction;
	float		m_elasticity;

public:
	Plane();	//Default at ground zero facing up
	Plane(const vec2& normal, float distance, float elasticity = 0.9f);		//normal + dist
	Plane(float x, float y, float distance, float elasticity = 0.9f);		//ax2 + by + dist
	Plane(const vec2& point1, const vec2& point2, ePerpDirection pdir = LEFT, float elasticity = 0.9f);		//2 points

	//Physics Material constructors
	Plane(const vec2& normalStart, const vec2& normalEnd, PhysicsMaterial* material);
	~Plane();

	void		FixedUpdate(const vec2& gravity, float timeStep) override {};
	void		Debug() override {}
	void		DrawGizmo() override;
	void		ResolveCollision(RigidBody* other);

	vec2		normal() { return m_normal; }
	float		distance() { return m_distanceToOrigin; }

	float		distanceTo(const vec2& point) const;
	vec2		closestPoint(const vec2& point) const;

	//Extra functionality from tutorial for double sided plane
	//ePlaneResult testSide(const glm::vec2& point) const;
};

