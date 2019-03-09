#include "SAT.h"
#include <Gizmos.h>
#include "PhysicsMaterial.h"

SAT::SAT() :	//Test
	RigidBody(eShapeType::SAT, vec2(0,0), vec2(20, 20), 0, 10)
{
	m_vextents.push_back(vec2(-9, 0));
	m_vextents.push_back(vec2(0, 16));
	m_vextents.push_back(vec2(9, 0));
}

SAT::SAT(const vec2& position, const vec2& velocity, float mass, const vec4& colour, const vec2array& vextents = vec2array()) :
	RigidBody(eShapeType::SAT, position, velocity, 0, mass),
	m_colour(colour)
{
	//Set vertices
	m_vextents = vextents;
}

SAT::SAT(const vec2 & position, const vec2 & velocity, float mass, const vec4 & colour, int sides, float radius) :
	RigidBody(eShapeType::SAT, position, velocity, 0, mass),
	m_colour(colour)
{
	//Generate regular polygon
	float angleRad;
	for (int i = 0; i < sides; ++i)
	{
		angleRad = i * 2.0f * PI / (float)sides;
		m_vextents.push_back(vec2(radius * sinf(angleRad), radius * cosf(angleRad)));
	}
}

SAT::SAT(const vec2& pos, const vec2& vel, float rot, float size, unsigned int sides, const vec4& colour, float friction, float elasticity, float mass, bool isKinematic) :
	RigidBody(eShapeType::SAT, pos, vel, rot, isKinematic, new PhysicsMaterial(friction, elasticity, eMaterial::NIL), mass),
	m_colour(colour)
{
	float radius = size * 0.5f;
	//MOI
	m_moment = 0.5f * m_mass * radius * radius;		//Just use Circle formula

	//Generate regular polygon
	float angleRad;
	for (int i = 0; i < sides; ++i)
	{
		angleRad = i * 2.0f * PI / (float)sides;
		m_vextents.push_back(vec2(radius * sinf(angleRad), radius * cosf(angleRad)));
	}
}

SAT::SAT(const vec2 & pos, const vec2 & vel, float rot, float size, unsigned int sides, const vec4& colour, PhysicsMaterial* material, bool isKinematic) :
	RigidBody(eShapeType::SAT, pos, vel, rot, isKinematic),
	m_colour(colour)
{
	float radius = size * 0.5f;
	//Mass
	m_mass = PI * radius * radius * material->getDensity();

	//MOI
	m_moment = 0.5f * m_mass * radius * radius;		//Just use Circle formula

	//Generate regular polygon
	float angleRad;
	for (int i = 0; i < sides; ++i)
	{
		angleRad = i * 2.0f * PI / (float)sides;
		m_vextents.push_back(vec2(radius * sinf(angleRad), radius * cosf(angleRad)));
	}
}

SAT::~SAT()
{
}

void SAT::DrawGizmo()
{
	//Make sure that there are enough vertices
	if (m_vextents.size() < 3) return;

	//if so get the first vert
	vec2 vert0 = m_vextents[0];

	for (int i = 1; i < m_vextents.size() - 1; ++i)
	{
		auto vert1 = m_vextents[i];
		auto vert2 = m_vextents[i + 1];
		aie::Gizmos::add2DTri(vert1 + m_position, vert0 + m_position, vert2 + m_position, m_colour);
	}
}

void SAT::AddVextent(const vec2& newVextent)
{
	//Local coords
	m_vextents.push_back(newVextent);
}

void SAT::AddVertex(const vec2& newVertex)
{
	//World coords
	m_vextents.push_back(newVertex - m_position);
}

void SAT::CentralisePosition()
{
	/*
	original offset = 100,100;

	*/

	auto originalOffset = m_position;
	vec2 localCenter = vec2(0, 0);
	//listvec2 cacheVertices = vertices();
	//for (auto v : cacheVertices)
	for (auto v : m_vextents)
	{
		//Get sum of all local vertex extents
		localCenter += v;
	}
	//Calculate average
	localCenter /= (float)(m_vextents.size());
	m_position -= localCenter;	//Set as new rb.position
}

vec2 SAT::vertex(int index) const
{
	//Returns a vertex in WORLD coordinates
	assert(index >= 0 && index < m_vextents.size());	//Index out of bounds
	return m_vextents[index] + m_position;
}

vec2array SAT::vertices() const
{
	//Return all vertices in WORLD coordinates
	vec2array result;
	for (auto v : m_vextents)
	{
		result.push_back(v + m_position);
	}
	return result;
}

vec2 SAT::projection(const vec2& axis) const
{
	float min = glm::dot(axis, this->vertex(0));
	float max = min;
	for (int i = 0; i < m_vextents.size(); ++i)		//faster than getVertices().size()
	{
		//Note: the axis must be normalized to get accurate results
		float point = glm::dot(axis, vertex(i));

		//Work out the mins and max
		if (point < min)
			min = point;
		else if (point > max)
			max = point;
	}
	return vec2(min, max);
}

vec2array SAT::edges() const
{
	std::vector<vec2> result;
	//Loop through all vertices, get edges, return
	for (int i = 0; i < vertices().size(); ++i)
	{
		vec2 start = vertex(i);
		vec2 end = vertex(i + 1 == m_vextents.size() ? 0 : i + 1);
		result.push_back(end-start);
	}
	return result;
}

vec2array SAT::surfaceNormals() const
{
	vec2array result;
	//Get edges, normalise, perpendiculate
	vec2array edgesCached = edges();
	for (auto e : edgesCached)
	{
		result.push_back(glm::normalize(vec2(-e.y, e.x)));	//normalise + perpend
	}
	return result;
}



