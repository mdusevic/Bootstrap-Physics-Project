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

	m_physicsScene->SetGravity(glm::vec2(0, -15));

	// Lower the value, the more accurate the simulation will be;
	// but it will increase the processing time required.
	// If it is too high it causes the sim to stutter and reduce stability.
	m_physicsScene->SetTimeStep(0.01f);

	DrawPachinkoGame();

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

	// Movement Functions
	SpawnerMovement(deltaTime);
	SpinningWheelMovement(deltaTime);

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

	// Outputs high score
	char score[32];
	sprintf_s(score, 32, "Score: %i", GetScore());
	m_2dRenderer->drawText(m_font, score, 450, 720 - 32);

	// Outputs number of balls left
	char ballCount[32];
	sprintf_s(ballCount, 32, "Balls Left: %i", (m_maxBallAmount - GetCurrentBallAmount()));
	m_2dRenderer->drawText(m_font, ballCount, 5, 720 - 32);
	
	// Draws FPS counter
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 5, 720 - 96);

	// Draws text in point bin to indicate point amount
	m_2dRenderer->drawText(m_font, "1000", 605, 80);
	m_2dRenderer->drawText(m_font, "500", 515, 80);
	m_2dRenderer->drawText(m_font, "500", 710, 80);
	m_2dRenderer->drawText(m_font, "250", 398, 80);
	m_2dRenderer->drawText(m_font, "250", 830, 80);
	m_2dRenderer->drawText(m_font, "100", 965, 80);
	m_2dRenderer->drawText(m_font, "100", 260, 80);

	// Output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 5, 15);

	// Done drawing sprites
	m_2dRenderer->end();
}

