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
	SAT(vec2 position, vec2 velocity, float mass, vec4 colour, listvec2 &vextents = listvec2());	//Defaults to an empty list of vertices
	~SAT();

	void			DrawGizmo() override;
	void			AddVextent(const vec2 newVextent);	//Appends a new vertex (extent; local coords)
	void			AddVertex(const vec2 newVertex);	//Appends a new vertex (world coords)
	void			CentralisePosition();				//Calculates the mean center of all vertices and sets rb.position to this

	vec2			getVertex(int index) const;			//Returns a vertex in WORLD coordinates
	listvec2		getVertices() const;				//Return list of vertices in WORLD coordinates
	
	vec2			getProjection(vec2 axis) const;			//Returns projection in WORLD coordinates
	listvec2		getEdges() const;						//Returns all edges in WORLD coordinates
	listvec2		getSurfaceNormals() const;					//Returns all surface normals in WORLD coordinates
	
};

