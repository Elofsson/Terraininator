#include "Terrain.h"


Terrain::Terrain()
{
}

void Terrain::draw()
{ 
  std::cout << "Draw terrain " << std::endl; 

  // draw mesh
  glBindVertexArray(m_vao);

  //Enables attributes.
  if(m_verticesLoc != -1)
    glEnableVertexAttribArray(m_verticesLoc);

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
    glEnableVertexAttribArray(0);
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

  m_numStrips = imageData->height - 1;
  m_numVerticesPerStrip = imageData->width * 2;

  //Free the image data, it is not required anymore since vertices and indices have been
  //generated already.
  imgLoader.free();

  return true;
}

