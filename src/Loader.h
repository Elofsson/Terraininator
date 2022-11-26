
#pragma once

#include <string>
#include <memory>

#include "Geometry.h"

class Loader
{
public:
  virtual std::shared_ptr<Geometry> loadGeometry(std::string file) = 0;
};