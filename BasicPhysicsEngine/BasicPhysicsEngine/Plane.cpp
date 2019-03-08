#include "Plane.h"
#include <Gizmos.h>
#include "RigidBody.h"
#include "PhysicsMaterial.h"

Plane::Plane() :
	PhysicsObject(eShapeType::PLANE),
	m_distanceToOrigin(0),
	m_normal(glm::vec2(0, 1)),	//Ground zero facing up
	m_elasticity(0.9f)
{
}

Plane::Plane(float x, float y, float distance, float elasticity /*= 0.9f*/) :
	PhysicsObject(eShapeType::PLANE),
	m_distanceToOrigin(distance),
	m_elasticity(elasticity)
{
	m_normal = glm::normalize(glm::vec2(x, y));
}

Plane::Plane(const vec2& normal, float distance, float elasticity /*= 0.9f*/) :
	PhysicsObject(eShapeType::PLANE),
	m_distanceToOrigin(distance),
	m_elasticity(elasticity)
{
	m_normal = glm::normalize(normal);
}

Plane::Plane(const vec2& point1, const vec2& point2, ePerpDirection pdir /*= LEFT*/, float elasticity /*= 0.9f*/) :
	PhysicsObject(eShapeType::PLANE),
	m_elasticity(elasticity)
{
	//Calculate normalised vector from point1 to point2
	auto v = point2 - point1;
	v = glm::normalize(v);

	//Set normal perpendicular to the vector (Left)
	if (pdir == LEFT) {
		m_normal.x = -v.y;
		m_normal.y = v.x;
	}
	else {
		m_normal.x = v.y;
		m_normal.y = -v.x;
	}

	//Calculate distance
	m_distanceToOrigin = glm::dot(point1, m_normal);
}

Plane::Plane(const vec2 & normalStart, const vec2 & normalEnd, PhysicsMaterial* material) :
	PhysicsObject(eShapeType::PLANE)
{
	//Material
	m_friction = material->friction;
	m_elasticity = material->elasticity;

	//Get normal vector
	m_normal = glm::normalize(normalEnd - normalStart);

	//Find distance to origin
	m_distanceToOrigin = glm::dot(normalStart, m_normal);
}

Plane::~Plane()
{
}

void Plane::DrawGizmo()
{
	float lineSegmentLength = 1000000.f;
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

	//Average the elasticities
	float elasticity = (m_elasticity + other->getElasticity()) / 2.0f;

	//Super formula (impulse magnitude)
	float j = glm::dot(-(1 + elasticity) * (relVelocity), m_normal) / (1 / other->mass());

	//Force to apply (impulsed aimed at the normal)
	glm::vec2 force = m_normal * j;
	other->ApplyForce(force);
}

float Plane::distanceTo(const vec2& point) const
{
	//Super fast (no sqrt)
	return (glm::dot(point, m_normal) - m_distanceToOrigin);
}

glm::vec2 Plane::closestPoint(const vec2& point) const
{
	return point - m_normal * distanceTo(point);
}

//ePlaneResult Plane::testSide(const glm::vec2 & point) const
//{
//	float t = glm::dot(point, m_normal) + m_distanceToOrigin;
//	if (t < 0)
//		return ePlaneResult::BACK;
//	else if (t > 0)
//		return ePlaneResult::FRONT;
//	return ePlaneResult::INTERSECTS;
//}
