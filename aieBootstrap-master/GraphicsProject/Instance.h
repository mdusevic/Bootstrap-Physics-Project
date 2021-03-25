#pragma once
#include <glm/glm.hpp>
#include <string>

class Camera;
class Scene;

namespace aie
{
	class OBJMesh;
	class ShaderProgram;
}

class Instance
{
public:
	Instance(std::string a_name, glm::mat4 a_transform, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader);
	Instance(std::string a_name, glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader);

	void Draw(Scene* a_scene);

	static glm::mat4 MakeTransform(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale);

	std::string GetObjectName() { return m_name; }
	glm::mat4 GetTransform() { return m_transform; }
	void SetTransform(glm::mat4 a_transform) { m_transform = a_transform; }

	glm::vec3 GetRotation() { return m_rotation; }
	void SetRotation(glm::vec3 a_rotation) { m_rotation = a_rotation; }

protected:
	std::string m_name;
	glm::mat4 m_transform;
	glm::vec3 m_rotation;
	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shader;

};

