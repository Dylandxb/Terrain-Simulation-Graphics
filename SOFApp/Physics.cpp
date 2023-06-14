#include "stdafx.h"
#include "physics.h"

std::vector<ContactInfo> Physics::CollideWithWorld(const glm::vec3& spherePos, float sphereRad) const
{
	std::vector<ContactInfo> contacts;

	for (auto c : staticColliders)
	{
		glm::vec3 normal;
		float depth;
		if (c->Collide(spherePos, sphereRad,normal, depth))
		{
			//collision = true;
			contacts.push_back(ContactInfo{ normal,depth });
		}
	}
	return contacts;
}

void Physics::Update(float deltaTime)
{
	//Add collisions with spheres in separate loop
	//for loop with int index, first loop is from 0 to the number-1, second in loop from first number +1 to n 
	//pairs of spheres
	for (auto p : projectiles)
	{
		//Euler's method formula
		//y = y(xi) + f(xi, y(xi))(x−xi)
		p->pos += p->vel * deltaTime;
		p->vel += gravity * deltaTime;
		std::vector<ContactInfo> contacts = CollideWithWorld(p->pos, 0.25f);
		for (auto c : contacts)
		{
			if (glm::dot(p->vel, c.normal) < 0.0f)
			{
				p->pos += c.normal * c.depth;
				p->vel -= c.normal * glm::dot(c.normal, p->vel) * 2.0f; //2.0f is 1 + coefficient of resistution
			}
		}
		//TODO
	//Collision counter to projectile class
	//Slow the rate of velocity of the projectiles after a few seconds
	}
}