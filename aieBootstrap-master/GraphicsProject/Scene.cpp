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

void Scene::AddInstance(Instance* a_instance)
{
	m_instances.push_back(a_instance);
}

void Scene::Draw()
{
	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].m_direction;
		m_pointLightColors[i] = m_pointLights[i].m_color;
		aie::Gizmos::addSphere(m_pointLightPositions[i], 2.0f, 2, 2, m_pointLightColors[i]);
	}

	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
	{
		Instance* instance = *i;
		instance->Draw(this);
	}
}
