

#pragma once

#include <vector>
#include <iostream>

#include <glm/gtx/io.hpp>   //This is required for making the print work for glm types.

namespace utils{

  template<typename T>
  void printVector(std::vector<T> vec)
  {
    for(int i = 0; i < vec.size(); i++)
    {
      std::cout << "[" << i << "] : " << vec[i] << std::endl;
    }
  }
}
