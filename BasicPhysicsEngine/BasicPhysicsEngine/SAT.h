#pragma once
#include "RigidBody.h"
#include <glm/ext.hpp>

#include "GameDefines.h"
#include <vector>

class SAT : public RigidBody
{
protected:
	std::vector<glm::vec2>		m_vertices;		//List of vertices of SAT object

public:
	SAT();
	~SAT();

	std::vector<glm::vec2>		getNormals() const;

};

