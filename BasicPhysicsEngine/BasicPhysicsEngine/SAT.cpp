#include "SAT.h"

#include <Gizmos.h>

SAT::SAT() :	//Test
	RigidBody(eShapeType::SAT, vec2(250, 125), vec2(0,0), 0, 10)
{
	m_vextents.push_back(vec2(-9, 0));
	m_vextents.push_back(vec2(0, 16));
	m_vextents.push_back(vec2(9, 0));
}

SAT::SAT(vec2 position, vec2 velocity, float mass, vec4 colour, listvec2 & vextents) :
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
	////TEST!!!
	aie::Gizmos::add2DCircle(m_position, 2.f, 15, m_colour);		//Draw the main (rb) position
	for (int i = 0; i < m_vextents.size(); ++i)
	{
		auto vertHead = m_vextents[i];
		auto vertEnd = m_vextents[i + 1 == m_vextents.size() ? 0 : i + 1];	//Loop around
		aie::Gizmos::add2DLine(vertHead + m_position, vertEnd + m_position, m_colour);
	}
}

void SAT::AddVextent(const vec2 newVextent)
{
	//Local coords
	m_vextents.push_back(newVextent);
}

void SAT::AddVertex(const vec2 newVertex)
{
	//World coords
	m_vextents.push_back(newVertex - m_position);
}

void SAT::CentralisePosition()
{
	vec2 center;
	for (auto v : m_vextents)
	{
		//Get sum of all vertices
		center += v;
	}
	//Calculate average
	center /= (float)(m_vextents.size() + 1);
	m_position = center;	//Set as new rb.position
}

vec2 SAT::getVertex(int index) const
{
	//Returns a vertex in WORLD coordinates
	assert(index >= 0 && index < m_vextents.size());	//Index out of bounds
	return m_vextents[index] + m_position;
}

listvec2 SAT::getVertices() const
{
	//Return all vertices in WORLD coordinates
	listvec2 result;
	for (auto v : m_vextents)
	{
		result.push_back(v + m_position);
	}
	return result;
}

vec2 SAT::getProjection(vec2 axis) const
{
	double min = glm::dot(axis, getVertex(0));
	double max = min;
	for (int i = 1; i < getVertices().size(); ++i)
	{
		//Note: the axis must be normalized to get accurate results
		double point = glm::dot(axis, getVertex(1));

		//Work out the mins and max
		if (point < min)
			min = point;
		else if (point > max)
			max = point;
	}
	return vec2((float)min, (float)max);
}

listvec2 SAT::getEdges() const
{
	std::vector<vec2> result;
	//Loop through all vertices, get edges, return
	for (int i = 0; i < getVertices().size(); ++i)
	{
		vec2 start = getVertex(i);
		vec2 end = getVertex(i + 1 == m_vextents.size() ? 0 : i + 1);
		result.push_back(end-start);
	}
	return result;
}

listvec2 SAT::getSurfaceNormals() const
{
	listvec2 result;
	//Get edges, normalise, perpendiculate
	listvec2 edgesCached = getEdges();
	for (auto e : edgesCached)
	{
		result.push_back(glm::normalize(vec2(-e.y, e.x)));	//normalise + perpend
	}
	return result;
}



