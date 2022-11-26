
#pragma once


#include "Geometry.h"
#include "loaders/ImageLoader.h"


class Terrain : public Geometry 
{

public:
  Terrain();
  void draw() override;
  bool loadTerrain(std::string heightMapPath);
private:

  std::uint32_t m_numStrips;
  std::uint32_t m_numVerticesPerStrip;
};


