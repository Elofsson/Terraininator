#include "Scene.h"

#include <iostream>

Scene::Scene()
{
  m_camera = std::shared_ptr<Camera>(new Camera());
  m_program = -1;
}

//TODO maybe read in shader here?
bool Scene::init()
{
  if(m_program == -1)
  {
    std::cout << "A program has not been set yet. Cannot init scene." << std::endl;
    return false;
  }

  glUseProgram(m_program);
  
  if(!m_camera->init(m_program))
  {
    std::cout << "Failed to initalize camera. " << std::endl;
    return false;
  }
  
  glUseProgram(0);


  return true;
}

std::shared_ptr<Camera> Scene::getCamera()
{
  return m_camera;
}

bool Scene::addGeometry(std::shared_ptr<Geometry> geometry)
{
  if(m_program < 0)
  {
    std::cout << "Cannot add geometry, a shader has not been initalized..." << std::endl;
    return false;
  }

  glUseProgram(m_program);

  if(!geometry->initShader(m_program))
  {
    return false;
  }

  geometry->upload();
  glUseProgram(0);

  m_geometries.push_back(geometry);
  return true;
}

BoundingBox Scene::calculateBoundingBox()
{
  BoundingBox bbox;
  for(auto geometry : m_geometries)
  {
    bbox.expand(geometry->calculateBoundingBox());
  }
  return bbox;
}

//TODO Add another way to set shader programs.
//Should be possible to add, and use multiple shaders.
void Scene::setProgram(GLuint program)
{
  m_program = program;
}

void Scene::render()
{

  if(m_program < 0)
  {
    std::cout << "Cannot render. A shader has not been initalized ):" << std::endl;
    return;
  }

  glUseProgram(m_program);

  //Apply camera.
  m_camera->apply(m_program);

  //Render all the geometry here.
  for(auto geometry : m_geometries)
  {
    geometry->apply();
    geometry->draw();
  }

  shaderutils::glCheckError();

  glUseProgram(0);
}

