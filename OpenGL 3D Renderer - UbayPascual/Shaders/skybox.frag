#version 330 core

out vec4 FragColor;

in vec3 texCoord;

uniform samplerCube skybox;

void main()
{
	//Call the texture function to get the color
	FragColor = texture(skybox, texCoord);
}