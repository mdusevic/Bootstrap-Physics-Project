#pragma once

#include "Application.h"
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"
#include <glm/mat4x4.hpp>

class GraphicsProjectApp : public aie::Application {
public:

	GraphicsProjectApp();
	virtual ~GraphicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void SolarSystem(float dt);

protected:
	// Camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	// --- SHADER ---
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_bunnyShader;
	aie::ShaderProgram m_dragonShader;
	// --------------
	// Basic Plane
	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;
	// Create a Bunny with a flat color
	aie::OBJMesh m_bunnyMesh;
	glm::mat4 m_bunnyTransform;
	// Create a Dragon with a flat color
	aie::OBJMesh m_dragonMesh;
	glm::mat4 m_dragonTransform;

public:
	bool LoadShaderAndMeshLogic();
	void DrawShaderAndMeshes(glm::mat4, glm::mat4);
};