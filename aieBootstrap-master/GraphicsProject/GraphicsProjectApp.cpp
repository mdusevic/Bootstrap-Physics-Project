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

bool GraphicsProjectApp::startup()
{
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	Light light;
	light.m_color = { 1, 1, 1 };
	light.m_direction = { 1, -1, 1 };

	return LoadShaderAndMeshLogic(light);
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

	m_scene->GetLight().m_direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

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

	//DrawShaderAndMeshes(projectionMatrix, viewMatrix);
	m_scene->Draw();

	Gizmos::draw(projectionMatrix * viewMatrix);
}

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

	m_quadMesh.InitialiseQuad();
	// m_quadMesh.Initialise(6, vertices, 6, indices);

	// We will make the quad 10 units by 10 units 
	m_quadTransform = {
		10,  0,  0,  0,
		 0, 10,  0,  0,
		 0,  0, 10,  0,
		 0,  0,  0,  1
	};

	/*m_quadTransform = glm::rotate(m_quadTransform, glm::radians(90.0f), glm::vec3(1, 0, 0));*/

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

#pragma region GridLogic
	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Failed to load: numbered_grid.tga\n");
		return false;
	}

#pragma endregion

#pragma region Objects
	m_scene = new Scene(&m_camera, glm::vec2(getWindowWidth(), getWindowHeight()), a_light, glm::vec3(0.25f));
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
	m_scene->GetPointLights().push_back(Light(vec3(5, 3, 0), vec3(1, 0, 0), 50)); 
	// Add a green light on the right side
	m_scene->GetPointLights().push_back(Light(vec3(-5, 3, 0), vec3(0, 1, 0), 50));

	return true;

#pragma endregion

}

