#version 330 core

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

uniform vec3 cameraEye;

in vec4 in_Position;
in vec4 in_Color;
in vec4 in_Normal;
in vec2 in_Texture;

layout(std140) uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

out vec4 ex_Color;

void main () {
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;
	//gl_Position = in_Position;
	ex_Color = in_Color;
}