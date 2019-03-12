#pragma once
#include <glm/ext.hpp>
#include <vector>

class PhysicsObject;

//Clean minimal code
using glm::vec2;
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

class PhysicsScene
{
private:
	bool m_isCollisionEnabled;

protected:
	vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
	
public:
	PhysicsScene(
		float timeStep = 0.02f,					//60 fps
		vec2 gravity = vec2(0, 0),	//Gravity
		bool collisionEnabled = true);			//Collision enabled by default	
	~PhysicsScene();

	void		AddActor(PhysicsObject* actor);
	bool		RemoveActor(PhysicsObject* actor);
	void		Update(float deltaTime);
	void		UpdateGizmos();		//Redraws gizmos
	void		DebugScene();

	void		CheckForCollisions();
	static float FindOverlap(const vec2 &projection1, const vec2 &projection2); 

	static bool	Plane2Plane(PhysicsObject*, PhysicsObject*) { return false;	} //DO NOT REMOVE! Place holder
	static bool Plane2Circle(PhysicsObject*, PhysicsObject*);
	static bool Plane2AABB(PhysicsObject*, PhysicsObject*);
	static bool Plane2SAT(PhysicsObject*, PhysicsObject*);

	static bool Circle2Plane(PhysicsObject*, PhysicsObject*);
	static bool Circle2Circle(PhysicsObject*, PhysicsObject*);
	static bool Circle2AABB(PhysicsObject*, PhysicsObject*);
	static bool Circle2SAT(PhysicsObject*, PhysicsObject*);

	static bool AABB2Plane(PhysicsObject*, PhysicsObject*);
	static bool AABB2Circle(PhysicsObject*, PhysicsObject*);
	static bool AABB2AABB(PhysicsObject*, PhysicsObject*);
	static bool AABB2SAT(PhysicsObject*, PhysicsObject*);

	static bool SAT2Plane(PhysicsObject*, PhysicsObject*);
	static bool SAT2Circle(PhysicsObject*, PhysicsObject*);
	static bool SAT2AABB(PhysicsObject*, PhysicsObject*);
	static bool SAT2SAT(PhysicsObject*, PhysicsObject*);

	vec2		getGravity() const { return m_gravity; }
	void		setGravity(const vec2 gravity) { m_gravity = gravity; }

	float		getTimeStep() const { return m_timeStep; }
	void		setTimeStep(const float timeStep) { m_timeStep = timeStep; }

	void		setCollisionsOn() { m_isCollisionEnabled = true; }
	void		setCollisionsOff() { m_isCollisionEnabled = false; }

	size_t		getActorCount() const { return m_actors.size(); }
};

