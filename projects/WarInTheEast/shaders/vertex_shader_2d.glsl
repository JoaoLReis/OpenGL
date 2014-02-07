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
	ex_Texture = vec2(in_Texture.x, 1.0 - in_Texture.y);
}


