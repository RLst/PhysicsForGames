#include "Box.h"

#include <Gizmos.h>

Box::Box(glm::vec2 position, glm::vec2 velocity, float mass, float width, float height, glm::vec4 colour) :
	RigidBody(ShapeType::BOX, position, velocity, 0, mass),
	m_colour(colour)
{
	//Calc min and max values
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	m_min.x = position.x - halfWidth;
	m_min.y = position.y - halfHeight;
	m_max.x = position.x + halfWidth;
	m_max.y = position.y + halfHeight;
}

Box::~Box()
{
}

void Box::MakeGizmo()
{
	aie::Gizmos::add2DAABBFilled(m_position, m_max, m_colour);	//TODO &extents == m_max???
}

//bool Box::CheckCollision(PhysicsObject * other)
//{
//	return false;
//}
