#include "Circle.h"
#include <Gizmos.h>
#include "PhysicsMaterial.h"

Circle::Circle(const vec2& pos, const vec2& vel, float rot, float radius, const vec4& colour, float mass, float friction, float elasticity, bool isKinematic) :
	RigidBody(eShapeType::CIRCLE, pos, vel, rot, isKinematic, new PhysicsMaterial(friction, elasticity, eMaterial::NIL), mass),
	m_radius(radius), m_colour(colour)
{
	//MOI
	m_moment = 0.5f * m_mass * radius * radius;
}

Circle::Circle(const vec2 & pos, const vec2 & vel, float rot, float radius, const vec4& colour, PhysicsMaterial* material, bool isKinematic) :
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
	unsigned int segments = 20;
	aie::Gizmos::add2DCircle(m_position, m_radius, segments, m_colour);
}