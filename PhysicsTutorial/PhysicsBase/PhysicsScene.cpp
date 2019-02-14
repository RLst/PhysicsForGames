#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"

#include <algorithm>
#include <list>
#include <iostream>

#include "Plane.h"
#include "Circle.h"
#include "Box.h"

PhysicsScene::PhysicsScene(float timeStep, glm::vec2 gravity, bool collisionEnabled) :
	m_timeStep(timeStep),
	m_gravity(gravity),
	m_isCollisionEnabled(collisionEnabled)
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

//Setup the collision function array
static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane, 	PhysicsScene::Plane2Circle,		PhysicsScene::Plane2Box,
	PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle,	PhysicsScene::Circle2Box,
	PhysicsScene::Box2Plane,	PhysicsScene::Box2Circle,		PhysicsScene::Box2Box
};

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
		if (m_isCollisionEnabled)
		{
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

void PhysicsScene::CheckForCollisions()
{
	int actorCount = m_actors.size();

	//Need to check for collisions against all objects except this one
	for (int outer = 0; outer < actorCount-1; ++outer)
	{
		for (int inner = outer + 1; inner < actorCount; ++inner)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeID1 = object1->GetShapeID();
			int shapeID2 = object2->GetShapeID();

			//Using function pointers
			int functionIDX = (shapeID1 * SHAPE_COUNT) + shapeID2;	//Auto select the correct function
			fn fpCollision = collisionFunctionArray[functionIDX];
			if (fpCollision != nullptr)
			{
				//did a collision occur
				fpCollision(object1, object2);
			}
		}
	}
}

bool PhysicsScene::Plane2Plane(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::Plane2Circle(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::Plane2Box(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::Circle2Plane(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::Circle2Circle(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Try to cast object to circle and circle
	Circle *circle1 = (Circle*)(obj1);
	Circle *circle2 = (Circle*)(obj2);

	//If we are successful then test for collision
	if (circle1 != nullptr && circle2 != nullptr)
	{
		//Get distance between 2 circles
		float distance = glm::distance(circle1->getPosition(), circle2->getPosition());

		//If distance is less than the combined radius of both spheres,
		if (distance < circle1->getRadius() + circle2->getRadius())
		{
			//then a collision occured so set the velocity of both spheres to 0 (for now)
			circle1->tempSetVelocity(vec2(0, 0));
			circle2->tempSetVelocity(vec2(0, 0));
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Circle2Box(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::Box2Plane(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::Box2Circle(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::Box2Box(PhysicsObject *, PhysicsObject *)
{
	return false;
}
