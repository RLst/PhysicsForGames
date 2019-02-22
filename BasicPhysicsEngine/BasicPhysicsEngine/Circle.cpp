#include "Circle.h"
#include <glm/ext.hpp>
#include <Gizmos.h>

Circle::Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) :
	RigidBody(eShapeType::CIRCLE, position, velocity, 0, mass),
	m_radius(radius),
	m_colour(colour)
{
}

Circle::~Circle()
{
}

void Circle::MakeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 22, m_colour);
}

//bool Circle::CheckCollision(PhysicsObject * other)	//TODO Should this still be used???
//{
//	//For now only 2 spheres can collide together
//
//	//Cast (which one is better?)
//	auto otherC = (Circle*)other;
//	//auto otherD = dynamic_cast<Circle*>(other);
//
//	auto distanceBetween = glm::distance(m_position, otherC->m_position);
//
//	//Do the actual collision
//	///If distance is less than the combined radius of both spheres
//	if (distanceBetween < (m_radius + otherC->m_radius))
//	{
//		return true;	//Collision occured!
//	}
//	return false;
//}
