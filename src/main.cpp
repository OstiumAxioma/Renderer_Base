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
	//注意偏移量是在一个顶点内进行计算而不是整个数组，每个顶点数据都会偏移一次这个量。
    
    //6. 解绑VAO
	GL_CALL(glBindVertexArray(0));

}

void prepareShader () {
    //1. 完成shader源码并装入字符串
	const char* vertexShaderSource =
        "#version 460 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

    const char* fragmentShaderSource =
        "#version 460 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

	//2. 创建顶点着色器对象并编译
	GLuint vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    //3. 输入shader代码
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    int success = 0;
    char infoLog[512];
    //4. 执行代码编译
	glCompileShader(vertexShader);
	//检查编译结果
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	glCompileShader(fragmentShader);
	//检查编译结果
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

	//5. 创建着色器程序
    GLuint shaderProgram = 0;
    shaderProgram = glCreateProgram();

	//6. 将编译好的着色器附加到程序上
    glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//7. 链接着色器程序
	glLinkProgram(shaderProgram);
	//检查链接错误
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    };

	//清理不再需要的着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
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

	prepareShader();
	prepareInterleavedBuffer();

    while (app->update()) {
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    }

    app->destroy();
    return 0;
}