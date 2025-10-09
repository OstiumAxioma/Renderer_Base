#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void frameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
    std::cout << "Updated Window Size: " << width << ", " << height << std::endl;
}

void keyBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W) {
        std::cout << "Press: " << key << std::endl;
    }
}

int main()
{
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);

    glfwMakeContextCurrent(window);


    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);


    glfwSetKeyCallback(window, keyBack);


    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}