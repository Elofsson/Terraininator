#pragma once

/* GLM */
// #define GLM_MESSAGES
#define GLM_FORCE_RADIANS

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <math.h>  

class Material
{
public:

  Material();
  Material(std::shared_ptr<Material> &material);

  glm::vec4 getAmbient() const;
  glm::vec4 getSpecular() const;
  glm::vec4 getDiffuse() const;
  float getShininess() const;

  void setAmbient(const glm::vec4& color);
  void setSpecular(const glm::vec4& color);
  void setDiffuse(const glm::vec4& color);
  void setShininess(float s);

  bool init(GLuint program);
  void apply();

private:
  glm::vec4 m_ambient;
  glm::vec4 m_diffuse;
  glm::vec4 m_specular;

  float m_shininess;
  float m_epsilon;

  GLint m_locAmbient;
  GLint m_locDiffuse;
  GLint m_locSpecular;
  GLint m_locShininess;

};

typedef std::vector<std::shared_ptr<Material>> MaterialVector;

