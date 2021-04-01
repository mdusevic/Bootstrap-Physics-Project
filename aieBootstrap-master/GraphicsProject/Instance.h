/*
 * File:	Instance.h
 *
 * Author: Mara Dusevic (s200494@students.aie.edu.au)
 * Date Created: Thursday 18 March 2021
 * Date Last Modified: Thursday 1 April 2021
 *
 * This file creates the variables used in the corresponding
 * .cpp file that creates an instance of each object created.
 *
 */

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

	// Draw Function
	void Draw(Scene* a_scene);

	// Creates the matrix transform for the instance
	static glm::mat4 MakeTransform(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale);

	// Getters
	std::string GetObjectName() { return m_name; }
	glm::mat4 GetTransform() { return m_transform; }
	glm::vec3 GetRotation() { return m_rotation; }
	
	// Setters
	void SetTransform(glm::mat4 a_transform) { m_transform = a_transform; }
	void SetRotation(glm::vec3 a_rotation) { m_rotation = a_rotation; }

protected:
	std::string m_name;
	glm::mat4 m_transform;
	glm::vec3 m_rotation;
	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shader;

};