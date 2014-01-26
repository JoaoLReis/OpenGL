#pragma once

#include "Includes.h"
#include "Face.h"
#include "ShaderProgram.h"

class Piece
{

private:
	glm::fquat orientation;
	glm::mat4 scale;
	glm::mat4 initTransformation;
	glm::mat4 transformation;

	ShaderProgram* shaderProg;

	std::vector<Vertex> Vertexes;
	std::vector<Face*> *faces; 

	GLuint VboId[2], VaoId, progID;
	GLint unifID, normalID, uboID;

public:
	Piece(std::vector<Vertex> vs, std::vector<Face*> *faces, ShaderProgram* prog);

	void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye);
	void createBufferObject();
	void addFace(Face* f);

	glm::mat4 createModelMatrix();
};