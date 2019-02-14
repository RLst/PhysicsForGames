#include "Plane.h"
#include <Gizmos.h>
#include "RigidBody.h"

Plane::Plane() :
	PhysicsObject(ShapeType::PLANE),
	m_distanceToOrigin(0),
	m_normal(glm::vec2(0, 1))	//Ground zero facing up
{
}

Plane::Plane(glm::vec2 normal, float distance) :
	PhysicsObject(ShapeType::PLANE),
	m_normal(normal),
	m_distanceToOrigin(distance)
{
}

Plane::~Plane()
{
}


void Plane::MakeGizmo()
{
	float lineSegmentLength = 1500;
	vec2 centerPoint = m_normal * m_distanceToOrigin;
		//Easy to rotate normal through 90 degrees around z
	vec2 parallel(m_normal.y, -m_normal.x);
	col colour(1, 1, 1, 1);
	vec2 start = centerPoint + (parallel * lineSegmentLength);
	vec2 end = centerPoint - (parallel * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, colour);
}

void Plane::ResolveCollision(RigidBody * other)
{
	//glm::vec2 normal = glm::normalize(other->getPosition() - m_position);
	glm::vec2 relVelocity = other->getVelocity();

	//Super formula (impulse magnitude)
	float elasticity = 0.99f;		//TODO
	float j = glm::dot(-(1 + elasticity) * (relVelocity), m_normal) / (1 / other->getMass());

	glm::vec2 force = m_normal * j;

	other->ApplyForce(force);
}
