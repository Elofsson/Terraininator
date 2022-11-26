#include "ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ImageLoader::ImageLoader()
{
  m_imageData = nullptr;
}


std::shared_ptr<Image> ImageLoader::readImage(std::string imageFilePath)
{
  // load height map texture
  m_imageData = std::shared_ptr<Image>(new Image());

  //Check if the image is okay.
  int x, y, n;
  bool ok = stbi_info(imageFilePath.c_str(), &x, &y, &n);
  if(!ok)
  {
    std::cout << "Image to be read is not ok. " << imageFilePath << std::endl;
    return nullptr;
  }

  std::cout << "Image: " << imageFilePath << " Is ok to read. Continue loading... " << std::endl;

  int width, height, nChannels;
  m_imageData->data = stbi_load(imageFilePath.c_str(),
                                &width, &height, &nChannels,
                                0);

  m_imageData->width = width;
  m_imageData->height = height;
  m_imageData->nrChannels = nChannels;

  if(!m_imageData->data)
  {
    std::cout << "Failed to load image " << imageFilePath << std::endl;
    return nullptr;
  }

  return m_imageData;
}

void ImageLoader::free()
{
  if(m_imageData)
  {
    std::cout << "Free image data. " << std::endl;
    stbi_image_free(m_imageData->data);
  }
}