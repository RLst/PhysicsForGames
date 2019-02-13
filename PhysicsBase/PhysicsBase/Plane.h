#pragma once
#include "PhysicsObject.h"
#include <glm/ext.hpp>

#include "GameDefines.h"

class Plane : public PhysicsObject
{
protected:
	vec2 m_normal;
	float m_distanceToOrigin;

private:
	//For ResetPosition (OK?)
	vec2 m_orig_normal;
	float m_orig_distanceToOrigin;

public:
	Plane() = default;
	Plane(glm::vec2 normal, float distance);
	//Plane(glm::vec2 normal, )
	~Plane();

	void FixedUpdate(glm::vec2 gravity, float timeStep) override {};
	void Debug() override;
	void MakeGizmo() override;
	void ResetPosition() override;

	vec2		GetNormal() { return m_normal; }
	float		GetDistance() { return m_distanceToOrigin; }
};

