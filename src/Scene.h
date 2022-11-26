
#include <memory>
#include <vector>

#include "Geometry.h"
#include "Camera.h"
#include "Light.h"

#pragma once


class Scene
{
public:
  Scene();
  void render();
  bool addGeometry(std::shared_ptr<Geometry> geometry);
  BoundingBox calculateBoundingBox();
  std::shared_ptr<Camera> getCamera();
  void setProgram(GLuint program);
  void setLight(std::shared_ptr<Light> light);
  bool init();

  //Add light.
  //Add geometry.
  //Add shader.
  //void addCamera(Camera& camera);

private:
  GLuint m_program;
  
  std::vector<std::shared_ptr<Geometry>> m_geometries;
  std::shared_ptr<Camera> m_camera;
  std::shared_ptr<Light> m_light;
};