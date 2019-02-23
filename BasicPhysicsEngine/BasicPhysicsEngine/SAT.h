#pragma once
#include "RigidBody.h"
#include <glm/ext.hpp>

#include "GameDefines.h"
#include <vector>

typedef glm::vec2 point;
typedef glm::vec2 surface;
typedef glm::vec2 normal;
typedef glm::vec2 projection;
typedef std::vector<glm::vec2> Vertices;

class SAT : public RigidBody
{
protected:
	std::vector<glm::vec2>		m_vertices;		//List of vertices of SAT object; 
		//Should these be the extents (ie. relative to this objects main position?)
	glm::vec4					m_colour = pkr::colour::random();

public:
	SAT();
	SAT(glm::vec2 position, glm::vec2 velocity, float mass, Vertices &vertices, glm::vec4 colour);
	~SAT();

	void						DrawGizmo() override;

	glm::vec2					project(glm::vec2 axis) const;

	std::vector<glm::vec2>		edges() const;		//Return list of all edges
	std::vector<glm::vec2>		normals() const;	//Return list of all surface normals
	std::vector<glm::vec2>		vertices() const;	//Return list of vertices relative to the world?
};

