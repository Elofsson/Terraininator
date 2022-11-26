
#pragma once

#include <glad/glad.h>
#include <string>
#include <optional>
#include <vector>

namespace shaderutils
{ 
  std::optional<std::string> readShader(std::string shaderPath); 
  GLuint createProgram(const char *vsource, const char *fsource);
  void checkCompileErrors(unsigned int shader);

  GLenum glCheckError_(const char *file, int line);
  #define glCheckError() glCheckError_(__FILE__, __LINE__);

  //TODO Move this to a separate utils class with template.
  void printVector(std::vector<GLchar> vec);
};
