#include "Terrain.h"


Terrain::Terrain()
{
  m_material = std::shared_ptr<Material>(new Material());
  m_material->setShininess(250.0f);
}

void Terrain::draw()
{ 
  //std::cout << "Draw terrain " << std::endl; 

  // draw mesh
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

  // render the mesh triangle strip by triangle strip - each row at a time
  for(unsigned int strip = 0; strip < m_numStrips; ++strip)
  {
    glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
                  m_numVerticesPerStrip, // number of indices to render
                  GL_UNSIGNED_INT,     // index data type
                  (void*)(sizeof(unsigned int)
                            * m_numVerticesPerStrip
                            * strip)); // offset to starting index
  }

  //Release resources.
  if(m_verticesLoc != -1)
  {
    glEnableVertexAttribArray(0); 
  }

  if(m_normalsLoc != -1)
  {
    glEnableVertexAttribArray(0);
  }
}

bool Terrain::loadTerrain(std::string heightMapFilePath)
{
  std::cout << "Attempts to read heightmap: " << heightMapFilePath << std::endl;
  ImageLoader imgLoader;
  auto imageData = imgLoader.readImage(heightMapFilePath);

  if(!imageData)
  {
    std::cout << "Failed to read image " << heightMapFilePath << std::endl;
    return false;
  }

  std::cout << "Successfully read image. " << imageData->height << " : " << imageData->width << " : " << imageData->nrChannels << std::endl;
  
  // vertex generation
  float yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data
  for(unsigned int i = 0; i < imageData->height; i++)
  {
    for(unsigned int j = 0; j < imageData->width; j++)
    {
        // retrieve texel for (i,j) tex coord
        unsigned char* texel = imageData->data + (j + imageData->width * i) * imageData->nrChannels;

        // raw height at coordinate
        unsigned char y = texel[0];

        // vertex
        glm::vec4 vertex(1.0f);
        vertex.x = -imageData->height/2.0f + i;     
        vertex.y = (int)y * yScale - yShift;        
        vertex.z = -imageData->width/2.0f + j;

        addVertex(vertex);
    }
  }

  // index generation
  for(unsigned int i = 0; i < imageData->height-1; i++)         // for each row a.k.a. each strip
  {
    for(unsigned int j = 0; j < imageData->width; j++)          // for each column
    {
      for(unsigned int k = 0; k < 2; k++)                       // for each side of the strip
      {
        addIndex(j + imageData->width * (i + k));
      }
    }
  }

  //Generate face normals.
  m_normals = std::vector<glm::vec3>(m_indices.size());
  for(unsigned int i = 0; i < m_indices.size(); i += 3)
  {
    const int vertexIndex = m_indices[i];
    //Get three vertices that constructs a face.
    const glm::vec3 v1 = glm::vec3(m_vertices[vertexIndex]);
    const glm::vec3 v2 = glm::vec3(m_vertices[vertexIndex + 1]);
    const glm::vec3 v3 = glm::vec3(m_vertices[vertexIndex + 2]);

    //Compute edges.
    const glm::vec3 edge1 = v1 - v2;
    const glm::vec3 edge2 = v3 - v2;

    //Calculate normal
    const glm::vec3 normal = glm::cross(edge1, edge2);

    //Save normals
    m_normals[i] += normal;
    m_normals[i + 1] += normal;
    m_normals[i + 2] += normal;
  }

  m_numStrips = imageData->height - 1;
  m_numVerticesPerStrip = imageData->width * 2;

  //Free the image data, it is not required anymore since vertices and indices have been
  //generated already.
  imgLoader.free();

  return true;
}

