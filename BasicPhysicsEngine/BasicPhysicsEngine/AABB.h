#pragma once
#include "RigidBody.h"

#include <glm/ext.hpp>
#include "GameDefines.h"
#include <vector>

class vector;

class AABB : public RigidBody
{
protected:
	glm::vec2	m_extents;	//Top right half extents
	glm::vec4	m_colour;
public:
	AABB(glm::vec2 position, glm::vec2 velocity, float mass, float width, float height, glm::vec4 colour);
	~AABB();

	void		DrawGizmo() override;
	//bool		CheckCollision(PhysicsObject* other) override;

	//void		FixedUpdate(glm::vec2 gravity, float timeStep) override {}
	//void		Debug() override {}

	glm::vec2				min() const { return m_position - m_extents; }	//Returns the min absolute point
	glm::vec2				max() const { return m_position + m_extents; }	//Returns the max absolute point
	
	std::vector<glm::vec2>	vertices() const;	//Returns an array of all vertices starting from min
};

