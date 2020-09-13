#version 330

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;

out vec4 color;

uniform mat4 modelViewProjectMatrix;
uniform mat3 normalMatrix;
uniform mat4 modelViewMatrix;


void main (void) 
{

  // Defining Materials
  vec4 diffuse = vec4(1.0, 1.0, 0.0, 1.0); 
  vec4 ambient = vec4(0.02, 0.02, 0.02, 1.0);
  vec4 specular = vec4(0.1, 0.1, 0.0, 1.0);
  float shininess = 100;
  vec4 spec = vec4(0.0); 
  
  vec3 v = vec3(modelViewMatrix * vPosition);
  vec3 N = normalize(normalMatrix * vNormal);
  vec3 E = normalize(-v);       // we are in Eye Coordinates, so EyePos is (0,0,0)  
  
  
  // Defining Light 
  vec4 lightPos = vec4(0.0, 0.0, 10.0, 0.0);
  vec3 L = normalize(lightPos.xyz - v); 
  vec3 R = normalize(-reflect(L,N));  
  
  vec4 Idiff = vColor * diffuse * max(dot(N,L),0.0);

  vec4 Ispec = specular * pow(max(dot(R,E),0.0),shininess);

  vec4 Iamb = ambient;

  color = Iamb + Idiff + Ispec;
  
  gl_Position = modelViewProjectMatrix * vPosition;
      
}
