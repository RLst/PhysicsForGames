#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"

#include <algorithm>
#include <list>
#include <iostream>

PhysicsScene::PhysicsScene() :
	m_timeStep(0.01f),
	m_gravity(0, 0)
{
}

PhysicsScene::~PhysicsScene()
{
	//Clean up m_actors
	for (auto actor : m_actors)
	{
		delete actor;
	}
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
	static std::list<PhysicsObject*> dirty;

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

		//BAD Check for collisions (ideally you'd want to have some sort of scene management in place)
		for (auto actor : m_actors) 
		{
			for (auto other : m_actors)
			{
				//Ignore if it's the same actor
				if (actor == other)
					continue;
				//Ignore if they're both the final actors in the list??? 
				if (std::find(dirty.begin(), dirty.end(), actor) != dirty.end() &&
					std::find(dirty.begin(), dirty.end(), other) != dirty.end())
					continue;

				//Handle Collision
				RigidBody* rigid = (RigidBody*)(actor);
				if (rigid->CheckCollision(other) == true)
				{
					rigid->ApplyForceToActor((RigidBody*)other, rigid->getVelocity() * rigid->getMass());
					dirty.push_back(rigid);
					dirty.push_back(other);
				}
			}
		}
		dirty.clear();
	}
}

void PhysicsScene::UpdateGizmos()
{
	for (auto actor : m_actors)
	{
		actor->MakeGizmo();
	}
}

void PhysicsScene::DebugScene()
{
	int count = 0;
	for (auto actor : m_actors)
	{
		std::cout << count << " : ";
		actor->Debug();
		++count;
	}
}
