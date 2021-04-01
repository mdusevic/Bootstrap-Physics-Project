/*
 * File:	Scene.h
 *
 * Author: Mara Dusevic (s200494@students.aie.edu.au)
 * Date Created: Thursday 18 March 2021
 * Date Last Modified Thursday 1 April 2021
 *
 * This file creates the variables used in the corresponding
 * .cpp file that creates and sets functions used by any instance
 * of a scene.
 *
 */

#pragma once

#include "Gizmos.h"

#include <list>
#include <vector>
#include <glm/glm.hpp>
#include <string>

class Camera;
class Instance;

const int MAX_LIGHTS = 4;

// Light struct
struct Light
{
	Light()
	{
		m_name = std::string("Light");
		m_direction = glm::vec3(1);
		m_color = glm::vec3(1);
		m_intensity = 10.0f;
	};

	Light(std::string a_name, glm::vec3 a_pos, glm::vec3 a_color, float a_intensity)
	{
		m_name = a_name;
		m_direction = a_pos;
		m_intensity = a_intensity;
		m_color = a_color * m_intensity;
	};

	// Getters
	std::string GetLightName() { return m_name; };
	glm::vec3& GetLightPosition() { return m_direction; }
	glm::vec3& GetLightColor() { return m_color; }
	float& GetLightIntensity() { return m_intensity; }

	// Light Variables
	std::string m_name;
	glm::vec3 m_direction;
	glm::vec3 m_color;
	float m_intensity;
};

class Scene
{
public:
	Scene(Camera* a_camera, glm::vec2 a_windowsSize, Light& a_light, glm::vec3 a_ambientLight);
	~Scene();

	// Adds instance to list of instances in scene
	void AddInstance(Instance* a_instance);

	// Adds camera to list of cameras in scene
	void AddCamera(Camera* a_camera);

	// Draw Function
	void Draw();

	// Getters
	glm::vec2 GetWindowSize() { return m_windowSize; }

	Camera* GetCamera() { return m_camera; }
	std::vector<Camera*> GetCameras() { return m_cameras; }

	int GetNumLights() { return (int)m_pointLights.size(); }
	glm::vec3* GetPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* GetPointLightColors() { return &m_pointLightColors[0]; }
	std::string* GetPointLightNames() { return &m_pointLightNames[0]; }
	std::vector<Light>& GetPointLights() { return m_pointLights; }
	Light& GetLight() { return m_light; }
	glm::vec3& GetAmbientLight() { return m_ambientLight; }

	std::list<Instance*> GetAllInstances() { return m_instances; }

	// Setters
	void SetCamera(Camera* a_camera) { m_camera = a_camera; }

protected:
	Camera* m_camera;
	std::vector<Camera*> m_cameras;
	glm::vec2 m_windowSize;
	Light m_light;
	Light m_sunLight;
	std::vector<Light> m_pointLights;
	glm::vec3 m_ambientLight;
	std::list<Instance*> m_instances;

	std::string m_pointLightNames[MAX_LIGHTS];
	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColors[MAX_LIGHTS];

};

