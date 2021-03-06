/*
 * File:	GraphicsProjectApp.cpp
 *
 * Author: Mara Dusevic (s200494@students.aie.edu.au)
 * Date Created: Wednesday 10 March 2021
 * Date Last Modified Thursday 1 April 2021
 *
 * This file displays a scene created to display the features
 * added to the graphics engine.
 *
 */

#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"
#include "Scene.h"
#include "Instance.h"

#define GLM_ENABLE_EXPERIMENTAL 1

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsProjectApp::GraphicsProjectApp()
{

}

GraphicsProjectApp::~GraphicsProjectApp()
{

}

// Startup Function
bool GraphicsProjectApp::startup()
{
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// Initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// Create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	Light light;
	light.m_color = { 1, 1, 1 };
	light.m_direction = { 1, -1, 1 };

	return LoadShaderAndMeshLogic(light);
}

// Shutdown Function
void GraphicsProjectApp::shutdown()
{
	Gizmos::destroy();
}

// Update Function
void GraphicsProjectApp::update(float deltaTime)
{
	// Wipe the gizmos clean for this frame
	Gizmos::clear();

	// Draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	// Add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	m_scene->SetCamera(m_camera);

	m_camera->Update(deltaTime);

	IMGUI_Logic();

	// Quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
	{
		quit();
	}

	#pragma region Cameras Switching

	if (input->wasKeyPressed(aie::INPUT_KEY_TAB))
	{
		m_cameraID++;

		if (m_cameraID > 3)
		{
			m_cameraID = 0;
		}

		m_camera = m_scene->GetCameras().at(m_cameraID);
	}

	#pragma endregion

}

// Draw Function
void GraphicsProjectApp::draw()
{
	if (EnablePostShader)
	{
		// Bind our render target
		m_renderTarget.bind();
	}

	// Wipe the screen to the background colour
	clearScreen();
	
	glm::mat4 projectionMatrix = m_camera->GetProjectionMatrix(getWindowWidth(), (float)getWindowHeight());
	glm::mat4 viewMatrix = m_camera->GetViewMatrix();

	m_scene->Draw();

	Gizmos::draw(projectionMatrix * viewMatrix);

	if (EnablePostShader)
	{
		// Unbind target to return to backbuffer
		m_renderTarget.unbind();

		// Clear the backbuffer
		clearScreen();

		m_postShader.bind();
		m_postShader.bindUniform("ColorTarget", 0);
		m_postShader.bindUniform("SwitchEffect", SwitchPostEffect);
		m_renderTarget.getTarget(0).bind(0);

		// Fullscreen Quad
		m_fullscreenQuad.Draw();
	}
}

// Loads shaders and meshes
bool GraphicsProjectApp::LoadShaderAndMeshLogic(Light a_light)
{

	#pragma region Quad
	// Load the vertex shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_simpleShader.link())
	{
		printf("Simple Shader had an error: %s\n", m_simpleShader.getLastError());
		return false;
	}

	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0.0f,  0.5f, 1.0f };
	vertices[1].position = { 0.5f, 0.0f,  0.5f, 1.0f };
	vertices[2].position = { -0.5f, 0.0f, -0.5f, 1.0f };
	vertices[3].position = { 0.5f, 0.0f, -0.5f, 1.0f };

	unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };

	m_quadMesh.InitialiseQuad();

	// We will make the quad 10 units by 10 units 
	m_quadTransform = {
		10,  0,  0,  0,
		 0, 10,  0,  0,
		 0,  0, 10,  0,
		 0,  0,  0,  1
	};

#pragma endregion

	#pragma region Dragon

	if (m_dragonMesh.load("./stanford/dragon.obj") == false)
	{
		printf("Dragon Mesh Failed!\n");
		return false;
	}

#pragma endregion

	#pragma region Grenade

	if (m_grenadeMesh.load("./grenade/uploads_files_2503290_Hand_Grenade.obj", true, true) == false)
	{
		printf("Grenade Mesh Failed!\n");
		return false;
	}

#pragma endregion
	
	#pragma region SoulSpear

	if (m_spearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soul spear Mesh Failed!\n");
		return false;
	}

#pragma endregion
	
	#pragma region Phong
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");

	if (m_phongShader.link() == false)
	{
		printf("Phong Shader has an error: %s\n", m_phongShader.getLastError());
		return false;
	}

#pragma endregion
	
	#pragma region TextureShader
	m_textureShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_textureShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");

	if (m_textureShader.link() == false)
	{
		printf("Texture Shader has an error: %s\n", m_textureShader.getLastError());
		return false;
	}