//void GraphicsProjectApp::DrawShaderAndMeshes(glm::mat4 a_projectionMatrix, glm::mat4 a_viewMatrix)
//{
//	 PVM = Projection View Matrix
//	auto pvm = a_projectionMatrix * a_viewMatrix * glm::mat4(0);
//
//#pragma region Quad
//	 Bind the shader
//	m_textureShader.bind();
//
//	 Bind the transform of the mesh
//	pvm = a_projectionMatrix * a_viewMatrix * m_quadTransform;
//	m_textureShader.bindUniform("ProjectionViewModel", pvm);
//
//	 Bind the texture to a location of your choice (0)
//	m_textureShader.bindUniform("diffuseTexture", 0);
//
//	 Bind the texture to the specified location
//	m_gridTexture.bind(0);
//
//	 Draw the quad
//	m_quadMesh.Draw();
//
//#pragma endregion
//
//#pragma region FlatBunny
//	m_bunnyShader.bind();
//	pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
//	m_bunnyShader.bindUniform("ProjectionViewModel", pvm);
//	m_bunnyShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 0, 1));
//
//	 Draw bunny mesh
//	 m_bunnyMesh.draw();
//
//#pragma endregion
//
//#pragma region Dragon
//	m_dragonShader.bind();
//	pvm = a_projectionMatrix * a_viewMatrix * m_dragonTransform;
//	m_dragonShader.bindUniform("ProjectionViewModel", pvm);
//	m_dragonShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 0, 1));
//
//	 Draw dragon mesh
//	 m_dragonMesh.draw();
//
//#pragma endregion
//
//#pragma region Buddha
//	m_buddhaShader.bind();
//	pvm = a_projectionMatrix * a_viewMatrix * m_buddhaTransform;
//	m_buddhaShader.bindUniform("ProjectionViewModel", pvm);
//	m_buddhaShader.bindUniform("MeshFlatColor", glm::vec4(1, 0, 1, 1));
//
//	 Draw buddha mesh
//	 m_buddhaMesh.draw();
//
//#pragma endregion
//
//#pragma region Lucy
//	m_lucyShader.bind();
//	pvm = a_projectionMatrix * a_viewMatrix * m_lucyTransform;
//	m_lucyShader.bindUniform("ProjectionViewModel", pvm);
//	m_lucyShader.bindUniform("MeshFlatColor", glm::vec4(0, 1, 1, 1));
//
//	 Draw lucy mesh
//	 m_lucyMesh.draw();
//
//#pragma endregion
//
//#pragma region SoulSpear
//	m_normalMapShader.bind();
//
//	 Bind the camera position
//	m_normalMapShader.bindUniform("CameraPosition", m_camera.GetPosition());
//
//	 Bind the light
//	m_normalMapShader.bindUniform("AmbientColor", m_ambientLight);
//	m_normalMapShader.bindUniform("LightColor", m_light.color);
//	m_normalMapShader.bindUniform("LightDirection", m_light.direction);
//
//	m_normalMapShader.bindUniform("ModelMatrix", m_spearTransform);
//
//	pvm = a_projectionMatrix * a_viewMatrix * m_spearTransform;
//	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
//
//	 Draw spear mesh
//	m_spearMesh.draw();
//
//#pragma endregion
//
//#pragma region Baby Yoda
//	 m_normalMapShader.bind();
//
//	  Bind the camera position
//	 m_normalMapShader.bindUniform("CameraPosition", m_camera.GetPosition());
//
//	  Bind the light
//	 m_normalMapShader.bindUniform("AmbientColor", m_ambientLight);
//	 m_normalMapShader.bindUniform("LightColor", m_light.color);
//	 m_normalMapShader.bindUniform("LightDirection", m_light.direction);
//
//	 m_normalMapShader.bindUniform("ModelMatrix", m_yodaTransform);
//
//	 pvm = a_projectionMatrix * a_viewMatrix * m_yodaTransform;
//	 m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
//
//	  Draw yoda mesh
//	 m_yodaMesh.draw();
//
//#pragma endregion
//
//#pragma region Grenade
//	 m_normalMapShader.bind();
//
//	  Bind the camera position
//	 m_normalMapShader.bindUniform("CameraPosition", m_camera.GetPosition());
//
//	  Bind the light
//	 m_normalMapShader.bindUniform("AmbientColor", m_ambientLight);
//	 m_normalMapShader.bindUniform("LightColor", m_light.color);
//	 m_normalMapShader.bindUniform("LightDirection", m_light.direction);
//
//	 m_normalMapShader.bindUniform("ModelMatrix", m_yodaTransform);
//
//	 pvm = a_projectionMatrix * a_viewMatrix * m_yodaTransform;
//	 m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
//
//	  Draw grenade mesh
//	 m_grenadeMesh.draw();
//
//#pragma endregion
//
//#pragma region Phong
//	 Bind the shader
//	m_phongShader.bind();
//
//	 Bind the camera position
//	m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(a_viewMatrix)[3]));
//
//	 Bind the light
//	m_phongShader.bindUniform("AmbientColor", m_ambientLight);
//	m_phongShader.bindUniform("LightColor", m_light.color);
//	m_phongShader.bindUniform("LightDirection", m_light.direction);
//
//	 --- BUNNY ---
//	 Bind the PVM
//	pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
//	m_phongShader.bindUniform("ProjectionViewModel", pvm);
//
//	 Bind the lighting transforms
//	m_phongShader.bindUniform("ModelMatrix", m_bunnyTransform);
//
//	 Draw mesh
//	m_bunnyMesh.draw();
//
//	 --- DRAGON ---
//	 Bind the PVM
//	pvm = a_projectionMatrix * a_viewMatrix * m_dragonTransform;
//	m_phongShader.bindUniform("ProjectionViewModel", pvm);
//
//	 Bind the lighting transforms
//	m_phongShader.bindUniform("ModelMatrix", m_dragonTransform);
//
//	 Draw mesh
//	m_dragonMesh.draw();
//
//	 --- BUDDHA ---
//     Bind the PVM
//	pvm = a_projectionMatrix * a_viewMatrix * m_buddhaTransform;
//	m_phongShader.bindUniform("ProjectionViewModel", pvm);
//
//	 Bind the lighting transforms
//	m_phongShader.bindUniform("ModelMatrix", m_buddhaTransform);
//
//	 Draw mesh
//	m_buddhaMesh.draw();
//
//	 --- LUCY ---
//     Bind the PVM
//	pvm = a_projectionMatrix * a_viewMatrix * m_lucyTransform;
//	m_phongShader.bindUniform("ProjectionViewModel", pvm);
//
//	 Bind the lighting transforms
//	m_phongShader.bindUniform("ModelMatrix", m_lucyTransform);
//
//	 Draw mesh
//	m_lucyMesh.draw();
//
//#pragma endregion
//
//}

void GraphicsProjectApp::IMGUI_Logic() 
{
	ImGui::Begin("Scene Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_scene->GetLight().m_direction[0], 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat3("Sunlight Color", &m_scene->GetLight().m_color[0], 0.1f, 0.0f, 2.0f);
	ImGui::End();

	ImGui::Begin("Objects");
	
	std::vector<Instance*> Objects;
	for (Instance* const& c : m_scene->GetAllInstances())
	{
		Objects.push_back(c);
	}

	for each (auto obj in Objects)
	{
		glm::vec3 scale = glm::vec3(1.0f);
		glm::quat rotation = glm::quat();
		glm::vec3 translation = glm::vec3(1.0f);
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(obj->GetTransform(), scale, rotation, translation, skew, perspective);
		glm::vec3 rot = obj->GetRotation();

		if (ImGui::CollapsingHeader(obj->GetObjectName().c_str()))
		{
			ImGui::Indent();
			if (ImGui::TreeNode("Transform"))
			{
				if (ImGui::DragFloat3("Position:", &translation[0], 0.1f))
				{

				}

				if (ImGui::DragFloat3("Rotation:", &rot[0], 0.1f, -180.0f, 180.0f))
				{
					obj->SetRotation(rot);
				}

				if (ImGui::DragFloat3("Scale:", &scale[0], 0.1f))
				{

				}

				obj->SetTransform(obj->MakeTransform(translation, rot, scale));

				ImGui::TreePop();
			}
			ImGui::Unindent();
		}
	}
	
	ImGui::End();

	ImGui::Begin("Lighting");

	ImGui::End();
}
