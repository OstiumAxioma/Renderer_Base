#include "application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//初始化Application静态变量
Application* Application::mInstance = nullptr;
Application* Application::getInstance() {
	if (mInstance == nullptr) {
		mInstance = new Application();
	}

	return mInstance;
}

Application::Application() {
	
}

Application::~Application() {

}

bool Application::init(const int& width, const int& height) {
	//初始化环境
	mWidth = width;
	mHeight = height;
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//启用核心模式/非立即渲染模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗体对象指针指向Create出来的窗口
	mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL Window", NULL, NULL);

	if (mWindow == NULL) {
		return false;
	}

	//设置当前窗体对象为OpenGL的舞台
	glfwMakeContextCurrent(mWindow);

	//加载所有当前版本的Opengl函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glfwSetFramebufferSizeCallback(mWindow, frameBufferSizeCallback);

	return true;
}
bool Application::update() {
	if (glfwWindowShouldClose(mWindow)) {
		return false;
	}

	glfwPollEvents();

	//切换双缓存
	glfwSwapBuffers(mWindow);

	return true;

}

void Application::destroy() {
	glfwTerminate();
}

void Application::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	std::cout << "Resize" << std::endl;
	if (Application::getInstance()->mResizeCallback != nullptr) {
		Application::getInstance()->mResizeCallback(width, height);
	}
	
	std::cout << "Width: " << width << " Height: " << height << std::endl;
}