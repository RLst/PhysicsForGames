#include "AABB.h"

#include <Gizmos.h>

AABB::AABB(glm::vec2 position, glm::vec2 velocity, float mass, float width, float height, glm::vec4 colour) :
	RigidBody(eShapeType::BOX, position, velocity, 0, mass),
	m_colour(colour)
{
	m_extents = vec2(width / 2.f, height / 2.f);
}

AABB::~AABB()
{
}

void AABB::DrawGizmo()
{
	aie::Gizmos::add2DAABBFilled(m_position, m_extents, m_colour);	//TODO &extents == m_max???
}

//bool AABB::CheckCollision(PhysicsObject * other)
//{
//	return false;
//}

std::vector<glm::vec2> AABB::vertices() const
{
	std::vector<glm::vec2> corners(4);
	corners[0] = min();
	corners[1] = { min().x, max().y };
	corners[2] = max();
	corners[3] = { max().x, min().y };
	return corners;
}
