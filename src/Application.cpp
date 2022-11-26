#include "Application.h"
#include "utils/ShaderUtils.h"
#include <iostream>
#include <fstream>

#include <glm/gtx/io.hpp>

Application::Application()
{
  m_scene = std::shared_ptr<Scene>(new Scene());
}

bool Application::init(std::string vshaderFilePath, std::string fshaderFilePath)
{
  //Initalize shaders here.
  auto vshader = shaderutils::readShader(vshaderFilePath);
  if(!vshader.has_value())
  {
    std::cout << "Vertex shader do not have a value. " << std::endl;
    return false;
  }

  auto fshader = shaderutils::readShader(fshaderFilePath);
  if(!fshader.has_value())
  {
    std::cout << "Fragment shader do not have a value. " << std::endl;
    return false;
  }
  
  //Compile, link and set program.
  GLuint program = shaderutils::createProgram(vshader.value().c_str(), fshader.value().c_str());
  shaderutils::glCheckError();
  m_scene->setProgram(program);

  if(!m_scene->init())
  {
    std::cout << "Failed to init scene ): " << std::endl;
    return false;
  }
  shaderutils::glCheckError();

  return true;
}

bool Application::initView()
{
  //TODO The size should be passed in as parameter probably, and also respond to resize callback.
  glm::uvec2 screenSize = glm::uvec2(1280, 720);
  glViewport(0, 0, screenSize.x, screenSize.y);

  // Compute a bounding box around the whole scene
  BoundingBox box = m_scene->calculateBoundingBox();
  float radius = box.getRadius();
  std::cout << "Radius: " << radius << std::endl;

  // Compute the diagonal and a suitable distance so we can see the whole thing
  float distance = radius * 1.5f;
  glm::vec3 eye = glm::vec3(0, radius, distance);

  glm::vec3 direction = glm::normalize(box.getCenter() - eye);
  std::cout << "Direction in init view " << direction << std::endl;

  // Set the position/direction of the camera
  auto camera = m_scene->getCamera();
  camera->setScreenSize(screenSize);
  camera->set(glm::vec3(0.0, 0.5, 0.0), direction, glm::vec3(0.0, 1.0, 0.0));
  camera->setNearFar(glm::vec2(0.1, distance * 20));
  camera->setSceneScale(0.01f * radius);
  camera->setFov(90);

  return true;
}

bool Application::addGeometry(std::string filePath)
{

  ObjLoader loader;
  auto newGeometry = loader.loadGeometry(filePath);

  if(!newGeometry)
  {
    std::cout << "Failed to load new object: " << filePath << std::endl;
    return false;
  }

  auto res = m_scene->addGeometry(newGeometry);
  return res;
}

void Application::processInput(GLFWwindow *window)
{
  //Pass to camera.
  m_scene->getCamera()->processInput(window);
}

void Application::update()
{
  m_scene->render();
}

void Application::loadTerrain(std::string heightMapFilePath)
{
  

  //Create a new terrain geometry object to be rendered and add to scene.
  std::shared_ptr<Terrain> terrainObj = std::shared_ptr<Terrain>(new Terrain());
  if(!terrainObj->loadTerrain(heightMapFilePath))
  {
    std::cout << "Failed to load terrain ): " << std::endl;
    return;
  }

  //Set geometry material.
  std::shared_ptr<Material> mat = std::shared_ptr<Material>(new Material());
  terrainObj->setMaterial(mat);
  
  m_scene->addGeometry(terrainObj);
}