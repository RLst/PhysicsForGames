#pragma once
#include "RigidBody.h"
#include <glm/ext.hpp>

#include "GameDefines.h"
#include <vector>

typedef glm::vec2 point;
typedef glm::vec2 surface;
typedef glm::vec2 normal;
typedef glm::vec2 projection;

class SAT : public RigidBody
{
protected:
	std::vector<glm::vec2>		m_vertices;		//List of vertices of SAT object

public:
	SAT();
	~SAT();

	glm::vec2					project(glm::vec2 axis) const;

	std::vector<glm::vec2>		edges() const;		//Return list of all edges
	std::vector<glm::vec2>		normals() const;	//Return list of all surface normals

};

