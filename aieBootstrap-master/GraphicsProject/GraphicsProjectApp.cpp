#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"

#define GLM_ENABLE_EXPERIMENTAL 1

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

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

bool GraphicsProjectApp::startup()
{
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	m_light.color = { 1, 1, 1 };
	m_ambientLight = { 0.25f, 0.25f, 0.25f};

	return LoadShaderAndMeshLogic();
}

void GraphicsProjectApp::shutdown()
{
	Gizmos::destroy();
}

void GraphicsProjectApp::update(float deltaTime)
{
	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
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

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	m_camera.Update(deltaTime);

	IMGUI_Logic();

	float time = getTime();

	m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

	//SolarSystem(deltaTime);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsProjectApp::draw()
{
	// wipe the screen to the background colour
	clearScreen();
	
	glm::mat4 projectionMatrix = m_camera.GetProjectionMatrix(getWindowWidth(), (float)getWindowHeight());
	glm::mat4 viewMatrix = m_camera.GetViewMatrix();

	// update perspective based on screen size
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	DrawShaderAndMeshes(projectionMatrix, viewMatrix);

	Gizmos::draw(projectionMatrix * viewMatrix);
}

void GraphicsProjectApp::SolarSystem(float dt)
{
	// Sun
	Gizmos::addSphere(glm::vec3(0, 0, 0), 1.0f, 16, 16, glm::vec4(1, 0.8, 0, 1));

	//Mercury
	/*glm::vec3 mercuryPos = glm::rotate(10.0f, glm::vec3(2, 0, 0));*/
	Gizmos::addSphere(glm::vec3(2, 0, 0), 0.15f, 16, 16, glm::vec4(1, 0.8, 0, 1));
}

bool GraphicsProjectApp::LoadShaderAndMeshLogic()
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

	// Define the 6 vertices for the 2 triangles that make the quad
	//Mesh::Vertex verticesNoIndex[6];
	//verticesNoIndex[0].position = { -0.5f, 0.0f,  0.5f, 1.0f };
	//verticesNoIndex[1].position = { 0.5f, 0.0f,  0.5f, 1.0f };
	//verticesNoIndex[2].position = { -0.5f, 0.0f, -0.5f, 1.0f };

	//verticesNoIndex[3].position = { -0.5f, 0.0f, -0.5f, 1.0f };
	//verticesNoIndex[4].position = { 0.5f, 0.0f,  0.5f, 1.0f };
	//verticesNoIndex[5].position = { 0.5f, 0.0f, -0.5f, 1.0f };

	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0.0f,  0.5f, 1.0f };
	vertices[1].position = { 0.5f, 0.0f,  0.5f, 1.0f };
	vertices[2].position = { -0.5f, 0.0f, -0.5f, 1.0f };
	vertices[3].position = { 0.5f, 0.0f, -0.5f, 1.0f };

	unsigned int indices[6] = { 0, 1, 2, 2, 1, 3 };

	m_quadMesh.Initialise(6, vertices, 6, indices);

	// We will make the quad 10 units by 10 units 
	m_quadTransform = {
		10,  0,  0,  0,
		 0, 10,  0,  0,
		 0,  0, 10,  0,
		 0,  0,  0,  1
	};

	/*m_quadTransform = glm::rotate(m_quadTransform, glm::radians(90.0f), glm::vec3(1, 0, 0));*/

#pragma endregion

#pragma region FlatBunny
	m_bunnyShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_bunnyShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_bunnyShader.link())
	{
		printf("Bunny Shader had an error: %s\n", m_bunnyShader.getLastError());
		return false;
	}

	if (m_bunnyMesh.load("./stanford/bunny.obj") == false)
	{
		printf("Bunny Mesh Failed!\n");
		return false;
	}

	m_bunnyPos = { -8, 0, 0 };

#pragma endregion

#pragma region Dragon
	m_dragonShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_dragonShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_dragonShader.link())
	{
		printf("Dragon Shader had an error: %s\n", m_dragonShader.getLastError());
		return false;
	}

	if (m_dragonMesh.load("./stanford/dragon.obj") == false)
	{
		printf("Dragon Mesh Failed!\n");
		return false;
	}

	m_dragonPos = { -3, 0, 0 };

#pragma endregion

#pragma region Buddha
	m_buddhaShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_buddhaShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_buddhaShader.link())
	{
		printf("Buddha Shader had an error: %s\n", m_buddhaShader.getLastError());
		return false;
	}

	if (m_buddhaMesh.load("./stanford/buddha.obj") == false)
	{
		printf("Dragon Mesh Failed!\n");
		return false;
	}

	m_buddhaPos = { 3, 0, 0 };

#pragma endregion

#pragma region Lucy
	m_lucyShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	// Load the fragment shader from a file
	m_lucyShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_lucyShader.link())
	{
		printf("Buddha Shader had an error: %s\n", m_lucyShader.getLastError());
		return false;
	}

	if (m_lucyMesh.load("./stanford/lucy.obj") == false)
	{
		printf("Dragon Mesh Failed!\n");
		return false;
	}

	m_lucyPos = { 6, 0, 0 };

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
	
	return true;
}

