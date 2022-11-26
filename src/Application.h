
#include <string>
#include <memory>
#include "Scene.h"
#include "loaders/ObjLoader.h"
#include "loaders/ImageLoader.h"
#include "Terrain.h"

#pragma once


class Application
{

public:
    Application();
    bool init(std::string vshader, std::string fshader);
    bool initView();
    void processInput(GLFWwindow *window);
    void update();
    bool addGeometry(std::string filePath);

    //TODO find a better way to load terrains...
    void loadTerrain(std::string heightMapFile);

private:
    std::shared_ptr<Scene> m_scene;
};