#pragma endregion
	
	#pragma region NormalMapShader
	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");

	if (m_normalMapShader.link() == false)
	{
		printf("Normal Map Shader has an error: %s\n", m_normalMapShader.getLastError());
		return false;
	}

#pragma endregion
	
	#pragma region Post-processingShader
	
	if (m_renderTarget.initialise(1, (float)getWindowWidth(), (float)getWindowHeight()) == false)
	{ 
		printf("Render Target Error!\n"); 
		return false; 
	}

	// Create a fullscreen quad
	m_fullscreenQuad.InitialiseFullscreenQuad();

	// Load a processing shader
	m_postShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/post.vert");
	m_postShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/post.frag");

	if (m_postShader.link() == false)
	{
		printf("Post Shader has an error: %s\n", m_postShader.getLastError());
		return false;
	}

#pragma endregion
	
	#pragma region GridLogic
	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Failed to load: numbered_grid.tga\n");
		return false;
	}

#pragma endregion
	
	#pragma region Objects
	m_scene = new Scene(m_camera, glm::vec2(getWindowWidth(), getWindowHeight()), a_light, glm::vec3(0.25f));
	int count = 0;

	// Row of spears
	for (int i = 0; i < 10; i++)
	{
		count++;
		m_scene->AddInstance(new Instance("Spear" + std::to_string(count) , glm::vec3(i * 2, 0, 0), glm::vec3(0, i * 30, 0), glm::vec3(1, 1, 1), &m_spearMesh, &m_normalMapShader));
	}

	// Dragon
	m_scene->AddInstance(new Instance("Dragon", glm::vec3(5, 0, -5), glm::vec3(0, 0, 0), glm::vec3(0.4, 0.4, 0.4), &m_dragonMesh, &m_phongShader));

	// Grenade
	m_scene->AddInstance(new Instance("Grenade", glm::vec3(5, 0, 5), glm::vec3(0, 0, 0), glm::vec3(20, 20, 20), &m_grenadeMesh, &m_normalMapShader));

#pragma endregion
	
	#pragma region Lights

	// Add a red light on the left side
	m_scene->GetPointLights().push_back(Light("Red Light", glm::vec3(5, 3, 0), glm::vec3(1, 0, 0), 50));
	// Add a green light on the right side
	m_scene->GetPointLights().push_back(Light("Green Light", glm::vec3(-5, 3, 0), glm::vec3(0, 1, 0), 50));

#pragma endregion
	
	#pragma region Cameras

	// Flyby Cam
	Camera* m_flybyCam = new Camera(glm::vec3(-10, 2, 0), 0.0f, 0.0f, true);
	m_scene->AddCamera(m_flybyCam);

	// X-axis Cam
	Camera* m_xAxisCam = new Camera(glm::vec3(-20, 4, 0), 0.0f, 0.0f, false);
	m_scene->AddCamera(m_xAxisCam);

	// Y-axis Cam
	Camera* m_yAxisCam = new Camera(glm::vec3(0, 20, 0), 0.0f, -90.0f, false);
	m_scene->AddCamera(m_yAxisCam);

	// Z-axis Cam
	Camera* m_zAxisCam = new Camera(glm::vec3(0, 4, 20), -90.0f, 0.0f, false);
	m_scene->AddCamera(m_zAxisCam);

	// Default cam is flyby cam
	m_camera = m_flybyCam;

	return true;

#pragma endregion

}
  
