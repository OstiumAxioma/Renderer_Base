#pragma once
#include <iostream>

//封装Application

class Application {
public:
	~Application();

	//用于访问示例的静态函数
	static Application* getInstance();

	void test() {
		std::cout << "App Test" << std::endl;
	}

private:
	//全局唯一静态变量示例
	static Application* mInstance;
	Application();

};