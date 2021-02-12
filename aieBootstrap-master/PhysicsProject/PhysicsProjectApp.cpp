#include "PhysicsProjectApp.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Spring.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "glm\ext.hpp"

#include <Gizmos.h>

PhysicsProjectApp::PhysicsProjectApp()
{

}

PhysicsProjectApp::~PhysicsProjectApp()
{

}

bool PhysicsProjectApp::startup()
{
	// Increases 2D line count to maximise the number of objects we can draw.
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();

	m_physicsScene->SetGravity(glm::vec2(0, -10));

	// Lower the value, the more accurate the simulation will be;
	// but it will increase the processing time required.
	// If it is too high it causes the sim to stutter and reduce stability.
	m_physicsScene->SetTimeStep(0.01f);

	DrawBackground();

	return true;
}

void PhysicsProjectApp::shutdown()
{
	delete m_font;
	delete m_2dRenderer;
}

// Update Function
void PhysicsProjectApp::update(float deltaTime)
{
	// Input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	// Circle on click
	//if (input->isMouseButtonDown(0))
	//{
	//	int xScreen, yScreen;
	//	input->getMouseXY(&xScreen, &yScreen);
	//	glm::vec2 worldPos = ScreenToWorld(glm::vec2(xScreen, yScreen));
	//	aie::Gizmos::add2DCircle(worldPos, 5, 32, glm::vec4(0.7f));
	//}

	Spawner(deltaTime);

	// Exits the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
	{
		quit();
	}
}

// Draw Function
void PhysicsProjectApp::draw()
{
	// Wipes the screen to the background colour
	clearScreen();

	// Begin drawing sprites
	m_2dRenderer->begin();

	// If X-axis = -100 to 100, Y-axis = -56.25 to 56.25
	aie::Gizmos::draw2D(glm::ortho<float>(-m_extents, m_extents, -m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.0f, 1.0f));

	// Draw your stuff here!

	// Draws FPS counter
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	
	// Output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// Done drawing sprites
	m_2dRenderer->end();
}

