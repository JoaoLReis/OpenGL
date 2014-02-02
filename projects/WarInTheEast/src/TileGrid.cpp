#include "TileGrid.h"


TileGrid::TileGrid(std::vector<Vertex> vs, std::vector<unsigned int> is, ShaderProgram* prog, std::vector<Tile*> totaltiles, int ident) : Piece(vs, is, prog, ident)
{
	tiles = totaltiles;
}

void TileGrid::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye)
{
		glm::mat4 modelMatrix = createModelMatrix();

		glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &viewMatrix);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projectionMatrix);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindVertexArray(VaoId);
		glUseProgram(progID);

		glUniformMatrix4fv(unifID, 1, GL_TRUE, glm::value_ptr(modelMatrix));
		glUniformMatrix3fv(normalID, 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(viewMatrix * glm::transpose(modelMatrix))))));

		glUniform3f(glGetUniformLocation(progID, "LightPosition"), 0.0, 0.0, -5.0);//2.0,0.0,0.0
		glUniform2f(glGetUniformLocation(progID, "LightAttenuation"), 0.0f, 0.0005f);//0.0, 0.0
		glUniform3f(glGetUniformLocation(progID, "AmbientLightColor"), 0.3f, 0.3f, 0.3f);//0.1,0.1,0.1
		glUniform3f(glGetUniformLocation(progID, "LightDiffuseColor"), 0.9f, 0.9f, 0.9f);//0.9,0.9,0.9
		glUniform3f(glGetUniformLocation(progID, "LightSpecularColor"), 0.9f, 0.9f, 0.9f);//0.9,0.9,0.9

		glUniform3f(glGetUniformLocation(progID, "MaterialAmbientColor"), 0.8f, 0.8f, 0.8f);//0.9,0.1,0.1
		glUniform3f(glGetUniformLocation(progID, "MaterialDiffuseColor"), 0.9f, 0.9f, 0.9f);//0.9,0.1,0.1
		glUniform3f(glGetUniformLocation(progID, "MaterialSpecularColor"), 0.9f, 0.9f, 0.9f);//0.9,0.9,0.9
		glUniform1f(glGetUniformLocation(progID, "MaterialShininess"), 64.0f);//64.0f//22
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		for (int i = 0, numT = 0; i < indices.size(); i += 4, numT++)
		{

			glUniform1i(glGetUniformLocation(progID, "selected"), getTile(numT)->isSelected());
			glDrawElementsBaseVertex(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)0, i);
		}
		glUseProgram(0);
		glBindVertexArray(0);
}

std::vector<Tile*> TileGrid::getTiles()
{
	return tiles;
}

void TileGrid::addTile(Tile* tile)
{
	tiles.push_back(tile);
}

Tile* TileGrid::getTile(int index)
{
	return tiles.at(index);
}

TileGrid::~TileGrid()
{
}
