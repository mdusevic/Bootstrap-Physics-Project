#pragma once

#include "Application.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"
#include "RenderTarget.h"
#include "Scene.h"

#include <glm/mat4x4.hpp>

class GraphicsProjectApp : public aie::Application
{
public:
	GraphicsProjectApp();
	virtual ~GraphicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	Camera* m_camera;

	// Camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::Texture m_gridTexture;

	// --- SHADER ---
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_dragonShader;
	aie::ShaderProgram m_spearShader;
	aie::ShaderProgram m_grenadeShader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_textureShader;
	aie::ShaderProgram m_normalMapShader;
	aie::ShaderProgram m_postShader;
	// --------------

	// Basic Plane
	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;
	// Fullscreen Quad
	Mesh m_fullscreenQuad;
	// Create a Dragon with a flat color
	aie::OBJMesh m_dragonMesh;
	// Create a SoulSpear 
	aie::OBJMesh m_spearMesh;
	// Create a Grenade
	aie::OBJMesh m_grenadeMesh;

	Scene* m_scene;

	// Light Time
	float m_time = 0.5f;

	// Used to switch between cameras
	int m_cameraID = 0;

	// Used to render post-processing
	aie::RenderTarget m_renderTarget;
	bool EnablePostShader = false;
	bool SwitchPostEffect = false;

public:
	bool LoadShaderAndMeshLogic(Light a_light);
	void IMGUI_Logic();
};