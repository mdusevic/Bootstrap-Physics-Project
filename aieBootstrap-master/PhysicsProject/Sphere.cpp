#include "Sphere.h"

#include <Gizmos.h>

Sphere::Sphere(glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_radius, glm::vec4 a_colour) :
	Rigidbody(SPHERE, a_position, a_velocity, a_mass, 0)
{
	m_radius = a_radius;
	m_colour = a_colour;
	m_moment = 0.5f * m_mass * m_radius * m_radius;
}

Sphere::~Sphere()
{
	
}

void Sphere::MakeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour);
}

bool Sphere::CheckCollision(PhysicsObject* pOther)
{
	Sphere* pSphere = dynamic_cast<Sphere*>(pOther);

	if (pSphere != nullptr)
	{
		float dist = glm::distance(GetPosition(), pSphere->GetPosition());
		if (GetRadius() + pSphere->GetRadius() > dist)
		{
			return true;
		}
	}

	return false;
}
