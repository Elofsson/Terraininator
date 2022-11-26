#include "ObjLoader.h"
#include "OBJ_Loader.h"

ObjLoader::ObjLoader()
{
}

std::shared_ptr<Geometry> ObjLoader::loadGeometry(std::string objFile)
{
  	// Initialize Loader
	objl::Loader loader;

	// Load .obj File
	bool loadout = loader.LoadFile(objFile);

  if(!loadout)
  {
    std::cout << "ObjLoader failed to load file... " << objFile << std::endl;
    return nullptr; 
  }

  //Loop through meshes and create new geometry.
  std::shared_ptr<Geometry> newGeometry = std::shared_ptr<Geometry>(new Geometry());
  for(auto mesh : loader.LoadedMeshes)
  {
    std::cout << "Loading mesh..." << std::endl;

    //Insert vertices.
    std::uint32_t debugIndex = 0;
    for(auto vertex : mesh.Vertices)
    {
      const auto pos = vertex.Position;
      newGeometry->addVertex(glm::vec4(pos.X, pos.Y, pos.Z, 1.0f));
      
      const auto normal = vertex.Normal;
      newGeometry->addNormal(glm::vec3(normal.X, normal.Y, normal.Z));

      debugIndex++;
    }
  
    //Insert indices.
    for(auto index : mesh.Indices)
    {
      newGeometry->addIndex(index);
    }
  }

  std::cout << "Loaded new geometry with vertices: " << newGeometry->getVertices().size() << std::endl;
  std::cout << "Indices: " << newGeometry->getIndices().size() << std::endl; 

  return newGeometry;
}
