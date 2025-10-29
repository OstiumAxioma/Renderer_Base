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

void prepareVBO() {
    //创建单个VBO
    GLuint VBO = 0;
	GL_CALL(glGenBuffers(1, &VBO));
    
    //销毁单个VBO
	GL_CALL(glDeleteBuffers(1, &VBO));
    
    //创建多个VBO
	GLuint VBOs[3] = { 0 };
	GL_CALL(glGenBuffers(3, VBOs));

    //销毁多个VBO
	GL_CALL(glDeleteBuffers(3, VBOs));
}

void bindVBOtoBuffer() {
    //准备顶点数据
    float vertices[] = {
        //位置属性
         0.5f,  0.5f, 0.0f,//右上角
         0.5f, -0.5f, 0.0f,//右下角
        -0.5f, -0.5f, 0.0f,//左下角
	};

	//生成单个VBO
	GLuint VBO = 0;
	GL_CALL(glGenBuffers(1, &VBO));

	//绑定当前VBO到OpenGL状态机VBO插槽
	//GL_ARRAY_BUFFER表示当前VBO插槽
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));

	//向当前VBO插槽传递数据/开辟显存空间
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
}

//单一存储
void prepareSingleBuffer() {

    //1.1 准备顶点位置数据
    float position[] = {
        //位置属性
         0.5f,  0.5f, 0.0f,//右上角
         0.5f, -0.5f, 0.0f,//右下角
        -0.5f, -0.5f, 0.0f,//左下角
	};

    //1.2 准备顶点颜色数据
    float color[] = {
        //颜色属性
        1.0f, 0.0f, 0.0f, //右上角 红色
        0.0f, 1.0f, 0.0f, //右下角 绿色
        0.0f, 0.0f, 1.0f, //左下角 蓝色
	};

	//2.1 分别生成单个VBO
	GLuint positionVBO = 0;
	GLuint colorVBO = 0;

    GL_CALL(glGenBuffers(1, &positionVBO));
    GL_CALL(glGenBuffers(1, &colorVBO));

	//3. 分别填充数据到两个VBO
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, positionVBO));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW));

    //4.1 生成VAO
    GLuint VAO = 0;
    GL_CALL(glGenVertexArrays(1, &VAO));

    //4.2 绑定启用VAO
    GL_CALL(glBindVertexArray(VAO));

    //5.1 填充描述信息到VAO
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, positionVBO)); //之前为了填充颜色数据，换绑到了颜色VBO，现在我们要换回位置VBO
    GL_CALL(glEnableVertexAttribArray(0)); //激活VAO的0号槽位用于存放
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0)); //解读为在0号槽位，压入3个来自position的数据，都是GL_FLOAT数据格式，归一化关闭，用3个浮点数的步长位移来读取，不使用任何偏移量

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVBO)); //换绑颜色VBO
    GL_CALL(glEnableVertexAttribArray(1)); //激活VAO的1号槽位用于存放
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0)); //解读为在1号槽位，压入3个来自colors的数据，都是GL_FLOAT数据格式，归一化关闭，用3个浮点数的步长位移来读取，不使用任何偏移量

    //5.2 解绑VAO
    glBindVertexArray(0);
}

//交叉存储
void prepareInterleavedBuffer() {
    //1. 定义单个数据集
    float vertices[] = {
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //右上角 红色
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //右下角 绿色
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, //左下角 蓝色
    };

    //2. 生成单个VBO交叉储存
    GLuint verticeVBO = 0;
    GL_CALL(glGenBuffers(1, &verticeVBO));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, verticeVBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    //3.1 生成VAO
    GLuint VAO = 0;
    GL_CALL(glGenVertexArrays(1, &VAO));

    //3.2 绑定启用VAO
    GL_CALL(glBindVertexArray(VAO));

	//4. 给VAO绑定VBO压入数据
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, verticeVBO));

    //5. 填充描述信息到VAO
	GL_CALL(glEnableVertexAttribArray(0)); //位置属性
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0)); // 位置属性

	GL_CALL(glEnableVertexAttribArray(1)); //颜色属性
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)))); // 颜色属性，不需要换绑VBO，只需要改变偏移量，因为交叉存储在同一个VBO中

    //6. 解绑VAO
	GL_CALL(glBindVertexArray(0));

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

	prepareInterleavedBuffer();

    while (app->update()) {
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    }

    app->destroy();
    return 0;
}