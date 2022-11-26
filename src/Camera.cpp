#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/io.hpp>

#include <iostream>

Camera::Camera()
{
	m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_position = glm::vec3(0.0f, -1.0f, 0.0f);
  m_sensitivity = 100.0f;

  m_screenSize[0] = 1280;
  m_screenSize[1] = 720;

  m_nearFar = glm::vec2(0.1, 100);
}

void Camera::set(glm::vec3 eye, glm::vec3 direction, glm::vec3 up)
{
	m_position = eye;
	m_direction = direction;
	m_up = up;
}

void Camera::setScreenSize(glm::uvec2 screenSize)
{
  m_screenSize = screenSize;
}

void Camera::processInput(GLFWwindow *window)
{
  // Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_position += m_speed * m_sceneScale * m_direction;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_position += m_speed * m_sceneScale * -glm::normalize(glm::cross(m_direction, m_up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_position += m_speed * m_sceneScale * -m_direction;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_position += m_speed * m_sceneScale * glm::normalize(glm::cross(m_direction, m_up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_position += m_speed * m_sceneScale * m_up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		m_position += m_speed * m_sceneScale * -m_up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		m_speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		m_speed = 0.1f;
	}


	// Handles mouse inputs
	//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	//{
		// Hides mouse cursor
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  // Prevents camera from jumping on the first click
  if (m_firstClick)
  {
    glfwSetCursorPos(window, (m_screenSize[0] / 2), (m_screenSize[1] / 2));
    m_firstClick = false;
  }

  // Stores the coordinates of the cursor
  double mouseX;
  double mouseY;
  // Fetches the coordinates of the cursor
  glfwGetCursorPos(window, &mouseX, &mouseY);

  // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
  // and then "transforms" them into degrees 
  float rotX = m_sensitivity * (float)(mouseY - (m_screenSize[1] / 2)) / m_screenSize[1];
  float rotY = m_sensitivity * (float)(mouseX - (m_screenSize[0] / 2)) / m_screenSize[0];

  //std::cout << "Rotx " << rotX << std::endl;
  //std::cout << "Roty: " << rotY << std::endl;

  // Calculates upcoming vertical change in the direction
  glm::vec3 newdirection = glm::rotate(m_direction, glm::radians(-rotX), glm::normalize(glm::cross(m_direction, m_up)));

  // Decides whether or not the next vertical direction is legal or not
  if (abs(glm::angle(newdirection, m_up) - glm::radians(90.0f)) <= glm::radians(85.0f))
  {
    m_direction = newdirection;
  }

  // Rotates the direction left and right
  m_direction = glm::rotate(m_direction, glm::radians(-rotY), m_up);

  // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
  glfwSetCursorPos(window, (m_screenSize[0] / 2), (m_screenSize[1] / 2));
}

bool Camera::init(GLuint program)
{
  const char* uniform_name;
  uniform_name = "v";
  m_uniform_v = glGetUniformLocation(program, uniform_name);
  if (m_uniform_v == -1) {
    std::cerr << "Could not bind uniform " << uniform_name << std::endl;
    return false;
  }

  uniform_name = "p";
  m_uniform_p = glGetUniformLocation(program, uniform_name);
  if (m_uniform_p == -1) {
    std::cerr << "Could not bind uniform " << uniform_name << std::endl;
    return false;
  }

  uniform_name = "v_inv";
  m_uniform_v_inv = glGetUniformLocation(program, uniform_name);
  if (m_uniform_v_inv == -1) {
    std::cerr << "Could not bind uniform " << uniform_name << std::endl;
    return false;
  }

  return true;
}

void Camera::apply(GLuint program)
{
  float aspect = (float)m_screenSize[0] / (float)m_screenSize[1];
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(m_fov), aspect, m_nearFar[0], m_nearFar[1]);

	// Makes camera look in the right direction from the right position
	glm::mat4 view = glm::lookAt(m_position, m_position + m_direction, m_up);

  glUniformMatrix4fv(m_uniform_v, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(m_uniform_p, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
  glm::mat4 v_inv = glm::inverse(view);
  glUniformMatrix4fv(m_uniform_v_inv, 1, GL_FALSE, glm::value_ptr(v_inv));
}

void Camera::setNearFar(const glm::vec2& nearFar)
{
  m_nearFar = nearFar;
}

void Camera::setFov(float fov)
{
  m_fov = fov;
}

void Camera::setSceneScale(float sceneScale)
{
  m_sceneScale = sceneScale;
}