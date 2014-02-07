#version 330 core

in vec4 in_Position;
in vec4 in_Color;
in vec4 in_Normal;
in vec2 in_Texture;

out vec4 ex_Colors;
out vec2 ex_Texture;

void main() {
    ex_Colors = in_Color;
    gl_Position = in_Position;
	ex_Texture = in_Texture;
}


