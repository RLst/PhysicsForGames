#pragma once
#include "RigidBody.h"
#include <glm/ext.hpp>

#include "GameDefines.h"
#include <vector>

//Clean and minimal code
using glm::vec2;
using glm::vec4;
typedef std::vector<glm::vec2> listvec2;

class SAT : public RigidBody
{
protected:
	listvec2		m_vextents;		//List of "Vertex EXTENTS" ie. vertices in LOCAL coordinates relative to rb.position
	vec4			m_colour = pkr::colour::random();

public:
	SAT();
	SAT(const vec2& position, const vec2& velocity, float mass, const vec4& colour, const listvec2& vextents);	//Defaults to an empty list of vertices
	~SAT();

	void			DrawGizmo() override;
	void			AddVextent(const vec2& newVextent);	//Appends a new vertex (extent; local coords)
	void			AddVertex(const vec2& newVertex);	//Appends a new vertex (world coords)
	void			CentralisePosition();				//Calculates the mean center of all vertices and sets rb.position to this

	vec2			vertex(int index) const;			//Returns a vertex in WORLD coordinates
	listvec2		vertices() const;				//Return list of vertices in WORLD coordinates
	
	vec2			projection(const vec2& axis) const;			//Returns projection in WORLD coordinates
	listvec2		edges() const;						//Returns all edges in WORLD coordinates
	listvec2		surfaceNormals() const;					//Returns all surface normals in WORLD coordinates
	
};

