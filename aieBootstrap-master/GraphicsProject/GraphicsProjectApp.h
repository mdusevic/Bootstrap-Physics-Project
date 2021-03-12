#pragma once

#include "Application.h"
#include "Camera.h"
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
	Camera m_camera;

	// Camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::Texture m_gridTexture;

	// --- SHADER ---
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_bunnyShader;
	aie::ShaderProgram m_dragonShader;
	aie::ShaderProgram m_buddhaShader;
	aie::ShaderProgram m_lucyShader;
	aie::ShaderProgram m_spearShader;
	aie::ShaderProgram m_yodaShader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_textureShader;
	aie::ShaderProgram m_normalMapShader;
	// --------------
	// Basic Plane
	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;
	// Create a Bunny with a flat color
	aie::OBJMesh m_bunnyMesh;
	glm::mat4 m_bunnyTransform;
	glm::vec3 m_bunnyPos;
	// Create a Dragon with a flat color
	aie::OBJMesh m_dragonMesh;
	glm::mat4 m_dragonTransform;
	glm::vec3 m_dragonPos;
	// Create a Buddha with a flat color
	aie::OBJMesh m_buddhaMesh;
	glm::mat4 m_buddhaTransform;
	glm::vec3 m_buddhaPos;
	// Create Lucy with a flat color
	aie::OBJMesh m_lucyMesh;
	glm::mat4 m_lucyTransform;
	glm::vec3 m_lucyPos;
	// Create a SoulSpear 
	aie::OBJMesh m_spearMesh;
	glm::mat4 m_spearTransform;
	glm::vec3 m_spearPos;
	// Create a Baby Yoda
	aie::OBJMesh m_yodaMesh;
	glm::mat4 m_yodaTransform;
	glm::vec3 m_yodaPos;

	struct Light
	{
		glm::vec3 direction;
		glm::vec3 color;
	};

	Light m_light;
	glm::vec3 m_ambientLight;

public:
	bool LoadShaderAndMeshLogic();
	void DrawShaderAndMeshes(glm::mat4, glm::mat4);
	void UpdateObjectTransforms();
	void IMGUI_Logic();
};