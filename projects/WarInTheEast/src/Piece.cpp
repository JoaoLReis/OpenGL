#include "Piece.h"

Piece::Piece() : Drawable() {}

Piece::Piece(std::vector<Vertex> vs, std::vector<unsigned int> is, ShaderProgram* prog, int ident) : Drawable()
{
	id = ident;
	vertexes = vs;
	indices = is;
	shaderProg = prog;
	scale = glm::mat4(1.0f);
	initTransformation = glm::mat4(1.0f);
	transformation = glm::mat4(1.0f);
	createBufferObject();
	tex = 0;
	variation = 0.0;
}

Piece::Piece(std::vector<Vertex> vs, std::vector<unsigned int> is, ShaderProgram* prog, Texture* t, int ident) : Drawable()
{
	id = ident;
	vertexes = vs;
	indices = is;
	shaderProg = prog;
	scale = glm::mat4(1.0f);
	initTransformation = glm::mat4(1.0f);
	transformation = glm::mat4(1.0f);
	tex = t;
	createBufferObject();
	variation = 0.0;
}

Drawable* Piece::clone()
{
	return 0;
}

void Piece::createBufferObject()
{
	progID = shaderProg->getProgram();

	unifID = glGetUniformLocation(progID,"ModelMatrix");
	normalID = glGetUniformLocation(progID,"NormalMatrix");
	uboID = glGetUniformBlockIndex(progID, "SharedMatrices");

	glUniform3f(glGetUniformLocation(progID, "LightPosition"), 0.0, 0.0, -5.0);//2.0,0.0,0.0
	glUniform2f(glGetUniformLocation(progID, "LightAttenuation"), 0.0f, 0.0005f);//0.0, 0.0
	glUniform3f(glGetUniformLocation(progID, "AmbientLightColor"), 0.3f, 0.3f, 0.3f);//0.1,0.1,0.1
	glUniform3f(glGetUniformLocation(progID, "LightDiffuseColor"), 0.9f, 0.9f, 0.9f);//0.9,0.9,0.9
	glUniform3f(glGetUniformLocation(progID, "LightSpecularColor"), 0.9f, 0.9f, 0.9f);//0.9,0.9,0.9

	glUniform3f(glGetUniformLocation(progID, "MaterialAmbientColor"), 0.8f, 0.8f, 0.8f);//0.9,0.1,0.1
	glUniform3f(glGetUniformLocation(progID, "MaterialDiffuseColor"), 0.9f, 0.9f, 0.9f);//0.9,0.1,0.1
	glUniform3f(glGetUniformLocation(progID, "MaterialSpecularColor"), 0.9f, 0.9f, 0.9f);//0.9,0.9,0.9
	glUniform1f(glGetUniformLocation(progID, "MaterialShininess"), 64.0f);//64.0f//22

	glUniformBlockBinding(progID, uboID, 0);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);

	glGenBuffers(2, VboId);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertexes.size(), &vertexes[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(VERTICES);
	glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(COLORS);
	glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(vertexes[0].XYZW));

	glEnableVertexAttribArray(NORMALS);
	glVertexAttribPointer(NORMALS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(vertexes[0].XYZW) + sizeof(vertexes[0].RGBA)));
	
	glEnableVertexAttribArray(TEXTURE);
	glVertexAttribPointer(TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(vertexes[0].XYZW) + sizeof(vertexes[0].RGBA) +  sizeof(vertexes[0].NORMAL)));

	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2, 0, GL_STREAM_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, VboId[1]);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Piece::prepareDraw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glBindVertexArray(VaoId);
	glUseProgram(progID);

	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &viewMatrix);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projectionMatrix);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if (tex != 0)
	{
		tex->actBindTexture();
		glUniform1f(glGetUniformLocation(progID, "textured"), true);
		glUniform1i(glGetUniformLocation(progID, "Texture0"), 0);
	}
	else
	{
		glUniform1f(glGetUniformLocation(progID, "textured"), false);
	}
}

void Piece::sendModelMatrix(glm::mat4 viewMatrix, glm::mat4 modelMatrix)
{
	glUniformMatrix4fv(unifID, 1, GL_TRUE, glm::value_ptr(modelMatrix));
	//15% performance hazard due to inverse plus 2 transposes
	glUniformMatrix3fv(normalID, 1, GL_FALSE, glm::value_ptr(glm::mat3(glm::transpose(glm::inverse(viewMatrix * glm::transpose(modelMatrix))))));
}

void Piece::bindDraw()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElementsBaseVertex(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0, 0);
}

void Piece::postDraw()
{
	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

/** /
void Piece::handleMovement()
{
	//Current position of the object.
	glm::vec4 currentPos = glm::vec4(0.0, 0.0, 0.0, 1.0);
	currentPos = currentPos * transformation;

	if (moving)
	{
		glm::mat4 x = glm::transpose(glm::translate(glm::mat4(1.0f),
			glm::vec3(
			Lerp(0.0, objective.x - currentPos.x, variation),
			Lerp(0.0, objective.y - currentPos.y, variation),
			Lerp(0.0, objective.z - currentPos.z, variation))));
		transformation = transformation * x;
		if (variation < 1.0)
		{
			variation += 0.00001;
		}
		else
		{
			variation = 0.0;
			moving = false;
		}
	}
}
/**/

void Piece::draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraEye)
{
	prepareDraw(viewMatrix, projectionMatrix);

	sendModelMatrix(viewMatrix, createModelMatrix());

	bindDraw();

	postDraw();
}

void Piece::addIndex(unsigned int v)
{
	indices.push_back(v);
}

int Piece::getID()
{
	return id;
}

void Piece::setId(int v)
{
	id = v;
}

glm::mat4 Piece::createModelMatrix(){
	return  initTransformation  * glm::mat4_cast(orientation)* scale * transformation;
}

void Piece::setColor()
{
	for (std::vector<Vertex>::iterator it = vertexes.begin(); it != vertexes.end(); ++it)
	{
		it->RGBA[0] = 0.5;
		it->RGBA[1] = 0.5;
		it->RGBA[2] = 0.5;
	}
}

void Piece::setTransformation(glm::mat4 trans)
{
	transformation = trans;
}

void Piece::setOrientation(glm::fquat ori)
{
	orientation = ori;
}

glm::fquat Piece::getOrientation()
{
	return orientation;
}

glm::mat4 Piece::getTransformation()
{
	return transformation;
}

void Piece::reset()
{
	transformation = glm::mat4(1.0f);
}

void Piece::translate(glm::vec3 vec)
{
	transformation = glm::transpose(glm::translate(transformation, vec));
}