
#pragma once

#include "Loader.h"

class ObjLoader : public Loader
{
public:
  ObjLoader();
  std::shared_ptr<Geometry> loadGeometry(std::string objFile);

};