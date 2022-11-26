#include "Geometry.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Geometry::Geometry()
{
  m_transform = glm::mat4(1.0f);
  m_inverseModelMatrixLoc = -1;
  m_modelMatrixLoc = -1;
  m_verticesLoc = -1;
  m_normalsLoc = -1;
  m_vboNormals = -1;
  m_material = nullptr;
}

void Geometry::draw()
{
  //Bind vao
  glBindVertexArray(m_vao);

  //Enables attributes.
  if(m_verticesLoc != -1)
  {
    glEnableVertexAttribArray(m_verticesLoc);
  }

  if(m_normalsLoc != -1)
  {
    glEnableVertexAttribArray(m_normalsLoc);
  }

  //OBS!! Very important that indices type is correct. (third argument to glDrawElements).
  //Do the drawing.
  GLuint size = GLuint(m_indices.size());
  glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

  //This works, which means that vertice data is correct.
  //glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_vertices.size());

  //Release resources.
  if(m_verticesLoc != -1)
  {
    glEnableVertexAttribArray(0);
  }

  if(m_normalsLoc != -1)
  {
    glEnableVertexAttribArray(0);
  }

  shaderutils::glCheckError();

  glBindVertexArray(0);
}

void Geometry::addVertex(glm::vec4 vertice)
{
  m_vertices.push_back(vertice);
}

void Geometry::addNormal(glm::vec3 normal)
{
  m_normals.push_back(normal);
}

void Geometry::addIndex(std::uint32_t index)
{
  m_indices.push_back(GLuint(index));
}

void Geometry::setVertices(std::vector<glm::vec4>& vertices)
{
  m_vertices = vertices;
}

void Geometry::setNormals(std::vector<glm::vec3>& normals)
{
  m_normals = normals;
}

void Geometry::setIndices(std::vector<std::uint32_t>& indices)
{
  m_indices = indices;
}

void Geometry::setMaterial(std::shared_ptr<Material> material)
{
  m_material = material;
}

const std::vector<glm::vec4> Geometry::getVertices()
{
  return m_vertices;
}
const std::vector<GLuint> Geometry::getIndices()
{
  return m_indices;
}

void Geometry::upload()
{
  //Setup Vertex Array Object (VAO)
  glGenVertexArrays(1, &m_vao); 
  glBindVertexArray(m_vao);
  shaderutils::glCheckError();

  //Upload vertices to graphics card.
  const auto nrOfVertices = m_vertices.size();
  if (nrOfVertices > 0) {
    std::cout << "Uploading " << nrOfVertices << " vertices " << std::endl;
    //utils::printVector<glm::vec4>(m_vertices);

    glGenBuffers(1, &m_vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(m_vertices[0]),
    m_vertices.data(), GL_STATIC_DRAW);
  }
  shaderutils::glCheckError();

  //Bind the inserted data to buffer.
  if(m_verticesLoc != -1)
  {
    glEnableVertexAttribArray(m_verticesLoc);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
    glVertexAttribPointer(
      m_verticesLoc,  // attribute
      4,                  // number of elements per vertex, here (x,y,z,w)
      GL_FLOAT,           // the type of each element
      GL_FALSE,           // take our values as-is
      0,                  // no extra data between each position
      0                   // offset of first element
    );
    glDisableVertexAttribArray(m_verticesLoc);
  }
  shaderutils::glCheckError();

  //Upload normals to graphics card.
  const auto nrOfNormals = m_normals.size();
  if(nrOfNormals > 0)
  {
    std::cout << "Uploading " << nrOfNormals << " normals " << std::endl;
    //utils::printVector<glm::vec3>(m_normals);

    glGenBuffers(1, &m_vboNormals);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
    glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(m_normals[0]), m_normals.data(), GL_STATIC_DRAW);
  } 
  shaderutils::glCheckError();

  if(m_normalsLoc != -1)
  {
    glEnableVertexAttribArray(m_normalsLoc);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
    glVertexAttribPointer(
      m_normalsLoc,  // attribute
      3,                  // number of elements per vertex, here (x,y,z)
      GL_FLOAT,           // the type of each element
      GL_FALSE,           // take our values as-is
      0,                  // no extra data between each position
      0                   // offset of first element
    );
    glDisableVertexAttribArray(m_normalsLoc);
  }
  shaderutils::glCheckError();

  //Upload the indicies to graphics card.
  const auto nrOfIndicies = m_indices.size();
  if (nrOfIndicies > 0) {
    std::cout << "Uploading " << nrOfIndicies << " indicies " << std::endl;
    //utils::printVector<GLuint>(m_indices);

    glGenBuffers(1, &this->m_iboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_iboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(m_indices[0]),
    m_indices.data(), GL_STATIC_DRAW);
  }
  shaderutils::glCheckError();

  //Disable all resources.
  glEnableVertexAttribArray(0);  // Disable our Vertex Array Object  
  glBindVertexArray(0); // Disable our Vertex Buffer Object
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  shaderutils::glCheckError();
}

bool Geometry::initShader(GLuint program)
{
  const char* uniform_name;
	uniform_name = "m";
	m_modelMatrixLoc = glGetUniformLocation(program, uniform_name);
	if (m_modelMatrixLoc == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return false;
	}
  shaderutils::glCheckError();

	//Get the location for normal matrix.
	/*uniform_name = "m_3x3_inv_transp";
	m_inverseModelMatrixLoc = glGetUniformLocation(program, uniform_name);
	if (m_inverseModelMatrixLoc == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return false;
	}*/

  const char* attribute_name;
  attribute_name = "pos";
  m_verticesLoc = glGetAttribLocation(program, attribute_name);
  if (m_verticesLoc == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return false;
  }
  shaderutils::glCheckError();

  attribute_name = "inNormal";
  m_normalsLoc = glGetAttribLocation(program, attribute_name);
  if (m_normalsLoc == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return false;
  }
  shaderutils::glCheckError();

  //Initalize the geometrys material if it exists.
  if(m_material && !m_material->init(program))
  {
    std::cout << "Failed to initalize material in geometry." << std::endl;
    return false;
  }

  return true;
}

void Geometry::apply()
{
  glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_transform));
  
  if(m_material)
  {
    m_material->apply();
  }
}

BoundingBox Geometry::calculateBoundingBox()
{
  //Reset boundingbox for now.
  m_boundingBox = BoundingBox();

  for(auto v : m_vertices)
  {
    auto transformedVertex = v * m_transform; 
    m_boundingBox.expand(transformedVertex);
  }
  
  return m_boundingBox;
}