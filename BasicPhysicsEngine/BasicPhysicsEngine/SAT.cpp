#include "SAT.h"

SAT::~SAT()
{
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
		result.push_back(p2-p1);
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
