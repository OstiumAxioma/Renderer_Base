#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h> //断言
#include "../wrapper/checkError.h"
#include "../application/application.h"

void frameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
    std::cout << "Updated Window Size: " << width << ", " << height << std::endl;
    glViewport(0, 0, width, height); //更新视口大小
}

void keyBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W) {
        std::cout << "Press: " << key << std::endl;
    }
}

int main()
{
    Application::getInstance()->test();

    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    //启用核心模式/非立即渲染模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //创建窗体对象指针指向Create出来的窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);


    //设置当前窗体对象为OpenGL的舞台
    glfwMakeContextCurrent(window);


    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);


    glfwSetKeyCallback(window, keyBack);

    //加载所有当前版本的Opengl函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //设置opengl视口和清理颜色
    glViewport(0, 0, 800, 600); //视口起点，大小
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //画布清理颜色

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();

        //执行画布清理操作
        GL_CALL(glClear(-1)); //用宏替换错误检查函数调用

        //切换双缓存
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}