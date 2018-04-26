/*
 * Window.cpp
 *
 *  Created on: 24-Apr-2018
 *      Author: naveen
 */

#include "Window.h"

namespace naveen {
Window::Window(){
	window = NULL;
	monitor = NULL;
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);
}
Window::~Window() {
	glfwTerminate();
}


bool Window::createWindow(int width,int height,const char* title,bool fullscreen){
	if(fullscreen){
		monitor = glfwGetPrimaryMonitor();
	}
	window = glfwCreateWindow(width,height,title,monitor,nullptr);
	if(!window){
		std::cout <<"Failed to create primary window" << std::endl;
		return false;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		std::cout <<"Failed to initialize glew library" << std::endl;
		return false;
	}
	glViewport(0, 0, width, height);
	return true;
}

bool Window::isOpen(){
	return !glfwWindowShouldClose(window);
}

void Window::pollEvents(){
	glfwPollEvents();
}
void Window::swapBuffers(){
	glfwSwapBuffers(window);
}

} /* namespace naveen */
