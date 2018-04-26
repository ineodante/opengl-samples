/*
 * Window.h
 *
 *  Created on: 24-Apr-2018
 *      Author: naveen
 */

#ifndef WINDOW_H_
#define WINDOW_H_
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
namespace naveen {

class Window {
private:
	GLFWwindow* window;
	GLFWmonitor* monitor;
public:
	Window();
	virtual ~Window();
	bool createWindow(int width,int height,const char* title,bool fullscreen);
	bool isOpen();
	void pollEvents();
	void swapBuffers();
};

} /* namespace naveen */

#endif /* WINDOW_H_ */
