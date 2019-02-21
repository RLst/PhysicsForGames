#include "Plane.h"
#include <Gizmos.h>
#include "RigidBody.h"

Plane::Plane() :
	PhysicsObject(ShapeType::PLANE),
	m_distanceToOrigin(0),
	m_normal(glm::vec2(0, 1)),	//Ground zero facing up
	m_elasticity(0.9f)
{
}

Plane::Plane(float x, float y, float distance, float elasticity) :
	PhysicsObject(ShapeType::PLANE),
	m_normal(x, y),
	m_distanceToOrigin(distance),
	m_elasticity(elasticity)
{
}

Plane::Plane(const glm::vec2 normal, float distance, float elasticity) :
	PhysicsObject(ShapeType::PLANE),
	m_normal(normal),
	m_distanceToOrigin(distance),
	m_elasticity(elasticity)
{
}

Plane::Plane(const glm::vec2 point1, const glm::vec2 point2, float elasticity) :
	PhysicsObject(ShapeType::PLANE)
{
	//Calculate normalised vector from point0 to point1
	auto v = point2 - point1;
	v = glm::normalize(v);

	//Set normal perpendicular to the vector
	m_normal.x = -v.x;
	m_normal.y = v.x;

	//Calculate distance
	m_distanceToOrigin = glm::dot(point1, m_normal);
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
	//glm::vec2 normal = glm::normalize(other->position() - m_position);
	glm::vec2 relVelocity = other->velocity();

	//Super formula (impulse magnitude)
	float elasticity = (m_elasticity + other->getElasticity()) / 2.0f;
	float j = glm::dot(-(1 + elasticity) * (relVelocity), m_normal) / (1 / other->mass());

	glm::vec2 force = m_normal * j;

	other->ApplyForce(force);
}

float Plane::distanceTo(const glm::vec2 & point) const
{
	return (glm::dot(point, m_normal) + m_distanceToOrigin);
}

glm::vec2 Plane::closestPoint(const glm::vec2 & point) const
{
	return point - m_normal * distanceTo(point);
}

ePlaneResult Plane::testSide(const glm::vec2 & point) const
{
	float t = glm::dot(point, m_normal) + m_distanceToOrigin;
	if (t < 0)
		return ePlaneResult::BACK;
	else if (t > 0)
		return ePlaneResult::FRONT;
	return ePlaneResult::INTERSECTS;
}
