#version 330 core

uniform sampler2D Texture0;

uniform bool textured;

in vec4 ex_Colors;
in vec2 ex_Texture;
out vec4 outColor;

void main() {
	if(textured)
	{
		outColor = texture(Texture0, ex_Texture);
	}
	else outColor = ex_Colors;
}