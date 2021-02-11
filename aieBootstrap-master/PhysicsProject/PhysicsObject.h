#pragma once
#include <glm/glm.hpp>

// This is a pure abstract class.
// It will be used as a base for the derived classes that will use FixedUpdate

enum ShapeType
{
	PLANE = 0,
	SPHERE,
	BOX,
	SHAPE_COUNT
};

class PhysicsObject
{
public:
	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep) = 0;
	virtual void MakeGizmo() = 0;
	virtual void Debug() = 0;
	virtual void ResetPosition() {};

	ShapeType GetShapeID() { return m_shapeID; }

	bool IsKinematic() { return m_isKinematic; }
	bool SetKinematic(bool a_state) { return a_state = m_isKinematic; }

	float GetElasticity() { return m_elasticity; }

	// Keep to 1 for realism.
	float SetElasticity(float a_new) { return m_elasticity = a_new; }

protected:
	ShapeType m_shapeID;

	bool m_isKinematic;
	float m_elasticity;

	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}
};
