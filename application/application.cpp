#include "application.h"

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