#version 330 core

uniform vec3 cameraEye;

in vec4 in_Position;
in vec4 in_Color;
in vec4 in_Normal;
in vec2 in_Texture;

out vec4 ex_Colors;

void main() {
    ex_Colors = in_Color;
    gl_Position = in_Position;
}
