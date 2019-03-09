#include "AABB.h"
#include <Gizmos.h>
#include "PhysicsMaterial.h"

AABB::AABB(const vec2& pos, const vec2& vel, float width, float height, const vec4& colour, float mass, float friction, float elasticity, bool isKinematic) :
	RigidBody(eShapeType::AABB, pos, vel, 0, isKinematic, new PhysicsMaterial(friction, elasticity, eMaterial::NIL), mass),
	m_colour(colour)
{
	m_moment = 1.f / 12.f * m_mass * width * height;
	m_extents = vec2(width * 0.5f, height * 0.5f);
}

AABB::AABB(const vec2 & pos, const vec2 & vel, float width, float height, const vec4& colour, PhysicsMaterial* material, bool isKinematic) :
	RigidBody(eShapeType::AABB, pos, vel, 0, isKinematic, material),
	m_colour(colour)
{
	m_mass = width * height * material->getDensity();
	m_moment = 1.f / 12.f * m_mass * width * height;
	m_extents = vec2(width * 0.5f, height * 0.5f);
}

AABB::~AABB()
{
}

void AABB::DrawGizmo()
{
	aie::Gizmos::add2DAABBFilled(m_position, m_extents, m_colour);
}

vec2array AABB::vertices() const
{
	vec2array verts(4);
	verts[0] = min();
	verts[1] = { min().x, max().y };
	verts[2] = max();
	verts[3] = { max().x, min().y };
	return verts;
}

vec2array AABB::vextents() const
{
	vec2array vextents(4);
	vextents[0] = -m_extents;
	vextents[1] = { -m_extents.x, m_extents.y };
	vextents[2] = m_extents;
	vextents[3] = { m_extents.x, -m_extents.y };
	return vextents;
}

vec2 AABB::projection(const vec2 & axis) const
{
	//Based on SAT algorithm
	auto cacheVertices = vertices();
	float min = glm::dot(axis, cacheVertices[0]);
	float max = min;
	for (int i = 0; i < vertices().size(); ++i)
	{
		float point = glm::dot(axis, cacheVertices[i]);
		if (point < min) min = point;
		else if (point > max) max = point;
	}
	return vec2(min, max);
}
