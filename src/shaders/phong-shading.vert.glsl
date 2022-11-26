#version 410 core

in vec4 pos;
in vec3 inNormal;

layout(location = 0) out vec4 position;  // position of the vertex (and fragment) in world space
layout(location = 1) out vec3 normal;  // surface normal vector in world space
//layout(location = 2) out vec2 texCoord; 

// model, view and projection transform
uniform mat4 m, v, p;

// Inverse transpose of model matrix for transforming normals
uniform mat3 m_3x3_inv_transp;

void main()
{
  mat4 mv = v * m;
  //mat4 mv = v;
  //texCoord = vertex.texCoord;
  
  //Save position for the fragment shader.
  position = mv * pos;
  
  //Pass normals to fragment shader.
  normal = normalize(m_3x3_inv_transp * inNormal);
  
  gl_Position = p * position;
}
