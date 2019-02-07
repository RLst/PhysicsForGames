#include "PhysicsScene.h"
#include "PhysicsObject.h"

#include <algorithm>

PhysicsScene::PhysicsScene() :
	m_timeStep(0.01f),
	m_gravity(0, 0)
{
}

PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::AddActor(PhysicsObject * actor)
{
	m_actors.push_back(actor);
}

bool PhysicsScene::RemoveActor(PhysicsObject * targetActor)
{
	//Go through the vector of actors...
	for (int i = 0; i < m_actors.size(); ++i)
	{
		//If the matching actor is found...
		if (m_actors[i] == targetActor)
		{
			//Erase
			m_actors.erase(m_actors.begin() + i);
			return true;
		}
	}
	return false;
}

void PhysicsScene::Update(float deltaTime)
{
	//Update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto actor : m_actors)
		{
			actor->FixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;
	}
}

void PhysicsScene::UpdateGizmos()
{
	for (auto actor : m_actors)
	{
		actor->MakeGizmo();
	}
}
