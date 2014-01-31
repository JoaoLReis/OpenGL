#pragma once

#include "Includes.h"
#include "Vertex.h"
#include "ShaderProgram.h"

class Piece
{

protected:
	int id;
	glm::fquat orientation;
	glm::mat4 scale;
	glm::mat4 initTransformation;
	glm::mat4 transformation;

	ShaderProgram* shaderProg;

	std::vector<Vertex> vertexes;
	std::vector<unsigned int> indices;

	GLuint VboId[2], VaoId, indexBuffer, progID;
	GLint unifID, normalID, uboID;

public:
	Piece();
	Piece(std::vector<Vertex> vs, std::vector<unsigned int> is, ShaderProgram* prog, int ident);

	virtual void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye); 
	void createBufferObject();
	void addIndex(unsigned int v);

	int getID();
	void setId(int id);

	void setColor();

	glm::mat4 createModelMatrix();
};