void GraphicsProjectApp::DrawShaderAndMeshes(glm::mat4 a_projectionMatrix, glm::mat4 a_viewMatrix)
{
	// PVM = Projection View Matrix
	auto pvm = a_projectionMatrix * a_viewMatrix * glm::mat4(0);

#pragma region Quad
	// Bind the shader
	m_simpleShader.bind();

	// Bind the transform of the mesh
	pvm = a_projectionMatrix * a_viewMatrix * m_quadTransform;
	m_simpleShader.bindUniform("ProjectionViewModel", pvm);

	m_quadMesh.Draw();

#pragma endregion

#pragma region FlatBunny
	m_bunnyShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
	m_bunnyShader.bindUniform("ProjectionViewModel", pvm);
	m_bunnyShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 0, 1));

	// Draw bunny mesh
	// m_bunnyMesh.draw();

#pragma endregion

#pragma region Dragon
	m_dragonShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_dragonTransform;
	m_dragonShader.bindUniform("ProjectionViewModel", pvm);
	m_dragonShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 0, 1));

	// Draw dragon mesh
	// m_dragonMesh.draw();

#pragma endregion

#pragma region Buddha
	m_buddhaShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_buddhaTransform;
	m_buddhaShader.bindUniform("ProjectionViewModel", pvm);
	m_buddhaShader.bindUniform("MeshFlatColor", glm::vec4(1, 0, 1, 1));

	// Draw buddha mesh
	// m_buddhaMesh.draw();

#pragma endregion

#pragma region Lucy
	m_lucyShader.bind();
	pvm = a_projectionMatrix * a_viewMatrix * m_lucyTransform;
	m_lucyShader.bindUniform("ProjectionViewModel", pvm);
	m_lucyShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 1, 1));

	// Draw lucy mesh
	// m_lucyMesh.draw();

#pragma endregion

#pragma region Phong
	// Bind the shader
	m_phongShader.bind();

	// Bind the camera position
	m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(a_viewMatrix)[3]));

	// Bind the light
	m_phongShader.bindUniform("AmbientColor", m_ambientLight);
	m_phongShader.bindUniform("LightColor", m_light.color);
	m_phongShader.bindUniform("LightDirection", m_light.direction);

	// --- BUNNY ---
	// Bind the PVM
	pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the lighting transforms
	m_phongShader.bindUniform("ModelMatrix", m_bunnyTransform);

	// Draw mesh
	m_bunnyMesh.draw();

	// --- DRAGON ---
	// Bind the PVM
	pvm = a_projectionMatrix * a_viewMatrix * m_dragonTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the lighting transforms
	m_phongShader.bindUniform("ModelMatrix", m_dragonTransform);

	// Draw mesh
	m_dragonMesh.draw();

	// --- BUDDHA ---
    // Bind the PVM
	pvm = a_projectionMatrix * a_viewMatrix * m_buddhaTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the lighting transforms
	m_phongShader.bindUniform("ModelMatrix", m_buddhaTransform);

	// Draw mesh
	m_buddhaMesh.draw();

	// --- LUCY ---
    // Bind the PVM
	pvm = a_projectionMatrix * a_viewMatrix * m_lucyTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	// Bind the lighting transforms
	m_phongShader.bindUniform("ModelMatrix", m_lucyTransform);

	// Draw mesh
	m_lucyMesh.draw();

#pragma endregion
}

void GraphicsProjectApp::UpdateObjectTransforms()
{
	// --- BUNNY ---
	m_bunnyTransform = {
		0.5f,     0,     0,  0,
		   0,  0.5f,     0,  0,
		   0,     0,  0.5f,  0,
	    m_bunnyPos.x, m_bunnyPos.y, m_bunnyPos.z, 1
	};
	
	// --- DRAGON ---
	m_dragonTransform = {
		0.5f,     0,     0,  0,
		   0,  0.5f,     0,  0,
		   0,     0,  0.5f,  0,
	    m_dragonPos.x, m_dragonPos.y, m_dragonPos.z, 1
	};

	// --- BUDDHA ---
	m_buddhaTransform = {
		 0.5f,     0,     0,  0,
			0,  0.5f,     0,  0,
			0,     0,  0.5f,  0,
		 m_buddhaPos.x, m_buddhaPos.y, m_buddhaPos.z, 1
	};

	// --- LUCY ---
	m_lucyTransform = {
		0.5f,     0,     0,  0,
		   0,  0.5f,     0,  0,
		   0,     0,  0.5f,  0,
	    m_lucyPos.x, m_lucyPos.y, m_lucyPos.z, 1
	};
}

void GraphicsProjectApp::IMGUI_Logic()
{
	ImGui::Begin("Scene Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat3("Sunlight Color", &m_light.color[0], 0.1f, 0.0f, 2.0f);
	ImGui::End();

	ImGui::Begin("Object Settings");
	ImGui::DragFloat3("Bunny Position", &m_bunnyPos[0], 0.1f);
	ImGui::DragFloat3("Dragon Position", &m_dragonPos[0], 0.1f);
	ImGui::DragFloat3("Buddha Position", &m_buddhaPos[0], 0.1f);
	ImGui::DragFloat3("Lucy Position", &m_lucyPos[0], 0.1f);

	UpdateObjectTransforms();

	ImGui::End();
}
