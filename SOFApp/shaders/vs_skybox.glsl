#version 420

in vec3 vertexPos;

uniform vec3 cameraPos;
uniform mat4 viewProj;
	
out vec3 texCoord;

void main()
{
	texCoord = normalize(vertexPos);								//Normalize the vertex position of the cube onto texCoordinate
	gl_Position = viewProj * vec4(vertexPos + cameraPos, 1.0f);		//Add vertex pos to camera pos to set the cube on the camera, then transform it by viewProj matrix

}