// ---- Main Game Scenes ----
void PhysicsProjectApp::DrawPachinkoGame()
{
	// Creates spawner 
	spawner = new Box(glm::vec2(0, 44), glm::vec2(0), 0.0f, 1.0f, 5.0f, 1.2f, glm::vec4(1, 1, 1, 1));
	m_physicsScene->AddActor(spawner);
	spawner->SetKinematic(true);

	// Creates spinning wheel
	wheel1 = new Box(glm::vec2(0, 0), glm::vec2(0), 0.0f, 8.0f, 1.0f, 9.0f, glm::vec4(1, 0.5, 0, 1));
	m_physicsScene->AddActor(wheel1);
	wheel1->SetKinematic(true);
	wheel1->SetElasticity(2.0f);

	wheel2 = new Box(glm::vec2(0, 0), glm::vec2(0), 0.0f, 8.0f, 9.0f, 1.0f, glm::vec4(1, 0.8, 0, 1));
	m_physicsScene->AddActor(wheel2);
	wheel2->SetKinematic(true);
	wheel2->SetElasticity(2.0f);

	wheel1->m_collisionCallback = [=](PhysicsObject* other)
	{
		Rigidbody* rb = dynamic_cast<Rigidbody*>(other);
		rb->ApplyForce(rb->GetVelocity() * 5.0f, glm::vec2(0, 0));
	};

	wheel2->m_collisionCallback = [=](PhysicsObject* other)
	{
		Rigidbody* rb = dynamic_cast<Rigidbody*>(other);
		rb->ApplyForce(rb->GetVelocity() * 5.0f, glm::vec2(0, 0));
	};

	Sphere* wheelCentre = new Sphere(glm::vec2(0, 0), glm::vec2(0), 8.0f, 3.4f, glm::vec4(0.3, 0.3, 0.3, 1));
	m_physicsScene->AddActor(wheelCentre);
	wheelCentre->SetKinematic(true);
	wheelCentre->SetElasticity(3.0f);

	// Creates grid of spheres for ball to collide with
	int spacing = 10;
	int count = 0;

	// Draws grid of balls
	// First two rows
	for (int y = -1; y < 2; y++)
	{
		for (int x = -6; x < 7; x++) 
		{
			if (y % 2 == 0)
			{
				if (count < 12)
				{
					Sphere* sphere = new Sphere(glm::vec2((x * spacing + 5), (y * spacing) + 25), glm::vec2(0), 5.0f, 1.5f, glm::vec4(1, 1, 1, 1));
					m_physicsScene->AddActor(sphere);
					sphere->SetKinematic(true);
					sphere->SetElasticity(2.0f);
					count++;
				}
			}

			else
			{
				Sphere* sphere = new Sphere(glm::vec2((x * spacing), (y * spacing) + 25), glm::vec2(0), 5.0f, 1.5f, glm::vec4(1, 1, 1, 1));
				m_physicsScene->AddActor(sphere);
				sphere->SetKinematic(true);
				sphere->SetElasticity(2.0f);
			}
		}
	}

	// Left side rows
	for (int y = -2; y < 1; y++)
	{
		for (int x = -1; x < 2; x++)
		{
			if (y % 2 == 0)
			{
				Sphere* sphere = new Sphere(glm::vec2((x * spacing + 5) - 50, (y * spacing) + 5), glm::vec2(0), 5.0f, 1.5f, glm::vec4(1, 1, 1, 1));
				m_physicsScene->AddActor(sphere);
				sphere->SetKinematic(true);
				sphere->SetElasticity(2.0f);
			}

			else
			{
				Sphere* sphere = new Sphere(glm::vec2((x * spacing) - 50, (y * spacing) + 5), glm::vec2(0), 5.0f, 1.5f, glm::vec4(1, 1, 1, 1));
				m_physicsScene->AddActor(sphere);
				sphere->SetKinematic(true);
				sphere->SetElasticity(2.0f);
			}
		}
	}

	// Right side rows
	for (int y = -2; y < 1; y++)
	{
		for (int x = -1; x < 2; x++)
		{
			if (y % 2 == 0)
			{
				Sphere* sphere = new Sphere(glm::vec2((x * spacing - 5) + 50, (y * spacing) + 5), glm::vec2(0), 5.0f, 1.5f, glm::vec4(1, 1, 1, 1));
				m_physicsScene->AddActor(sphere);
				sphere->SetKinematic(true);
				sphere->SetElasticity(2.0f);
			}

			else
			{
				Sphere* sphere = new Sphere(glm::vec2((x * spacing) + 50, (y * spacing) + 5), glm::vec2(0), 5.0f, 1.5f, glm::vec4(1, 1, 1, 1));
				m_physicsScene->AddActor(sphere);
				sphere->SetKinematic(true);
				sphere->SetElasticity(2.0f);
			}
		}
	}

	count = 0;

	// Middle two rows
	for (int y = -1; y < 1; y++)
	{
		for (int x = -3; x < 4; x++)
		{
			if (y % 2 == 0)
			{
				if (count < 6)
				{
					Sphere* sphere = new Sphere(glm::vec2((x * spacing + 5), (y * spacing) - 15), glm::vec2(0), 5.0f, 1.5f, glm::vec4(1, 1, 1, 1));
					m_physicsScene->AddActor(sphere);
					sphere->SetKinematic(true);
					sphere->SetElasticity(2.0f);
					count++;
				}
			}

			else
			{
				Sphere* sphere = new Sphere(glm::vec2((x * spacing), (y * spacing) - 15), glm::vec2(0), 5.0f, 1.5f, glm::vec4(1, 1, 1, 1));
				m_physicsScene->AddActor(sphere);
				sphere->SetKinematic(true);
				sphere->SetElasticity(2.0f);
			}
		}
	}

	// Bouncy springs
	Box* bouncyMidLeft = new Box(glm::vec2(-22, -2), glm::vec2(0), 0.9f, 8.0f, 0.5f, 5.5f, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(bouncyMidLeft);
	bouncyMidLeft->SetKinematic(true);
	bouncyMidLeft->SetElasticity(8.0f);

	Box* bouncyMidRight = new Box(glm::vec2(22, -2), glm::vec2(0), -0.9f, 8.0f, 0.5f, 5.5f, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(bouncyMidRight);
	bouncyMidRight->SetKinematic(true);
	bouncyMidRight->SetElasticity(8.0f);

	Box* bouncyBotLeft = new Box(glm::vec2(-48, -28), glm::vec2(0), 1.0f, 8.0f, 0.5f, 5.5f, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(bouncyBotLeft);
	bouncyBotLeft->SetKinematic(true);
	bouncyBotLeft->SetElasticity(8.0f);

	Box* bouncyBotRight = new Box(glm::vec2(48, -28), glm::vec2(0), -1.0f, 8.0f, 0.5f, 5.5f, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(bouncyBotRight);
	bouncyBotRight->SetKinematic(true);
	bouncyBotRight->SetElasticity(8.0f);

	// Borders
	Box* leftSide = new Box(glm::vec2(-66, 0), glm::vec2(0), 0.0f, 4.0f, 1.0f, 46.0f, glm::vec4(0.2, 0.2, 0.2, 1));
	m_physicsScene->AddActor(leftSide);
	leftSide->SetKinematic(true);
	leftSide->SetElasticity(5.0f);

	Box* rightSide = new Box(glm::vec2(66, 0), glm::vec2(0), 0.0f, 4.0f, 1.0f, 46.0f, glm::vec4(0.2, 0.2, 0.2 , 1));
	m_physicsScene->AddActor(rightSide);
	rightSide->SetKinematic(true);
	rightSide->SetElasticity(5.0f);

	Box* bottomSide = new Box(glm::vec2(0, -47), glm::vec2(0), 0.0f, 4.0f, 67.0f, 1.0f, glm::vec4(0.2, 0.2, 0.2, 1));
	m_physicsScene->AddActor(bottomSide);
	bottomSide->SetKinematic(true);
	bottomSide->SetElasticity(5.0f);

	// Score Bins
	Box* dividerMidLeft = new Box(glm::vec2(-7, -42), glm::vec2(0), 0.0f, 4.0f, 0.8f, 5.0f, glm::vec4(0.2, 0.2, 0.2, 1));
	m_physicsScene->AddActor(dividerMidLeft);
	dividerMidLeft->SetKinematic(true);
	dividerMidLeft->SetElasticity(5.0f);

	Box* dividerMidRight = new Box(glm::vec2(7, -42), glm::vec2(0), 0.0f, 4.0f, 0.8f, 5.0f, glm::vec4(0.2, 0.2, 0.2, 1));
	m_physicsScene->AddActor(dividerMidRight);
	dividerMidRight->SetKinematic(true);
	dividerMidRight->SetElasticity(5.0f);

	Box* dividerLeft = new Box(glm::vec2(-24, -42), glm::vec2(0), 0.0f, 4.0f, 0.8f, 5.0f, glm::vec4(0.2, 0.2, 0.2, 1));
	m_physicsScene->AddActor(dividerLeft);
	dividerLeft->SetKinematic(true);
	dividerLeft->SetElasticity(5.0f);

	Box* dividerRight = new Box(glm::vec2(24, -42), glm::vec2(0), 0.0f, 4.0f, 0.8f, 5.0f, glm::vec4(0.2, 0.2, 0.2, 1));
	m_physicsScene->AddActor(dividerRight);
	dividerRight->SetKinematic(true);
	dividerRight->SetElasticity(5.0f);

	Box* dividerFarLeft = new Box(glm::vec2(-44, -42), glm::vec2(0), 0.0f, 4.0f, 0.8f, 5.0f, glm::vec4(0.2, 0.2, 0.2, 1));
	m_physicsScene->AddActor(dividerFarLeft);
	dividerFarLeft->SetKinematic(true);
	dividerFarLeft->SetElasticity(5.0f);

	Box* dividerFarRight = new Box(glm::vec2(44, -42), glm::vec2(0), 0.0f, 4.0f, 0.8f, 5.0f, glm::vec4(0.2, 0.2, 0.2, 1));
	m_physicsScene->AddActor(dividerFarRight);
	dividerFarRight->SetKinematic(true);
	dividerFarRight->SetElasticity(5.0f);

	// Point bin triggers
	Box* pointBinMid = new Box(glm::vec2(0, -42), glm::vec2(0), 0.0f, 4.0f, 6.2f, 4.0f, glm::vec4(0, 0.8, 0, 0.5));
	m_physicsScene->AddActor(pointBinMid);
	pointBinMid->SetKinematic(true);
	pointBinMid->SetTrigger(true);

	pointBinMid->triggerEnter = [=](PhysicsObject* other)
	{ 
		SetScore(GetScore() + 1000);
	};

	Box* pointBinMidLeft = new Box(glm::vec2(-15.5, -42), glm::vec2(0), 0.0f, 4.0f, 7.7f, 4.0f, glm::vec4(0, 0.8, 0, 0.4));
	m_physicsScene->AddActor(pointBinMidLeft);
	pointBinMidLeft->SetKinematic(true);
	pointBinMidLeft->SetTrigger(true);

	pointBinMidLeft->triggerEnter = [=](PhysicsObject* other)
	{
		SetScore(GetScore() + 500);
	};

	Box* pointBinMidRight = new Box(glm::vec2(15.5, -42), glm::vec2(0), 0.0f, 4.0f, 7.7f, 4.0f, glm::vec4(0, 0.8, 0, 0.4));
	m_physicsScene->AddActor(pointBinMidRight);
	pointBinMidRight->SetKinematic(true);
	pointBinMidRight->SetTrigger(true);

	pointBinMidRight->triggerEnter = [=](PhysicsObject* other)
	{
		SetScore(GetScore() + 500);
	};

	Box* pointBinLeft = new Box(glm::vec2(-34, -42), glm::vec2(0), 0.0f, 4.0f, 9.2f, 4.0f, glm::vec4(0, 0.8, 0, 0.3));
	m_physicsScene->AddActor(pointBinLeft);
	pointBinLeft->SetKinematic(true);
	pointBinLeft->SetTrigger(true);

	pointBinLeft->triggerEnter = [=](PhysicsObject* other)
	{
		SetScore(GetScore() + 250);
	};

	Box* pointBinRight = new Box(glm::vec2(34, -42), glm::vec2(0), 0.0f, 4.0f, 9.2f, 4.0f, glm::vec4(0, 0.8, 0, 0.3));
	m_physicsScene->AddActor(pointBinRight);
	pointBinRight->SetKinematic(true);
	pointBinRight->SetTrigger(true);

	pointBinRight->triggerEnter = [=](PhysicsObject* other)
	{
		SetScore(GetScore() + 250);
	};

	Box* pointBinFarLeft = new Box(glm::vec2(-54.95, -42), glm::vec2(0), 0.0f, 4.0f, 10.1f, 4.0f, glm::vec4(0, 0.8, 0, 0.2));
	m_physicsScene->AddActor(pointBinFarLeft);
	pointBinFarLeft->SetKinematic(true);
	pointBinFarLeft->SetTrigger(true);

	pointBinFarLeft->triggerEnter = [=](PhysicsObject* other)
	{
		SetScore(GetScore() + 100);
	};

	Box* pointBinFarRight = new Box(glm::vec2(54.95, -42), glm::vec2(0), 0.0f, 4.0f, 10.1f, 4.0f, glm::vec4(0, 0.8, 0, 0.2));
	m_physicsScene->AddActor(pointBinFarRight);
	pointBinFarRight->SetKinematic(true);
	pointBinFarRight->SetTrigger(true);

	pointBinFarRight->triggerEnter = [=](PhysicsObject* other)
	{
		SetScore(GetScore() + 100);
	};
}

void PhysicsProjectApp::SpawnerMovement(float a_deltaTime)
{
	// Spawner Movement
	glm::vec2 spawnerPos = spawner->GetPosition();

	if (spawnerPos.x >= 61)
	{
		leftDir = true;
	}
	else if (spawnerPos.x <= -61)
	{
		leftDir = false;
	}

	spawner->SetPosition(glm::vec2(spawnerPos.x + (leftDir ? -m_spawnerSpeed: m_spawnerSpeed) * a_deltaTime, spawnerPos.y));

	// Spawner creating new pachinko balls
	aie::Input* input = aie::Input::getInstance();

	if (input->wasKeyPressed((aie::EInputCodes::INPUT_KEY_SPACE)) && m_currentBallAmount < m_maxBallAmount)
	{
		Sphere* ball = new Sphere(spawnerPos, glm::vec2(0), 6.0f, 1.4f, glm::vec4(0.4, 0.4, 0.4, 1));
		m_physicsScene->AddActor(ball);
		ball->SetElasticity(1.0f);
		m_currentBallAmount++;
	}
}

void PhysicsProjectApp::SpinningWheelMovement(float a_deltaTime)
{
	// Spinning Wheel Movement
	float wheel1Rot = wheel1->GetRotation();
	wheel1->SetRotation(wheel1Rot + m_spinningSpeed * a_deltaTime);
	wheel1->SetAngularVelocity(wheel1Rot);

	float wheel2Rot = wheel2->GetRotation();
	wheel2->SetRotation(wheel2Rot + m_spinningSpeed * a_deltaTime);
	wheel2->SetAngularVelocity(wheel2Rot);
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