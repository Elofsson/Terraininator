
#pragma once

#include <string>
#include <iostream>
#include <memory>


struct Image
{
  int width;
  int height;
  int nrChannels;
  unsigned char *data;
};

class ImageLoader
{
public:
  ImageLoader();
  std::shared_ptr<Image> readImage(std::string imageFilePath);
  void free();

private:
  std::shared_ptr<Image> m_imageData;
};