#include "SAT.h"

#include <Gizmos.h>

SAT::SAT() :	//Test
	RigidBody(eShapeType::SAT, vec2(0,0), vec2(20, 20), 0, 10)
{
	m_vextents.push_back(vec2(-9, 0));
	m_vextents.push_back(vec2(0, 16));
	m_vextents.push_back(vec2(9, 0));
}

SAT::SAT(const vec2& position, const vec2& velocity, float mass, const vec4& colour, const listvec2& vextents = listvec2()) :
	RigidBody(eShapeType::SAT, position, velocity, 0, mass),
	m_colour(colour)
{
	//Set vertices
	m_vextents = vextents;
}

SAT::~SAT()
{
}

void SAT::DrawGizmo()
{
	aie::Gizmos::add2DCircle(m_position, 0.5f, 8, m_colour);		//Draw the main (rb) position
	for (int i = 0; i < m_vextents.size(); ++i)
	{
		auto vertHead = m_vextents[i];
		auto vertEnd = m_vextents[i + 1 == m_vextents.size() ? 0 : i + 1];	//Loop around
		aie::Gizmos::add2DLine(vertHead + m_position, vertEnd + m_position, m_colour);
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

listvec2 SAT::vertices() const
{
	//Return all vertices in WORLD coordinates
	listvec2 result;
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

listvec2 SAT::edges() const
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

listvec2 SAT::surfaceNormals() const
{
	listvec2 result;
	//Get edges, normalise, perpendiculate
	listvec2 edgesCached = edges();
	for (auto e : edgesCached)
	{
		result.push_back(glm::normalize(vec2(-e.y, e.x)));	//normalise + perpend
	}
	return result;
}



