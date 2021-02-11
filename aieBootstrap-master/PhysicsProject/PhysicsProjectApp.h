#pragma once

#include "Application.h"
#include "Renderer2D.h"

#include "PhysicsScene.h"

class PhysicsProjectApp : public aie::Application {
public:

	PhysicsProjectApp();
	virtual ~PhysicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void DrawRect();
	void SphereAndPlane();
	void SpringTest(int a_amount);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;
};