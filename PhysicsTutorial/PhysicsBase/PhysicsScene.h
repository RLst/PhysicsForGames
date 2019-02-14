#pragma once
#include <glm/ext.hpp>
#include <vector>

class PhysicsObject;
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

class PhysicsScene
{
private:
	bool m_isCollisionEnabled;

protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
	
public:
	PhysicsScene(
		float timeStep = 0.02f,					//60 fps
		glm::vec2 gravity = glm::vec2(0, 0),	//Gravity
		bool collisionEnabled = true);			//Collision enabled by default	
	~PhysicsScene();

	void		AddActor(PhysicsObject* actor);
	bool		RemoveActor(PhysicsObject* actor);
	void		Update(float deltaTime);
	void		UpdateGizmos();
	void		DebugScene();

	//Collision detection
	void		CheckForCollisions();
	static bool	Plane2Plane(PhysicsObject*, PhysicsObject*);
	static bool Plane2Circle(PhysicsObject*, PhysicsObject*);
	static bool Plane2Box(PhysicsObject*, PhysicsObject*);
	static bool Circle2Plane(PhysicsObject*, PhysicsObject*);
	static bool Circle2Circle(PhysicsObject*, PhysicsObject*);
	static bool Circle2Box(PhysicsObject*, PhysicsObject*);
	static bool Box2Plane(PhysicsObject*, PhysicsObject*);
	static bool Box2Circle(PhysicsObject*, PhysicsObject*);
	static bool Box2Circle(PhysicsObject*, PhysicsObject*);
	static bool Box2Box(PhysicsObject*, PhysicsObject*);

	void		setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2	getGravity() const { return m_gravity; }

	void		setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float		getTimeStep() const { return m_timeStep; }

	void		setCollisionOn() { m_isCollisionEnabled = true; }
	void		setCollisionOff() { m_isCollisionEnabled = false; }
};

