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

		CheckForCollisions();

		//BAD Check for collisions (ideally you'd want to have some sort of scene management in place)
		//if (m_isCollisionEnabled)
		//{
		//	for (auto actor : m_actors) 
		//	{
		//		for (auto other : m_actors)
		//		{
		//			//Ignore if it's the same actor
		//			if (actor == other)
		//				continue;
		//			//Ignore if they're both the final actors in the list??? 
		//			if (std::find(dirty.begin(), dirty.end(), actor) != dirty.end() &&
		//				std::find(dirty.begin(), dirty.end(), other) != dirty.end())
		//				continue;

		//			//Handle Collision
		//			RigidBody* rigid = (RigidBody*)(actor);
		//			if (rigid->CheckCollision(other) == true)
		//			{
		//				rigid->ApplyForceToActor((RigidBody*)other, rigid->getVelocity() * rigid->getMass());
		//				dirty.push_back(rigid);
		//				dirty.push_back(other);
		//			}
		//		}
		//	}
		//	dirty.clear();
		//}
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
	auto actorCount = m_actors.size();

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

bool PhysicsScene::Plane2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::Plane2Circle(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return Circle2Plane(obj2, obj1);
}

bool PhysicsScene::Plane2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::Circle2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Circle *circle = (Circle*)obj1;
	Plane *plane = (Plane*)obj2;

	//If we are successful then test for collision
	if (circle != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->GetNormal();
		float circleToPlaneDistance = glm::dot(circle->getPosition(), plane->GetNormal()) - plane->GetDistance();

		//If we are behind plane then we flip the normal
		if (circleToPlaneDistance < 0)
		{
			collisionNormal *= -1.0f;
			circleToPlaneDistance *= -1.0f;
		}

		float intersection = circle->getRadius() - circleToPlaneDistance;
		if (intersection > 0)
		{
			//Set circle velocity to zero here
			circle->tempSetVelocity(glm::vec2());	//default to (0,0)?

			return true;	//Collided
		}
	}
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
			//Resolve collision
			circle1->ResolveCollision(circle2);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Circle2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::Box2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::Box2Circle(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}

bool PhysicsScene::Box2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return false;
}
