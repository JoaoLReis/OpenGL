#pragma once

#include "Includes.h"
#include <fstream>
#include <vector>

using namespace std;

class ShaderProgram {

	private:

		GLuint ProgramId;
		GLuint VertexShaderId;
		GLuint FragmentShaderId;
		GLint UBO_BP;

	public:

		ShaderProgram();

		void addVertexShader(const char* loc);
		void addFragmentShader(const char* loc);

		void bindAttr(char* decr, int loc);
		void link();

		GLint getUniform(char* descr);
		GLint *getUniformBlockIndex();
		GLuint getProgram();
		GLuint getVertexShaderId();
		GLuint getFragmentShaderId();

		~ShaderProgram();

};