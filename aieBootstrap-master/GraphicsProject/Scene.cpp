/*
 * File:	Scene.cpp
 *
 * Author: Mara Dusevic (s200494@students.aie.edu.au)
 * Date Created: Thursday 18 March 2021
 * Date Last Modified Thursday 1 April 2021
 *
 * This file creates and sets functions used by any
 * instance of a scene. It takes in objects, lights and 
 * cameras that could be included in scene.
 *
 */

#include "Scene.h"
#include "Instance.h"

Scene::Scene(Camera* a_camera, glm::vec2 a_windowsSize, Light& a_light, glm::vec3 a_ambientLight)
{
	m_camera = a_camera;
	m_windowSize = a_windowsSize;
	m_light = a_light;
	m_ambientLight = a_ambientLight;
}

Scene::~Scene()
{
	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
	{
		delete(*i);
	}
}

// Adds instance to list of instances in scene
void Scene::AddInstance(Instance* a_instance)
{
	m_instances.push_back(a_instance);
}

// Adds camera to list of cameras in scene
void Scene::AddCamera(Camera* a_camera)
{
	m_cameras.push_back(a_camera);
}

// Draw Function
void Scene::Draw()
{
	// Loops through all the point lights and adds their values to different lists
	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++)
	{
		m_pointLightNames[i] = m_pointLights[i].m_name;
		m_pointLightPositions[i] = m_pointLights[i].m_direction;
		m_pointLightColors[i] = m_pointLights[i].m_color;

		// Creates a sphere to indicate where the light is
		aie::Gizmos::addSphere(m_pointLightPositions[i], 0.5f, 8, 8, glm::vec4(m_pointLightColors[i].x, m_pointLightColors[i].y, m_pointLightColors[i].z, 1));
	}

	// Loops through all instances and draws them in scene
	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
	{
		Instance* instance = *i;
		instance->Draw(this);
	}
}
