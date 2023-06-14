#version 420

uniform samplerCube cubeMap;
in vec3 texCoord;
in vec4 bgColor;
out vec4 finalColor;


void main() 
{
   vec4 bgColor = vec4(0.0f,0.0f,1.0f,1.0f);
   vec4 color = texture(cubeMap, texCoord) * bgColor;
   finalColor = color;
}
//Add noise function to the color
//CHANGE LIGHTING ASPECTS OF THE SKYBOX TO IMPLEMENT NEW SHADER WORK