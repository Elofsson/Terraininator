### Terraingenerator ###
This project aims to do some terrain generation and visualisation
and is a work in progress. Currently the only terrain visualization
that can be done is by reading in a grayscale heightmap and generating vertices
from the heightmap. However, the goal is to do some procedural terrain generation. 

## How to build ##


## TODO list ##
- [ ] Fix a good way to handle window size, respond to resize callback.
- [ ] Consider making a terraingeneration class.
- [ ] Improve the Cmake files.
- [ ] See what member variables in geometry class that should be protected / private.
- [ ] Add textures to geometries.
- [ ] Make materials work for objects loaded in from ObjLoader.
- [ ] Move shader initalization to scene.
- [ ] Add multi shader support to scene.
- [ ] Implement phong illumination.
- [ ] Generate normals for terrain.
