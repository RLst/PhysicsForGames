#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "RigidBody.h"

#include <algorithm>
#include <list>
#include <iostream>

#include "Plane.h"
#include "Circle.h"
#include "AABB.h"

#include <vector>

typedef std::vector<glm::vec2> Vertices2;
typedef glm::vec2 Vertex2;

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

		CheckForCollisions();	//And also resolve collisions
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
	//int count = 0;
	//for (auto actor : m_actors)
	//{
	//	std::cout << count << " : ";
	//	actor->Debug();
	//	++count;
	//}
}

//Setup the collision function array
static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane,	PhysicsScene::Plane2Circle,		PhysicsScene::Plane2AABB,	PhysicsScene::Plane2SAT,
	PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle,	PhysicsScene::Circle2AABB,	PhysicsScene::Circle2SAT,
	PhysicsScene::AABB2Plane,	PhysicsScene::AABB2Circle,		PhysicsScene::AABB2AABB,		PhysicsScene::AABB2SAT,
	PhysicsScene::SAT2Plane,	PhysicsScene::SAT2Circle,		PhysicsScene::SAT2AABB,		PhysicsScene::SAT2SAT,
};

void PhysicsScene::CheckForCollisions()
{
	//Need to check for collisions against all objects except this one
	for (int outer = 0; outer < m_actors.size()-1; ++outer)
	{
		for (int inner = outer + 1; inner < m_actors.size(); ++inner)
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

//bool PhysicsScene::Plane2Plane(N/A)

bool PhysicsScene::Plane2Circle(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Plane *plane = (Plane*)obj1;
	Circle *circle = (Circle*)obj2;

	//If we are successful then test for collision
	if (circle != nullptr && plane != nullptr)
	{
		float intersection = circle->radius() - plane->distanceTo(circle->position());

		//If there is any intersecion it means the objects have collided
		if (intersection > 0.0f)
		{
			//Move circle out from the surface to prevent sticking
			//(The plane is static so it is easier to move object out)
			circle->displace(plane->normal() * intersection);

			//Objects have collided
			plane->ResolveCollision(circle);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Plane2AABB(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Try casting
	Plane *plane = (Plane*)obj1;
	AABB *aabb = (AABB*)obj2;

	//If successful then test for collisions
	if (plane != nullptr && aabb != nullptr)
	{
		//Brute force; Go through all vertices
		for (auto vertex : aabb->vertices())
		{
			//Does it makes more sense if an intersection occurs, that the number is positive?
			float intersection = -plane->distanceTo(vertex);

			if (intersection > 0.0f)
			{
				//Move aabb out from the surface to prevent sticking
				aabb->displace(plane->normal() * intersection);

				//Collision detected!
				plane->ResolveCollision(aabb);
				return true;
			}
		}
	}
	return false;
}

bool PhysicsScene::Plane2SAT(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::Circle2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Re-route
	return Plane2Circle(obj2, obj1);
}

bool PhysicsScene::Circle2Circle(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Try to cast object to circle and circle
	Circle *circle1 = (Circle*)(obj1);
	Circle *circle2 = (Circle*)(obj2);

	//If we are successful then test for collision
	if (circle1 != nullptr && circle2 != nullptr)
	{
		////Get distance between 2 circles 
		////UNOPTIMISED!!!
		//float distance = glm::distance(circle1->position(), circle2->position());
		//
		////If distance is less than the combined radius of both spheres,
		//if (distance < circle1->radius() + circle2->radius())
		//{
		//	//Resolve collision
		//	circle1->ResolveCollision(circle2);
		//	return true;
		//}

		//OPTIMISED!!!
		float dx = circle2->position().x - circle1->position().x;
		float dy = circle2->position().y - circle1->position().y;
		float radii = circle1->radius() + circle2->radius();
		if (glm::dot(vec2(dx, dy), vec2(dx, dy)) < radii * radii)
		//if ((dx * dx) + (dy * dy) < radii * radii)
		{
			//Resolve collision
			circle1->ResolveCollision(circle2);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Circle2AABB(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Try to cast objects to their types
	Circle* circle = (Circle*)obj1;
	AABB *aabb = (AABB*)obj2;

	//If casts were successful...
	if (circle != nullptr && aabb != nullptr)
	{
		////Test for collisions
		//Use clamping trick (A = clamp(C, min, max)
		glm::vec2 clampedDistance = glm::clamp(circle->position(), aabb->min(), aabb->max());
		glm::vec2 V = clampedDistance - circle->position();
		float result = glm::dot(V, V);
		
		//If there is any intersecion it means the objects have collided
		if (result <= (circle->radius() * circle->radius()))
		{
			//Objects have collided
			circle->ResolveCollision(aabb);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Circle2SAT(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::AABB2Plane(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Re-route
	return Plane2AABB(obj2, obj1);
}

bool PhysicsScene::AABB2Circle(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Re-route
	return Circle2AABB(obj2, obj1);
}

bool PhysicsScene::AABB2AABB(PhysicsObject * obj1, PhysicsObject * obj2)
{
	//Try casts
	AABB *aabb1 = (AABB*)(obj1);
	AABB *aabb2 = (AABB*)(obj2);

	//If cast successful..
	if (aabb1 != nullptr && aabb2 != nullptr)
	{
		//Find any overlap (collision)
		float xOverlap = pkr::min(aabb1->max().x, aabb2->max().x) - pkr::max(aabb1->min().x, aabb2->min().x);
		float yOverlap = pkr::min(aabb1->max().y, aabb2->max().y) - pkr::max(aabb1->min().y, aabb2->min().y);

		//If there is over lap then there is collision
		if (xOverlap > 0 && yOverlap > 0)
		{
			////AABBs have collided! 
			//Move out of each other's boundary 
			//TODO Not as straightforward as I initially thought
			//aabb1->displace(glm::vec2(-xOverlap, -yOverlap) / 2.0f);
			//aabb2->displace(glm::vec2(xOverlap, yOverlap) / 2.0f);
			
			//Resolve collision
			aabb1->ResolveCollision(aabb2);

			return true;
		}
	}
	return false;
}

bool PhysicsScene::AABB2SAT(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::SAT2Plane(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::SAT2Circle(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::SAT2AABB(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::SAT2SAT(PhysicsObject *, PhysicsObject *)
{
	return false;
}
