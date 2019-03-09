#include "Circle.h"
#include <Gizmos.h>
#include "PhysicsMaterial.h"

Circle::Circle(const vec2 & pos, const vec2 & vel, float rot, float radius, vec4 colour, PhysicsMaterial* material, bool isKinematic) :
	RigidBody(eShapeType::CIRCLE, pos, vel, rot, isKinematic, material),
	m_radius(radius), m_colour(colour)
{
	//Mass
	m_mass = PI * radius * radius * m_material->getDensity();

	//MOI
	m_moment = 0.5f * m_mass * radius * radius;

}

Circle::~Circle()
{
}

void Circle::DrawGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 20, m_colour);
}