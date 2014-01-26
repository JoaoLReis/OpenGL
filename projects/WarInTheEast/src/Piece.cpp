#include "Piece.h"

Piece::Piece(std::vector<Vertex> vs, std::vector<Face*> *fs, ShaderProgram* prog)
{
	Vertexes = vs;
	faces = fs;
	shaderProg = prog;
	scale = glm::mat4(1.0f);
	initTransformation = glm::mat4(1.0f);
	transformation = glm::mat4(1.0f);
	createBufferObject();
}

void Piece::createBufferObject()
{
	progID = shaderProg->getProgram();

	unifID = glGetUniformLocation(progID,"ModelMatrix");
	normalID = glGetUniformLocation(progID,"NormalMatrix");
	uboID = glGetUniformBlockIndex(progID, "SharedMatrices");

	glUniformBlockBinding(progID, uboID, 0);

	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);

	glGenBuffers(2, VboId);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*Vertexes.size(), &Vertexes[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(VERTICES);
	glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(COLORS);
	glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertexes[0].XYZW));

	glEnableVertexAttribArray(NORMALS);
	glVertexAttribPointer(NORMALS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertexes[0].XYZW) + sizeof(Vertexes[0].RGBA)));
	
	glEnableVertexAttribArray(TEXTURE);
	glVertexAttribPointer(TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertexes[0].XYZW) + sizeof(Vertexes[0].RGBA) +  sizeof(Vertexes[0].NORMAL)));

	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2, 0, GL_STREAM_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, VboId[1]);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDisableVertexAttribArray(NORMALS);
}

void Piece::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye)
{
	glm::mat4 modelMatrix = createModelMatrix();

	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &viewMatrix);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projectionMatrix);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindVertexArray(VaoId);
	glUseProgram(progID);

	glUniformMatrix4fv(unifID, 1, GL_TRUE, glm::value_ptr(modelMatrix));	
	glUniformMatrix3fv(normalID, 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse( viewMatrix * glm::transpose(modelMatrix))))));

	/*for (std::vector<Face*>::iterator it = faces->begin() ; it != faces->end(); ++it)
	{
		(*it)->draw();
	}*/
	glDrawArrays(GL_TRIANGLES,0,Vertexes.size());

	glUseProgram(0);
	glBindVertexArray(0);
}

void Piece::addFace(Face* f)
{
	faces->push_back(f);
}

glm::mat4 Piece::createModelMatrix(){
	return  initTransformation  * glm::mat4_cast(orientation)* scale * transformation;
}