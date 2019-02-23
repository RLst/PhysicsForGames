#include "SAT.h"

#include <Gizmos.h>

SAT::SAT() :	//Test
	RigidBody(eShapeType::SAT, vec2(150, 125), pkr::Random::range_v2(-10.f, 10.f), 0, 10)
{
	m_vertices.push_back(glm::vec2(0, 0));
	m_vertices.push_back(glm::vec2(8, 16));
	m_vertices.push_back(glm::vec2(16, 0));
}

SAT::~SAT()
{
}

void SAT::DrawGizmo()
{
	////TEST!!!
	//glm::vec4 colour = pkr::colour::random();
	aie::Gizmos::add2DCircle(m_position, 2.f, 15, m_colour);		//Draw the main (rb) position
	for (int i = 0; i < m_vertices.size(); ++i)
	{
		auto vertHead = m_vertices[i];
		auto vertEnd = m_vertices[i + 1 == m_vertices.size() ? 0 : i + 1];	//Loop around
		aie::Gizmos::add2DLine(vertHead + m_position, vertEnd + m_position, m_colour);
	}
	//aie::Gizmos::add2DTri(m_vertices[0] + m_position, m_vertices[1] + m_position, m_vertices[2] + m_position, colour);
}

glm::vec2 SAT::project(glm::vec2 axis) const
{
	double min = glm::dot(axis, m_vertices[0]);
	double max = min;
	for (int i = 1; i < m_vertices.size(); ++i)
	{
		//Note: the axis must be normalized to get accurate results
		double point = glm::dot(axis, m_vertices[1]);

		//Work out the mins and max
		if (point < min)
			min = point;
		else if (point > max)
			max = point;
	}
	return glm::vec2(min, max);
}

std::vector<glm::vec2> SAT::edges() const
{
	std::vector<glm::vec2> result;
	//Loop through all vertices, get edges, normalis, return
	for (int i = 0; m_vertices.size(); ++i)
	{
		point p1 = m_vertices[i];
		point p2 = m_vertices[i + 1 == m_vertices.size() ? 0 : i + 1];
		//surface normalisedEdge = glm::normalize(p2 - p1);
		result.push_back(p2-p1 /*+ m_position*/);	//test
	}
	return result;
}

std::vector<glm::vec2> SAT::normals() const
{
	std::vector<glm::vec2> result;
	//Get edges, normalise, perpendiculate
	std::vector<surface> cachedSurfaces = edges();
	for (auto e : cachedSurfaces)
	{
		glm::vec2 normalised = glm::normalize(e);
		glm::vec2 normal = vec2(-normalised.y, normalised.x);	//Left
	}
	return result;
}
