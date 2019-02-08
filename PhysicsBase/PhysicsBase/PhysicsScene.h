#pragma once
#include <glm/ext.hpp>
#include <vector>

class PhysicsObject;

class PhysicsScene
{
protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;

public:
	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* actor);
	bool RemoveActor(PhysicsObject* actor);
	void Update(float deltaTime);
	void UpdateGizmos();
	void DebugScene();

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }
};

