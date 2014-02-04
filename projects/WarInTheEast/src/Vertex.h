#pragma once
#include "Includes.h"

class Vertex
{

public:
	glm::vec4 XYZW;
	glm::vec4 RGBA;
	glm::vec4 NORMAL;
	glm::vec2 UV;

	Vertex();
	Vertex(glm::vec4 xyzw, glm::vec4 rgba, glm::vec4 normal, glm::vec2 uv);
};