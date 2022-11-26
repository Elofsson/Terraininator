#version 410 core

//------------------- From the vertex shader---------------------//
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
//---------------------------------------------------------------//

// The end result of this shader
out vec4 color;

// declaration of a Material structure
struct Material
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

// Definition of a light source structure
struct LightSource
{
  bool enabled;
  vec4 position;
  vec4 diffuse;
  vec4 specular;
  vec4 ambient;
};

// This is the uniforms that our program communicates with
uniform LightSource light;

// The front surface material
uniform Material material;
uniform mat4 v_inv;

// Some hard coded default ambient lighting
vec4 scene_ambient = vec4(0.5, 0.5, 0.5, 1.0);

void main()
{

  vec3 viewDirection = normalize(vec3(v_inv * vec4(0.0, 0.0, 0.0, 1.0) - position));
  vec3 normalDirection = normalize(normal);
  vec3 lightDirection = normalize(vec3(light.position));

  //Add ambient lightning.
  vec3 totalLighting = vec3(scene_ambient) * vec3(material.ambient) * vec3(light.ambient);

  vec3 diffuseReflection = vec3(light.diffuse) * vec3(material.diffuse)
      * max(0.0, dot(normalDirection, lightDirection));

  vec3 specularReflection;

  if (dot(normalDirection, lightDirection) < 0.0) // light source on the wrong side?
  {
    specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
  }
  else // light source on the right side
  {
    specularReflection = vec3(light.specular) * vec3(material.specular)
      * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), material.shininess);
  }

  //Add the computed specular and diffuse lightning.
  totalLighting = totalLighting + diffuseReflection + specularReflection; 

  color.rgb = totalLighting;
  color.a = 1.0f;
}