// Creates an editor at runtime to edit scene
void GraphicsProjectApp::IMGUI_Logic() 
{

	#pragma region Scene Settings IMGUI

	// Creates window to display settings to edit aspects that affect the whole scene
	ImGui::Begin("Scene Settings");

	// Scene Light Editor
	if (ImGui::CollapsingHeader("Light Settings"))
	{
		ImGui::Indent();

		// Edits directional light's direction
		ImGui::DragFloat3("Sunlight Direction", &m_scene->GetLight().m_direction[0], 0.1f, -1.0f, 1.0f);
		
		// Edits directional light's colour
		ImGui::DragFloat3("Sunlight Color", &m_scene->GetLight().m_color[0], 0.1f, 0.0f, 2.0f);
		
		// Edits the scene's ambient light colour
		ImGui::DragFloat3("Ambient Color", &m_scene->GetAmbientLight()[0], 0.1f, 0.0f, 2.0f);
		
		ImGui::Unindent();
	}

	// Camera Editor
	if (ImGui::CollapsingHeader("Camera Settings"))
	{
		ImGui::Indent();
		// Changes scene camera to flybuy camera
		if (ImGui::Button("Flybuy Camera"))
		{
			m_camera = m_scene->GetCameras().at(0);
		}
		ImGui::SameLine();
		
		// Changes scene camera to on x-axis
		if (ImGui::Button("X-axis Camera"))
		{
			m_camera = m_scene->GetCameras().at(1);
		}
		ImGui::SameLine();
		
		// Changes scene camera to on y-axis
		if (ImGui::Button("Y-axis Camera"))
		{
			m_camera = m_scene->GetCameras().at(2);
		}
		ImGui::SameLine();

		// Changes scene camera to on z-axis
		if (ImGui::Button("Z-axis Camera"))
		{
			m_camera = m_scene->GetCameras().at(3);
		}

		ImGui::Unindent();
	}

	// Post-processing Editor
	if (ImGui::CollapsingHeader("Post-processing Settings"))
	{
		// Used to turn on and off the post-processing
		ImGui::Checkbox("Enable Post-processing", &EnablePostShader);

		// Switches post-processing shader to distort the scene
		if (ImGui::Button("Distort"))
		{
			SwitchPostEffect = false;
		}
		ImGui::SameLine();

		// Switches post-processing shader to blur the scene
		if (ImGui::Button("Blur"))
		{
			SwitchPostEffect = true;
		}
		ImGui::SameLine();
	}

	ImGui::End();

	#pragma endregion

	#pragma region Object Inspector IMGUI

	int a = 0;

	// Creates window to display settings to edit objects' transforms in scene
	ImGui::Begin("Objects");
	
	// Pushes all objects into new vector, allows for easier call of each object
	std::vector<Instance*> Objects;
	for (Instance* const& c : m_scene->GetAllInstances())
	{
		Objects.push_back(c);
	}

	// Loops through all objects
	for each (auto obj in Objects)
	{
		// Uses glm::decompose to separate matrix into each individual variable
		glm::vec3 scale = glm::vec3(1.0f);
		glm::quat rotation = glm::quat();
		glm::vec3 translation = glm::vec3(1.0f);
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(obj->GetTransform(), scale, rotation, translation, skew, perspective);
		glm::vec3 rot = obj->GetRotation();

		// PushID for header
		ImGui::PushID(a);

		// For each object, its name is used to create a collapsible header
		if (ImGui::CollapsingHeader(obj->GetObjectName().c_str()))
		{
			ImGui::Indent();

			// PushID for treenode
			a++;
			ImGui::PushID(a);

			// Objects Transform Editor
			if (ImGui::TreeNode("Transform"))
			{
				// Sets objects new position to editor value
				ImGui::DragFloat3("Position", &translation[0], 0.1f);

				// Sets objects new rotation to editor value
				if (ImGui::DragFloat3("Rotation", &rot[0], 0.1f, -180.0f, 180.0f))
				{
					obj->SetRotation(rot);
				}

				// Sets objects new scale to editor value
				ImGui::DragFloat3("Scale", &scale[0], 0.1f);

				// Sets the object's new transform with all editor values
				obj->SetTransform(obj->MakeTransform(translation, rot, scale));

				ImGui::TreePop();
			}

			ImGui::Unindent();
			
			// PopID for treenode
			ImGui::PopID();
		}

		// PopID for header
		ImGui::PopID();
		a++;
	}
	
	ImGui::End();

	#pragma endregion
	
	#pragma region Lighting Inspector IMGUI

	// Creates window to display settings to edit individual lights in scene
	ImGui::Begin("Lighting");

	a++;

	// Loops through all point lights 
	for (auto i = m_scene->GetPointLights().begin(); i != m_scene->GetPointLights().end(); i++)
	{
		// PushID for header
		ImGui::PushID(a);

		// For each light, its name is used to create a collapsible header
		if (ImGui::CollapsingHeader(i->GetLightName().c_str()))
		{
			ImGui::Indent();

			// PushID for treenode
			a++;
			ImGui::PushID(a);

			// Lights Transform Editor
			if (ImGui::TreeNode("Transform"))
			{
				// Sets lights new position to editor value
				ImGui::DragFloat3("Position", &i->GetLightPosition()[0], 0.1f);

				ImGui::TreePop();
			}

			// PopID for treenode
			ImGui::PopID();

			ImGui::Unindent();

			ImGui::Indent();

			// PushID for treenode
			a++;
			ImGui::PushID(a);

			// Lights Colour Editor
			if (ImGui::TreeNode("Color"))
			{
				// Sets lights new colour to editor value
				ImGui::DragFloat3("Color", &i->GetLightColor()[0], 0.1f, 0.0f, 300.0f);

				ImGui::TreePop();
			}

			// PopID for treenode
			ImGui::PopID();

			ImGui::Unindent();
		}
		
		// PopID for header
		ImGui::PopID();
		a++;
	}

	ImGui::End();

	#pragma endregion

}