// ---- Main Game Scenes ----
void PhysicsProjectApp::DrawBackground()
{
	// Creates spawner 
	spawner = new Box(glm::vec2(0, 45), glm::vec2(0), 0.0f, 1.0f, 4.0f, 1.2f, glm::vec4(1, 1, 1, 1));
	spawner->SetKinematic(true);
	m_physicsScene->AddActor(spawner);

	int gridWidth = 10;
	int gridHeight = 10;
	int spacing = 12;
	int count = 0;

	// Draws grid of balls
	// First two rows
	for (int y = -1; y < 1; y++)
	{
		for (int x = -5; x < 5; x++)
		{
			if (y % 2 == 0)
			{
				if (count < 9)
				{
					Sphere* sphere = new Sphere(glm::vec2((x * spacing + 6) + 4, (y * spacing) + 35), glm::vec2(0), 2.0f, 1.5f, glm::vec4(1, 1, 1, 1));
					m_physicsScene->AddActor(sphere);
					sphere->SetKinematic(true);
					count++;
				}
			}

			else
			{
				Sphere* sphere = new Sphere(glm::vec2((x * spacing) + 4, (y * spacing) + 35), glm::vec2(0), 2.0f, 1.5f, glm::vec4(1, 1, 1, 1));
				m_physicsScene->AddActor(sphere);
				sphere->SetKinematic(true);
			}
		}
	}

	// Left side two rows
	for (int y = -1; y < 1; y++)
	{
		for (int x = -1; x < 1; x++)
		{
			if (y % 2 == 0)
			{
				Sphere* sphere = new Sphere(glm::vec2((x * spacing + 6) - 44, (y * spacing) + 10), glm::vec2(0), 2.0f, 1.5f, glm::vec4(1, 1, 1, 1));
				m_physicsScene->AddActor(sphere);
				sphere->SetKinematic(true);
			}

			else
			{
				Sphere* sphere = new Sphere(glm::vec2((x * spacing) - 44, (y * spacing) + 10), glm::vec2(0), 2.0f, 1.5f, glm::vec4(1, 1, 1, 1));
				m_physicsScene->AddActor(sphere);
				sphere->SetKinematic(true);
			}
		}
	}

	// Right side two rows
	for (int y = -1; y < 1; y++)
	{
		for (int x = -1; x < 1; x++)
		{
			if (y % 2 == 0)
			{
				Sphere* sphere = new Sphere(glm::vec2((x * spacing - 6) + 52, (y * spacing) + 10), glm::vec2(0), 2.0f, 1.5f, glm::vec4(1, 1, 1, 1));
				m_physicsScene->AddActor(sphere);
				sphere->SetKinematic(true);
			}

			else
			{
				Sphere* sphere = new Sphere(glm::vec2((x * spacing) + 52, (y * spacing) + 10), glm::vec2(0), 2.0f, 1.5f, glm::vec4(1, 1, 1, 1));
				m_physicsScene->AddActor(sphere);
				sphere->SetKinematic(true);
			}
		}
	}

	count = 0;

	// Middle two rows
	for (int y = -1; y < 1; y++)
	{
		for (int x = -3; x < 3; x++)
		{
			if (y % 2 == 0)
			{
				Sphere* sphere = new Sphere(glm::vec2((x * spacing - 6) + 10, (y * spacing) - 2), glm::vec2(0), 2.0f, 1.5f, glm::vec4(1, 1, 1, 1));
				m_physicsScene->AddActor(sphere);
				sphere->SetKinematic(true);
			}

			else
			{
				if (count < 5)
				{
					Sphere* sphere = new Sphere(glm::vec2((x * spacing) + 10, (y * spacing) - 2), glm::vec2(0), 2.0f, 1.5f, glm::vec4(1, 1, 1, 1));
					m_physicsScene->AddActor(sphere);
					sphere->SetKinematic(true);
					count++;
				}
			}
		}
	}

	// Borders
	Box* leftSide = new Box(glm::vec2(-65, 0), glm::vec2(0), 0.0f, 2.0f, 1.0f, 50.0f, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(leftSide);
	leftSide->SetKinematic(true);

	Box* rightSide = new Box(glm::vec2(62, 0), glm::vec2(0), 0.0f, 2.0f, 1.0f, 50.0f, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(rightSide);
	rightSide->SetKinematic(true);

}

void PhysicsProjectApp::Spawner(float a_deltaTime)
{
	// Spawner Movement
	glm::vec2 spawnerPos = spawner->GetPosition();

	if (spawnerPos.x >= 60)
	{
		leftDir = true;
	}
	else if (spawnerPos.x <= -60)
	{
		leftDir = false;
	}

	spawner->SetPosition(glm::vec2(spawnerPos.x + (leftDir ? -m_spawnerSpeed: m_spawnerSpeed) * a_deltaTime, spawnerPos.y));

	// Spawner creating new pachinko balls
	aie::Input* input = aie::Input::getInstance();

	if (input->wasKeyPressed((aie::EInputCodes::INPUT_KEY_SPACE)))
	{
		Sphere* ball = new Sphere(spawnerPos, glm::vec2(0), 2.0f, 1.5f, glm::vec4(0, 0.5, 0, 1));
		m_physicsScene->AddActor(ball);
	}
}

// ---- Test Scenes ----
void PhysicsProjectApp::DrawRect()
{
	m_physicsScene->AddActor(new Sphere(glm::vec2(20, 10), glm::vec2(-10, -17), 1, 3, glm::vec4(1, 0, 0, 1)));
	m_physicsScene->AddActor(new Plane(glm::vec2(-0.65, 0.75), -25));

	Box* box1 = new Box(glm::vec2(-20, 0), glm::vec2(16, -4), 1, 4, 8, 4, glm::vec4(1, 0, 0, 1));
	box1->SetRotation(0.5f);

	Box* box2 = new Box(glm::vec2(10, 0), glm::vec2(-4, 0), 1, 4, 8, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);

	box1->ApplyForce(glm::vec2(30, 0), glm::vec2(0));
	box2->ApplyForce(glm::vec2(-15, 0), glm::vec2(0));

	Sphere* ball = new Sphere(glm::vec2(5, 10), glm::vec2(0), 1, 3, glm::vec4(0, 0, 1, 1));
	ball->SetRotation(0.5);
	m_physicsScene->AddActor(ball);
	ball->SetKinematic(true);
}

void PhysicsProjectApp::SphereAndPlane()
{
	Sphere* ball1;
	ball1 = new Sphere(glm::vec2(-40, 40), glm::vec2(0), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball1);
	ball1->ApplyForce(glm::vec2(50, 30), glm::vec2(0));

	Sphere* ball2;
	ball2 = new Sphere(glm::vec2(40, 40), glm::vec2(0), 3.0f, 1, glm::vec4(0, 1, 0, 1));
	m_physicsScene->AddActor(ball2);
	ball2->ApplyForce(glm::vec2(-50, 30), glm::vec2(0));

	Plane* plane = new Plane();
	m_physicsScene->AddActor(plane);
}

void PhysicsProjectApp::SpringTest(int a_amount)
{
	Sphere* prev = nullptr;
	for (int i = 0; i < a_amount; i++)
	{
		// This will need to spawn the sphere at the bottom of the previous one,
		// to make a pendulum that is effected by gravity
		Sphere* sphere = new Sphere(glm::vec2(i * 3, 30 - i * 5), glm::vec2(0), 10, 2, glm::vec4(0, 0, 1, 1));

		if (i == 0)
		{
			sphere->SetKinematic(true);
		}

		m_physicsScene->AddActor(sphere);

		if (prev)
		{
			m_physicsScene->AddActor(new Spring(sphere, prev, 10, 500));
		}

		prev = sphere;
	}

	Box* box = new Box(glm::vec2(0, -20), glm::vec2(0), 0.3f, 20, 8, 2);
	box->SetKinematic(true);
	m_physicsScene->AddActor(box);
}

void PhysicsProjectApp::TriggerTest()
{
	Sphere* ball1 = new Sphere(glm::vec2(-20, 0), glm::vec2(10), 4, 4, glm::vec4(1, 0, 0, 1));
	Sphere* ball2 = new Sphere(glm::vec2(10, -20), glm::vec2(-10), 4, 4, glm::vec4(0, 0.5, 0.5, 1));

	ball2->SetKinematic(true);
	ball2->SetTrigger(true);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(new Plane(glm::vec2(0, 1), -30));
	m_physicsScene->AddActor(new Plane(glm::vec2(1, 0), -50));
	m_physicsScene->AddActor(new Plane(glm::vec2(-1, 0), -50));
	m_physicsScene->AddActor(new Box(glm::vec2(20, 10), glm::vec2(8, 0), 0.5, 4, 8, 4));
	m_physicsScene->AddActor(new Box(glm::vec2(-40, 10), glm::vec2(-8, 0), 0.5, 4, 8, 4));

	ball2->triggerEnter = [=](PhysicsObject* other) { std::cout << "Entered: " << other << std::endl; };
	ball2->triggerExit = [=](PhysicsObject* other) { std::cout << "Exited: " << other << std::endl; };
}

glm::vec2 PhysicsProjectApp::ScreenToWorld(glm::vec2 a_screenPos)
{
	glm::vec2 worldPos = a_screenPos;

	// We will move the center of the screen to (0, 0)
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	// Scale this according to the extents
	worldPos.x *= 2.0f * m_extents / getWindowWidth();
	worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

	return worldPos;
}