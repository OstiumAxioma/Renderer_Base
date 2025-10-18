#pragma once
#include <iostream>

//定义宏
#define app Application::getInstance()

class GLFWwindow;

using ResizeCallback = void(*)(int width, int height);
using KeyBoardCallback = void(*)(int key, int action, int mods);

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

	void setResizeCallback(ResizeCallback callback) { 
		mResizeCallback = callback;
	}
	void setKeyBoardCallback(KeyBoardCallback callback) {
		mKeyBoardCallback = callback;
	}

private:
	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	//全局唯一静态变量示例
	static Application* mInstance;

	uint32_t mWidth {0};
	uint32_t mHeight {0};
	GLFWwindow* mWindow{ nullptr };

	//成员变量用于主函数调用
	ResizeCallback mResizeCallback{ nullptr };
	KeyBoardCallback mKeyBoardCallback{ nullptr };

	Application();

};