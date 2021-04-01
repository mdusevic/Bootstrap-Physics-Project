/*
 * File:	Camera.h
 *
 * Author: Mara Dusevic (s200494@students.aie.edu.au)
 * Date Created: Thursday 18 March 2021
 * Date Last Modified: Thursday 1 April 2021
 *
 * This file creates the variables used in the corresponding
 * .cpp file that creates cameras with customisable variables
 * and/or restrictions.
 *
 */

#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 a_position, float a_xRot, float a_yRot, bool a_isFlybyCam);
	~Camera() {};

	// Update Function
	void Update(float a_deltaTime);
	
	// Getters
	glm::vec3 GetPosition() { return m_position; }
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float a_width, float a_height);

	// Setters
	void SetFlybyCam(bool a_isFlyby) { m_isFlybyCam = a_isFlyby; }

private:
	float m_theta;  // In degrees x
	float m_phi;    // In degrees y
	glm::vec3 m_position;

	// Is used to determine whether to assign movement to a camera
	bool m_isFlybyCam;

	float m_lastMouseX;
	float m_lastMouseY;
};
