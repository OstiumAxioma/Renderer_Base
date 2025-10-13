#include "checkError.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h>

void checkError() {
    //捕获错误代码
    GLenum errorCode = glGetError();

    //翻译错误代码
    std::string error = "";
    if (errorCode != GL_NO_ERROR) {
        switch (errorCode)
        {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        default:
            error = "UNKOWN_ERROR";
            break;
        }
    }

    std::cout << error << std::endl;

    //根据传入的布尔值决定程序是否停止
    assert(false);
};