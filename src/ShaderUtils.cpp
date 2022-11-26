#include "ShaderUtils.h"
#include <fstream>
#include <iostream>

std::optional<std::string> shaderutils::readShader(std::string shaderFile)
{
  std::fstream shaderStream;
  shaderStream.open(shaderFile);

  if(!shaderStream.is_open())
  {
    std::cout << "Something went wrong with opening shader: " << shaderFile << std::endl;
    return {};
  }
  
  std::string shaderStr;
  std::string tmpdata;
  while(!shaderStream.eof())
  {
    std::getline(shaderStream, tmpdata);
    shaderStr.append(tmpdata).append("\n");
  }

  shaderStream.close();
  std::cout << "New shader: \n" << shaderStr << std::endl;
  return shaderStr;
}

GLuint shaderutils::createProgram(const char *vsource, const char *fsource)
{
   // Compile shaders
  unsigned int vertex, fragment;
  GLuint program;

  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vsource, NULL);
  glCompileShader(vertex);
  shaderutils::glCheckError();
  shaderutils::checkCompileErrors(vertex);
  
  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fsource, NULL);
  glCompileShader(fragment);
  shaderutils::glCheckError();
  shaderutils::checkCompileErrors(fragment);

  // shader Program
  program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  //shaderutils::glCheckError();
  //shaderutils::checkCompileErrors(program);
  shaderutils::glCheckError();

  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  shaderutils::glCheckError();

  return program;
}

void shaderutils::checkCompileErrors(unsigned int shader)
{
  std::cout << "Checking shader status..." << std::endl;
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {

    std::cout << " Failed to compile shader: " << shader << " ): " << std::endl;

    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    std::cout << "Max log length " << maxLength << std::endl;

    // The maxLength includes the NULL character
    std::vector<GLchar> shaderInfoLog(maxLength);
    std::vector<GLchar> programInfoLog(maxLength);

    glGetShaderInfoLog(shader, maxLength, &maxLength, &shaderInfoLog[0]);
    glGetProgramInfoLog(shader, maxLength, &maxLength, &programInfoLog[0]);

    printVector(shaderInfoLog);
    printVector(programInfoLog);
  }
}

void shaderutils::printVector(std::vector<GLchar> vec)
{
  std::cout << "Printing vector of size:  " << vec.size() << std::endl;
  for(auto character : vec)
  {
    std::cout << character;
  }
  std::cout << "End of vector print" << std::endl; 
}

GLenum shaderutils::glCheckError_(const char *file, int line)
{
  GLenum errorCode;
  while ((errorCode = glGetError()) != GL_NO_ERROR)
  {
    std::string error;
    switch (errorCode)
    {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
    }
    std::cout << error << " | " << file << " (" << line << ")" << std::endl;
  }
  return errorCode;
}