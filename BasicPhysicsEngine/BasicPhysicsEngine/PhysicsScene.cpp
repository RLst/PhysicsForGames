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
	int count = 0;
	for (auto actor : m_actors)
	{
		std::cout << count << " : ";
		actor->Debug();
		++count;
	}
}

//Setup the collision function array
static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane,	PhysicsScene::Plane2Circle,		PhysicsScene::Plane2Box,	PhysicsScene::Plane2SAT,
	PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle,	PhysicsScene::Circle2Box,	PhysicsScene::Circle2SAT,
	PhysicsScene::Box2Plane,	PhysicsScene::Box2Circle,		PhysicsScene::Box2Box,		PhysicsScene::Box2SAT,
	PhysicsScene::SAT2Plane,	PhysicsScene::SAT2Circle,		PhysicsScene::SAT2BOX,		PhysicsScene::SAT2SAT,
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
		glm::vec2 collisionNormal = plane->GetNormal();
		float circleToPlaneDistance = glm::dot(circle->getPosition(), plane->GetNormal()) - plane->GetDistance();

		float intersection = circle->getRadius() - circleToPlaneDistance;
		//If there is any intersecion it means the objects have collided
		if (intersection > 0.0f)
		{
			//Move circle out from the plane to prevent sticking
			//(The plane is static so it is easier to move object out)
			circle->displace(plane->GetNormal() * intersection);

			//Objects have collided
			plane->ResolveCollision(circle);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Plane2Box(PhysicsObject * obj1, PhysicsObject * obj2)
{
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
		//Get distance between 2 circles 
		//TODO can this be optimised with sqrDist?
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

bool PhysicsScene::Circle2SAT(PhysicsObject *, PhysicsObject *)
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
	//Try to cast object to box and box
	Box *box1 = (Box*)(obj1);
	Box *box2 = (Box*)(obj2);

	//If cast successful..
	if (box1 != nullptr && box2 != nullptr)
	{
		//Find any overlap (collision)
		float xOverlap = pkr::min(box1->getMax().x, box2->getMax().x) - pkr::max(box1->getMin().x, box2->getMin().x);
		float yOverlap = pkr::min(box1->getMax().y, box2->getMax().y) - pkr::max(box1->getMin().y, box2->getMin().y);

		//If there is over lap then there is collision
		if (xOverlap > 0 && yOverlap > 0)
		{
			////Boxes (AABB's) have collided! 
		
			//Move out of each other's boundary 
			//TODO Not as straightforward as I initially thought
			//box1->displace(glm::vec2(-xOverlap, -yOverlap));
			
			//Resolve collision
			box1->ResolveCollision(box2);

			return true;
		}
	}
	return false;
}

bool PhysicsScene::Box2SAT(PhysicsObject *, PhysicsObject *)
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

bool PhysicsScene::SAT2BOX(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::SAT2SAT(PhysicsObject *, PhysicsObject *)
{
	return false;
}
