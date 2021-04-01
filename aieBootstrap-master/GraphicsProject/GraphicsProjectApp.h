/*
 * File:	GraphicsProjectApp.h
 *
 * Author: Mara Dusevic (s200494@students.aie.edu.au)
 * Date Created: Wednesday 10 March 2021
 * Date Last Modified Thursday 1 April 2021
 *
 * This file creates the variables used in the corresponding
 * .cpp file that displays the features implemented in the
 * graphics engine.
 *
 */

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

	// Startup Function
	virtual bool startup();
	// Shutdown Function
	virtual void shutdown();
	// Update Function
	virtual void update(float deltaTime);
	// Draw Function
	virtual void draw();

protected:
	Camera* m_camera;

	// Camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::Texture m_gridTexture;

	// ------ SHADERS ------
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_dragonShader;
	aie::ShaderProgram m_spearShader;
	aie::ShaderProgram m_grenadeShader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_textureShader;
	aie::ShaderProgram m_normalMapShader;
	aie::ShaderProgram m_postShader;
	// ---------------------

	// Basic Plane
	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;
	// Fullscreen Quad
	Mesh m_fullscreenQuad;
	// Dragon mesh
	aie::OBJMesh m_dragonMesh;
	// SoulSpear mesh 
	aie::OBJMesh m_spearMesh;
	// Grenade mesh
	aie::OBJMesh m_grenadeMesh;

	// Creates a new scene to display everything within
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
	// Loads shaders and meshes 
	bool LoadShaderAndMeshLogic(Light a_light);

	// Used to create an editor to edit things in scene
	void IMGUI_Logic();
};