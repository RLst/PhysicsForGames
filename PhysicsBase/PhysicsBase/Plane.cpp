#include "Plane.h"
#include <Gizmos.h>

Plane::Plane(glm::vec2 normal, float distance) :
	PhysicsObject(PLANE),
	m_normal(normal),
	m_distanceToOrigin(distance)
{
	m_orig_normal = m_normal;
	m_orig_distanceToOrigin = m_distanceToOrigin;
}

Plane::~Plane()
{
}

void Plane::Debug()
{
}

void Plane::MakeGizmo()
{
	float lineSegmentLength = 300;
	vec2 centerPoint = m_normal * m_distanceToOrigin;
		//Easy to rotate normal through 90 degrees around z
	vec2 parallel(m_normal.y, -m_normal.x);
	col colour(1, 1, 1, 1);
	vec2 start = centerPoint + (parallel * lineSegmentLength);
	vec2 end = centerPoint - (parallel * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, colour);
}

void Plane::ResetPosition()
{
	m_normal = m_orig_normal;
	m_distanceToOrigin = m_orig_distanceToOrigin;
}
