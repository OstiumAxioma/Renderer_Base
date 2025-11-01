#pragma once

#include "core.h"

class Shader {
public:
	Shader();
	~Shader();

	void begin(); //开始使用当前shader
	void end(); //结束使用当前shader
private:
	GLuint m_Program{ 0 };
};