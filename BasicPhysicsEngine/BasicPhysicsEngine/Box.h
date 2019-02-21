#pragma once
#include "RigidBody.h"
#include <glm/ext.hpp>

#include "GameDefines.h"

class Box : public RigidBody
{
protected:
	glm::vec2	m_extents;	//Top right half extents
	glm::vec4	m_colour;
public:
	Box(glm::vec2 position, glm::vec2 velocity, float mass, float width, float height, glm::vec4 colour);
	~Box();

	void		MakeGizmo() override;
	//bool		CheckCollision(PhysicsObject* other) override;

	//void		FixedUpdate(glm::vec2 gravity, float timeStep) override {}
	//void		Debug() override {}

	glm::vec2	getMin() const;
	glm::vec2	getMax() const;
	glm::vec4	getColour() const { return m_colour; }
};

