#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h> //断言
#include "../wrapper/checkError.h"
#include "../application/application.h"

GLuint VAO, shaderProgram;
int vertexCount = 0;
GLenum currentMode = GL_TRIANGLES;

void OnResize (int width, int height) {
    GL_CALL(glViewport(0, 0, width, height));
}

void printModeName(GLenum mode) {
    const char* name = "UNKNOWN";
    switch (mode) {
    case GL_POINTS: name = "GL_POINTS"; break;
    case GL_LINES: name = "GL_LINES"; break;
    case GL_LINE_STRIP: name = "GL_LINE_STRIP"; break;
    case GL_TRIANGLES: name = "GL_TRIANGLES"; break;
    case GL_TRIANGLE_STRIP: name = "GL_TRIANGLE_STRIP"; break;
    case GL_TRIANGLE_FAN: name = "GL_TRIANGLE_FAN"; break;
    default: break;
    }
    std::cout << "Current draw mode: " << name << " (" << mode << ")" << std::endl;
}

void OnKey(int key, int action, int mods) {
    if (action != GLFW_PRESS) return;

    // 使用数字键快速切换绘制模式，便于测试
    if (key == GLFW_KEY_1) {
        currentMode = GL_TRIANGLES;
        printModeName(currentMode);
    }
    else if (key == GLFW_KEY_2) {
        currentMode = GL_TRIANGLE_STRIP;
        printModeName(currentMode);
    }
    else if (key == GLFW_KEY_3) {
        currentMode = GL_TRIANGLE_FAN;
        printModeName(currentMode);
    }
    else if (key == GLFW_KEY_4) {
        currentMode = GL_LINES;
        printModeName(currentMode);
    }
    else if (key == GLFW_KEY_5) {
        currentMode = GL_LINE_STRIP;
        printModeName(currentMode);
    }
    else if (key == GLFW_KEY_6) {
        currentMode = GL_POINTS;
        printModeName(currentMode);
    }
    else {
        std::cout << "Key: " << key << " Action: " << action << " Mods: " << mods << std::endl;
    }
}

//交叉存储
void prepareInterleavedBuffer () {
    //1. 定义单个数据集
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //右上角 红色
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //右下角 绿色
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, //左下角 蓝色
    };

    //2. 生成单个VBO交叉储存
    GLuint verticeVBO = 0;
    GL_CALL(glGenBuffers(1, &verticeVBO));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, verticeVBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    //3.1 生成VAO
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

void drawMode () {
    //准备多个点
    float vertices[]{
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
         0.8f,  0.8f, 0.0f,
         0.8f,  0.0f, 0.0f
    };

    vertexCount = sizeof(vertices) / (3 * sizeof(float));

    //2. 生成单个VBO储存
    GLuint verticeVBO = 0;
    GL_CALL(glGenBuffers(1, &verticeVBO));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, verticeVBO));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    //3.1 生成VAO
    GL_CALL(glGenVertexArrays(1, &VAO));

    //3.2 绑定启用VAO
    GL_CALL(glBindVertexArray(VAO));

    //4. 给VAO绑定VBO压入数据
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, verticeVBO));

    //5. 填充描述信息到VAO
    GL_CALL(glEnableVertexAttribArray(0)); //位置属性
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0)); // 位置属性

    //6. 解绑VAO
    GL_CALL(glBindVertexArray(0));
}

void prepareShader () {
    //1. 完成shader源码并装入字符串
	const char* vertexShaderSource =
        "#version 460 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 vColor;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"   vColor = aColor;\n"
		"}\0";

    const char* fragmentShaderSource =
        "#version 460 core\n"
		"in vec3 vColor;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(vColor, 1.0f);\n"
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

void prepareColorShader() {
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
        "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
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

void render() {
    //执行画布清理
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    //1. 绑定当前program
    glUseProgram(shaderProgram);

    //2. 绑定当前VAO
    glBindVertexArray(VAO);

    //3. 发出绘制指令
	glDrawArrays(currentMode, 0, vertexCount);
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

	//prepareShader();
	//prepareInterleavedBuffer();

	prepareColorShader();
	drawMode();

    while (app->update()) {
        render();
    }

    app->destroy();
    return 0;
}