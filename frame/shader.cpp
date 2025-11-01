#include "shader.h"
#include "../wrapper/checkError.h"

Shader::Shader() {

}

Shader::~Shader() {

}

void Shader::begin() {
	GL_CALL(glUseProgram(m_Program));
}

void Shader::end() {
	GL_CALL(glUseProgram(0));
}