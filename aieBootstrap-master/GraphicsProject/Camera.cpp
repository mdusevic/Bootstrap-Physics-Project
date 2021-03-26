#include "Camera.h"
#include <glm/ext.hpp>
#include <Input.h>

Camera::Camera()
{
	m_position = glm::vec3(-10, 2, 0);
	m_phi = 0;
	m_theta = 0;
	m_isFlybyCam = false;
}

void Camera::Update(float a_deltaTime)
{
	if (m_isFlybyCam)
	{
		aie::Input* input = aie::Input::getInstance();
		float thetaR = glm::radians(m_theta);
		float phiR = glm::radians(m_phi);

		// Calculate the forwards and right axes and the up axis for the camera
		glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));
		glm::vec3 right(-glm::sin(thetaR), 0, glm::cos(thetaR));
		glm::vec3 up(0, 1, 0);

		#pragma region InputSettings

		float speed = 1.0f;

		if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
		{
			speed *= 10.0f;
		}

		if (input->isKeyDown(aie::INPUT_KEY_X))
		{
			m_position += up * a_deltaTime * speed;
		}

		if (input->isKeyDown(aie::INPUT_KEY_Z))
		{
			m_position -= up * a_deltaTime * speed;
		}

		if (input->isKeyDown(aie::INPUT_KEY_A))
		{
			m_position -= right * a_deltaTime * speed;
		}

		if (input->isKeyDown(aie::INPUT_KEY_D))
		{
			m_position += right * a_deltaTime * speed;
		}

		if (input->isKeyDown(aie::INPUT_KEY_W))
		{
			m_position += forward * a_deltaTime * speed;
		} 

		if (input->isKeyDown(aie::INPUT_KEY_S))
		{
			m_position -= forward * a_deltaTime * speed;
		}

		#pragma endregion

		// Get the current position of the mouse coordinates 
		float mX = input->getMouseX();
		float mY = input->getMouseY();
		const float turnSpeed = glm::radians(180.0f);

		// If the right button is down, increment the theta and phi
		if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
		{
			m_theta += turnSpeed * (mX - m_lastMouseX) * a_deltaTime;
			m_phi += turnSpeed * (mY - m_lastMouseY) * a_deltaTime;
		}

		// Now store the frames last values for the next
		m_lastMouseX = mX;
		m_lastMouseY = mY;
	}
	
}

glm::mat4 Camera::GetViewMatrix()
{
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);
	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));

	return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix(float a_width, float a_height)
{
	return glm::perspective(glm::pi<float>() * 0.25f, a_width / a_height, 0.1f, 1000.0f);
}
