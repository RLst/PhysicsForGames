#include "Box.h"

#include <Gizmos.h>

Box::Box(glm::vec2 position, glm::vec2 velocity, float mass, float width, float height, glm::vec4 colour) :
	RigidBody(ShapeType::BOX, position, velocity, 0, mass),
	m_colour(colour)
{
	m_extents = vec2(width / 2.f, height / 2.f);
}

Box::~Box()
{
}

void Box::MakeGizmo()
{
	aie::Gizmos::add2DAABBFilled(m_position, m_extents, m_colour);	//TODO &extents == m_max???
}

//bool Box::CheckCollision(PhysicsObject * other)
//{
//	return false;
//}

glm::vec2 Box::getMin() const
{
	return m_position - m_extents;
}

glm::vec2 Box::getMax() const
{
	return m_position + m_extents;
}
