#include "Circle.h"
#include <glm/ext.hpp>
#include <Gizmos.h>

Circle::Circle(vec2 position, vec2 velocity, float mass, float radius, vec4 colour) :
	RigidBody(eShapeType::CIRCLE, position, velocity, 0, mass),
	m_radius(radius),
	m_colour(colour)
{
}

Circle::~Circle()
{
}

void Circle::DrawGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 20, m_colour);
}