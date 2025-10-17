#pragma once
#include <iostream>

//定义宏
#define app Application::getInstance()

class GLFWwindow;

//封装Application
class Application {
public:
	~Application();

	//用于访问示例的静态函数
	static Application* getInstance();

	bool init(const int& width, const int& height);
	bool update();
	void destroy();

	//静态变量访问参数
	uint32_t getWidth() const {
		return mWidth;
	}

	uint32_t getHeight() const {
		return mHeight;
	}

private:
	//全局唯一静态变量示例
	static Application* mInstance;

	uint32_t mWidth {0};
	uint32_t mHeight {0};
	GLFWwindow* mWindow{ nullptr };

	Application();

};