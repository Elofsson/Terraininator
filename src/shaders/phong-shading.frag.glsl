#version 410 core

//------------------- From the vertex shader---------------------//
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
//---------------------------------------------------------------//

// The end result of this shader
out vec4 color;

// declaration of a Material structure
/*struct Material
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};*/

// Some hard coded default ambient lighting
vec4 scene_ambient = vec4(0.2, 0.2, 0.2, 1.0);

// The front surface material
//uniform Material material;

void main()
{
  //vec3 normalDirection = normalize(normal);
  //vec3 viewDirection = normalize(vec3(v_inv * vec4(0.0, 0.0, 0.0, 1.0) - position));
  //vec3 lightDirection;
  //float attenuation;

  // Compute ambient lightning with a hardcoded ambient intensity.
  //vec3 ambientLight = vec3(scene_ambient) * vec3(material.ambient);

  //vec3 totalLighting = vec3(scene_ambient) + (normal);
  //vec3 totalLighting = normalize(normal);

  //vec4 normalizedPos = normalize(position);
  //totalLighting.y += normalizedPos.y;

  color.rgb = vec3(scene_ambient) + normalize(normal);
  color.a = 1.0f;
}