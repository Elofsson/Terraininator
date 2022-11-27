#include <iostream>
#include <optional>
#include <windows.h>

#include "Application.h"

#define _GLFW_WIN32


void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
  // ignore non-significant error/warning codes
  if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

  std::cout << "---------------" << std::endl;
  std::cout << "Debug message (" << id << "): " <<  message << std::endl;

  switch (source)
  {
      case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
      case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
      case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
      case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
      case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
      case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
  } std::cout << std::endl;

  switch (type)
  {
      case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
      case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
      case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
      case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
      case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
      case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
      case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
      case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
      case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
  } std::cout << std::endl;
  
  switch (severity)
  {
      case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
      case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
      case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
      case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
  } std::cout << std::endl;
  std::cout << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Weak pointer to the application, so we can access it in callbacks
std::weak_ptr<Application> g_applicationPtr;

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  int shouldClose = 0;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);

  //if (key == GLFW_KEY_R && action == GLFW_PRESS)
    //if (auto app = g_applicationPtr.lock())
      //app->reloadScene();
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
  //if (auto app = g_applicationPtr.lock())
    //TODO make this resize the window.
    //app->setScreenSize(width, height);
}


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char **argv)
{
  std::cout << "Terrain generation entrypoint " << std::endl;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  auto gladSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if (!gladSuccess)
  {
    std::cout << "Failed to initialize GLAD: " << gladSuccess << std::endl;
    return -1;
  }  

  glfwSetKeyCallback(window, keyboard_callback);

  glfwSetWindowSizeCallback(window, window_size_callback);

  //Create the application.
  std::string vshaderPath = "src/shaders/phong-shading.vert.glsl";
  std::string fshaderPath = "src/shaders/phong-shading.frag.glsl";
  std::shared_ptr<Application> app = std::make_shared<Application>();
  g_applicationPtr = app;
  if(!app->init(vshaderPath, fshaderPath))
  {
    std::cout << "Failed to initalize application ): exit" << std::endl;
    return -1;
  }

  std::string scube = "res/simpleBoxV3.obj";
  if(!app->addGeometry(scube))
  {
    std::cout << "Failed to add obj " << scube << " to scene." << std::endl;
    return -1;
  }

  std::string capybaraFilePath = "res/capybara_2_high_res.jpg";
  std::string heightMapFilePath = "res/iceland_heightmap.png";
  app->loadTerrain(heightMapFilePath);

  if(!app->initView())
  {
    std::cout << "Failed to initalize application view " << std::endl;
    return -1;
  }

  //Initalize debug opengl
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); 

  int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
  {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
      glDebugMessageCallback(glDebugOutput, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
  } 

  while (!glfwWindowShouldClose(window))
  {
    // Just to test if things actually works.
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Update application.
    app->update();
    app->processInput(window);
   
    glfwSwapBuffers(window);
    glfwPollEvents();

  }

  glfwTerminate();

  return 0;
}


