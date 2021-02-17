#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "Box.h"

class PhysicsProjectApp : public aie::Application
{
public:
	PhysicsProjectApp();
	virtual ~PhysicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	// Main Game
	void DrawBackground();
	void SpawnerMovement(float a_deltaTime);
	void SpinningWheelMovement(float a_deltaTime);

	// Test Scenes
	void DrawRect();
	void SphereAndPlane();
	void SpringTest(int a_amount);
	void TriggerTest();

	glm::vec2 ScreenToWorld(glm::vec2 a_screenPos);

	int GetCurrentBallAmount() { return m_currentBallAmount; }

protected:
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;

	const float m_aspectRatio = 16.0f / 9.0f;
	const float m_extents = 100.0f;

	Box* spawner;
	Box* wheel1;
	Box* wheel2;
	bool leftDir = false;
	float m_spawnerSpeed = 15.0f;
	float m_spinningSpeed = 1.5f;
	int m_currentBallAmount = 0;
	int m_maxBallAmount = 15;
};