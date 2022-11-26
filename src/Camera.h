#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#pragma once

class Camera
{
public:
  Camera();
  void set(glm::vec3 eye, glm::vec3 direction, glm::vec3 up);
  void setNearFar(const glm::vec2& nearFar);
  void setFov(float fov);
  void setSceneScale(float sceneScale);
  void setScreenSize(glm::uvec2 screenSize);
  void processInput(GLFWwindow *window);
  void apply(GLuint program);
  bool init(GLuint program);

private:
  GLint m_uniform_v;
  GLint m_uniform_p;
  GLint m_uniform_v_inv;

  glm::vec3 m_direction;
  glm::vec3 m_up;
  glm::vec3 m_position;

  glm::uvec2 m_screenSize;
  glm::vec2 m_nearFar;

  // Prevents the camera from jumping around when first clicking left click
  bool m_firstClick;

  // Adjust the speed of the camera and it's sensitivity when looking around
  float m_speed;
  float m_sceneScale;
  float m_sensitivity;
  float m_fov;
};