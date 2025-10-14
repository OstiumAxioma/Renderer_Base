#include "checkError.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h>

void checkError() {
    //²¶»ñ´íÎó´úÂë
    GLenum errorCode = glGetError();

    //·­Òë´íÎó´úÂë
    std::string error = "";
    if (errorCode != GL_NO_ERROR) {
        switch (errorCode)
        {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            std::cout << error << std::endl;
            assert(false);
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            std::cout << error << std::endl;
            assert(false);
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            std::cout << error << std::endl;
            assert(false);
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            std::cout << error << std::endl;
            assert(false);
            break;
        default:
            error = "UNKOWN_ERROR";
            std::cout << error << std::endl;
            assert(false);
            break;
        }
    }

};