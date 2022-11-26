#include "Material.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

#include <sstream>
#include <iostream>


/// Simple class for storing material properties
Material::Material()
{
  m_epsilon = std::numeric_limits<float>::epsilon();
  m_ambient = glm::vec4(0.1, 1.0, 0.2, 1.0);
  m_diffuse = glm::vec4(0.7, 0.8, 0.8, 1.0);
  m_specular = glm::vec4(1.0, 1.0, 1.0, 1.0);
  m_shininess = 32.0f;

  m_locAmbient = -1;
  m_locDiffuse = -1;
  m_locSpecular = -1;
}

Material::Material(std::shared_ptr<Material> &material)
{
  m_epsilon = std::numeric_limits<float>::epsilon();
  m_ambient = material->getAmbient();
  m_diffuse = material->getDiffuse();
  m_specular = material->getSpecular();
  m_shininess = material->getShininess();
}

glm::vec4 Material::getAmbient() const { return m_ambient; }
glm::vec4 Material::getSpecular() const { return m_specular; }
glm::vec4 Material::getDiffuse() const { return m_diffuse; }
float Material::getShininess() const { return m_shininess; }

void Material::setAmbient(const glm::vec4& color)  { m_ambient = color; }
void Material::setSpecular(const glm::vec4& color)  { m_specular = color; }
void Material::setDiffuse(const glm::vec4& color)  { m_diffuse = color; }
void Material::setShininess(float s)  { m_shininess = s; }

bool Material::init(GLuint program)
{ 
  m_locShininess = glGetUniformLocation(program, "material.shininess");
  
  bool allMatsOk = true;
  m_locAmbient = glGetUniformLocation(program, "material.ambient");
  if(m_locAmbient < 0)
  {
    std::cout << "Failed to initalize ambient compnent in material" << std::endl;
    allMatsOk = false;
  }

  m_locDiffuse = glGetUniformLocation(program, "material.diffuse");
  if(m_locDiffuse < 0)
  {
    std::cout << "Failed to initalize diffuse component in material " << std::endl;
    allMatsOk = false; 
  }
  
  m_locSpecular = glGetUniformLocation(program, "material.specular");
  if(m_locSpecular < 0)
  {
    std::cout << "Failed to initalize specular component in material " << std::endl;
    allMatsOk = false;
  }

  return allMatsOk;
}

void Material::apply()
{
  if(m_locAmbient > 0)
  {
    glUniform4fv(m_locAmbient, 1, glm::value_ptr(m_ambient));
  }

  if(m_locSpecular > 0)
  {
    glUniform4fv(m_locSpecular, 1, glm::value_ptr(m_specular));
  }

  if(m_locDiffuse > 0)
  {
    glUniform4fv(m_locDiffuse, 1, glm::value_ptr(m_diffuse));
  }
  
  glUniform1f(m_locShininess, m_shininess);
}
