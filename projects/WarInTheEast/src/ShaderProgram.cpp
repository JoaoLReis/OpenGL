#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() {
	ProgramId = glCreateProgram();
	UBO_BP = 0;
}

std::string readFromFile(const char* filename) {

	std::ifstream ifs(filename);
	std::string content( (std::istreambuf_iterator<char>(ifs) ),
						(std::istreambuf_iterator<char>()    ) );
	ifs.clear();
	ifs.close();
	return content;
}

void checkShaderCompilation(GLuint shaderId)
{
	GLint isCompiled = 0;

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		//
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

		//
		glDeleteShader(shaderId);

		if (infoLog.size() >= 0){
			std::cerr << "SHADER COMPILE ERROR->" << std::endl;
		}
		//print to cerr the infolog
		for (unsigned int i = 0; i <= (infoLog.size() - 1); i++) {
			std::cerr << infoLog[i];
		}
		exit(1);
	}
}

void ShaderProgram::addVertexShader(const char* filename) {
	std::string var = readFromFile(filename);

	const GLchar* VertexShader = var.c_str();
	//std::cout << VertexShader << std::endl;
	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
	glCompileShader(VertexShaderId);

	checkShaderCompilation(VertexShaderId);

	glAttachShader(ProgramId, VertexShaderId);

}

void ShaderProgram::addFragmentShader(const char* filename) {
	GLint isCompiled = 0;
	std::string var = readFromFile(filename);

	const GLchar* FragmentShader = var.c_str();
	//std::cout << FragmentShader << std::endl;
	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
	glCompileShader(FragmentShaderId);

	checkShaderCompilation(FragmentShaderId);

	glAttachShader(ProgramId, FragmentShaderId);

}

void ShaderProgram::bindAttr(char* decr, int loc) {
	glBindAttribLocation(ProgramId, loc, decr);
}

void ShaderProgram::link() {
	glLinkProgram(ProgramId);

	GLint isLinked = 0;
	glGetProgramiv(ProgramId, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE)
	{
			GLint maxLength = 0;
			glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &maxLength);
 
			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(ProgramId, maxLength, &maxLength, &infoLog[0]);
 
			//We don't need the program anymore.
			glDeleteProgram(ProgramId);
			//Don't leak shaders either.
			glDeleteShader(VertexShaderId);
			glDeleteShader(FragmentShaderId);
 
			if(infoLog.size()>= 0){
				std::cerr<<"SHADER LINKER ERROR->"<<std::endl;
			}
			//print to cerr the infolog
			for(unsigned int i = 0; i<= (infoLog.size()-1); i++) {
				std::cerr<< infoLog[i];
			}
			exit(1);
	}
}

GLint ShaderProgram::getUniform(char* descr) {
	return glGetUniformLocation(ProgramId, descr);
}

GLint *ShaderProgram::getUniformBlockIndex() {
	GLint UboId = glGetUniformBlockIndex(ProgramId, "SharedMatrices");
	glUniformBlockBinding(ProgramId, UboId, UBO_BP);
	return (&UBO_BP);
}

GLuint ShaderProgram::getProgram() {
	return ProgramId;
}

GLuint ShaderProgram::getVertexShaderId() {
	return VertexShaderId;
}

GLuint ShaderProgram::getFragmentShaderId() {
	return FragmentShaderId;
}

ShaderProgram::~ShaderProgram(void) {

	glUseProgram(0);

	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	
	glDeleteProgram(ProgramId);
}
