
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "BoundingBox.h"
#include "Material.h"
#include "utils/ShaderUtils.h"
#include "utils/Utils.h"

#pragma once


class Geometry
{
public:
  Geometry();
  void upload();
  virtual void draw();
  void apply();
  bool initShader(GLuint program);
  BoundingBox calculateBoundingBox();

  void addVertex(glm::vec4 vertice);
  void addIndex(std::uint32_t index);
  void addNormal(glm::vec3 normal);

  void setVertices(std::vector<glm::vec4>& vertices);
  void setIndices(std::vector<std::uint32_t>& indices);
  void setNormals(std::vector<glm::vec3>& normals);
  void setMaterial(std::shared_ptr<Material> material);

  const std::vector<glm::vec4> getVertices();
  const std::vector<GLuint> getIndices();

//TODO Check if everything should be exposed.
protected:

  glm::mat4 m_transform;

  GLuint m_vboVertices, m_vboNormals, m_iboIndices;
  GLuint m_vao;

  //Do not use unsigned ints in order to check if -1 is returned from certain opengl functions.
  GLint m_verticesLoc;
  GLint m_normalsLoc;
  GLint m_modelMatrixLoc;
  GLint m_inverseModelMatrixLoc;

  //TODO add texture coordinates.

  std::vector<glm::vec4> m_vertices;
  std::vector<glm::vec3> m_normals;
  std::vector<GLuint> m_indices;

  std::shared_ptr<Material> m_material;
  BoundingBox m_boundingBox;
};