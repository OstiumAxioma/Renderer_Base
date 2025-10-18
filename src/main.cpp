#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h> //断言
#include "../wrapper/checkError.h"
#include "../application/application.h"

void OnResize(int width, int height) {
    GL_CALL(glViewport(0, 0, width, height));
}

void OnKey(int key, int action, int mods) {
	std::cout << "Key: " << key << " Action: " << action << " Mods: " << mods << std::endl;
}

int main()
{
    if (!app->init(800, 600)) {
        return -1;
    }

	app->setResizeCallback(OnResize);
	app->setKeyBoardCallback(OnKey);


    //glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
    //glfwSetKeyCallback(window, keyBack);

    //设置opengl视口和清理颜色
    glViewport(0, 0, 800, 600); //视口起点，大小
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //画布清理颜色

    while (app->update()) {
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    }

    app->destroy();
    return 0;
}