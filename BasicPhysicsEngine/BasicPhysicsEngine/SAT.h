#pragma once
#include "RigidBody.h"

class PhysicsMaterial;

class SAT : public RigidBody
{
protected:
	vec2array			m_vextents;		//List of "Vertex EXTENTS" ie. vertices in LOCAL coordinates relative to rb.position
	vec4				m_colour = pkr::colour::random();

public:
	SAT();
	SAT(const vec2& position, const vec2& velocity, float mass, const vec4& colour, const vec2array& vextents);	//Defaults to an empty list of vertices
	SAT(const vec2& position, const vec2& velocity, float mass, const vec4& colour, int sides, float radius);	//Regular polygon constructor

	SAT(const vec2& pos, const vec2& vel, float rot, float size, int sides, vec4 colour, PhysicsMaterial* material, bool isKinematic = false);
	~SAT();

	void			DrawGizmo() override;
	void			AddVextent(const vec2& newVextent);	//Appends a new vertex (extent; local coords)
	void			AddVertex(const vec2& newVertex);	//Appends a new vertex (world coords)
	void			CentralisePosition();				//Calculates the mean center of all vertices and sets rb.position to this

	vec2			vertex(int index) const;			//Returns a vertex in WORLD coordinates
	vec2array		vertices() const;					//Return list of vertices in WORLD coordinates
	
	vec2			projection(const vec2& axis) const;		//Returns projection in WORLD coordinates
	vec2array		edges() const;							//Returns all edges in WORLD coordinates
	vec2array		surfaceNormals() const;					//Returns all surface normals in WORLD coordinates
	
};

