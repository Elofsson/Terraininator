#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

/**
 * @brief Simple class that store light properties and apply them to Uniforms
 * 
 */
class Light
{
public:
  Light();

  /**
   * @brief Apply this light to the shader
   * 
   * @param program Program light should be applied to
   */
  void apply(GLuint program);

  /**
   * @brief Get the Ambient object
   * 
   * @return glm::vec4 
   */
  glm::vec4 getAmbient();

  /**
   * @brief Get the Diffuse object
   * 
   * @return glm::vec4 
   */
  glm::vec4 getDiffuse();

  /**
   * @brief Get the Specular object
   * 
   * @return glm::vec4 
   */
  glm::vec4 getSpecular();

  /**
   * @brief Get the Position object
   * 
   * @return glm::vec4 
   */
  glm::vec4 getPosition();

  /**
   * @brief Checks if this light is enabled.
   * 
   * @return true 
   * @return false 
   */
  bool isEnabled();

  /**
   * @brief Set the Ambient object
   * 
   * @param ambient 
   */
  void setAmbient(glm::vec4 ambient);

  /**
   * @brief Set the Diffuse object
   * 
   * @param diffuse 
   */
  void setDiffuse(glm::vec4 diffuse);

  /**
   * @brief Set the Specular object
   * 
   * @param specular 
   */
  void setSpecular(glm::vec4 specular);

  /**
   * @brief Set the Position object
   * 
   * @param position 
   */
  void setPosition(glm::vec4 position);

  /**
   * @brief Set the Enabled object
   * 
   * @param enabled 
   */
  void setEnabled(bool enabled);

private:

  bool m_enabled;
  glm::vec4 m_position;
  glm::vec4 m_diffuse;
  glm::vec4 m_specular;
  glm::vec4 m_ambient;
};
typedef std::vector<std::shared_ptr<Light>> LightVector;


