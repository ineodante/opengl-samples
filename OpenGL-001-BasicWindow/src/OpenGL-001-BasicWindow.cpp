//============================================================================
// Name        : OpenGL-001-BasicWindow.cpp
// Author      : Naveen Kumar
//============================================================================

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;
GLFWmonitor* monitor;
bool fullscreen = false;
void glfw_callback(int errorCode,const char* message);
void glfw_keycallback(GLFWwindow* window,int key,int scancode,int action,int mods);
int main() {
	if(!glfwInit()){
		cout << "Failed to initialize glfw library" << endl;
		return EXIT_FAILURE;
	}
	glfwSetErrorCallback(glfw_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE,false);

	monitor = glfwGetPrimaryMonitor();
	if(!monitor){
		cout << "Failed to get primary monitor info" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	GLFWwindow* window = glfwCreateWindow(800,600,"Naveen Kumar",nullptr,nullptr);
	if(!window){
		cout << "Failed to create window" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, glfw_keycallback);
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		cout << "GLEW Library initialization failed"<<endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	while(!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f,0.4f,0.4f,1.0f);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return EXIT_SUCCESS;
}

void glfw_callback(int errorCode, const char* message){
	cout <<"Error Code: "<<errorCode << ", " << message <<endl;
}

void glfw_keycallback(GLFWwindow* window,int key,int scancode,int action,int mods){
	if(action == GLFW_PRESS){
		switch(key){
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			break;
		case GLFW_KEY_F:
			fullscreen = !(fullscreen);
			if(fullscreen){
				glfwSetWindowMonitor(window, monitor, 0, 0, 1366, 768, 60);
			}else{
				glfwSetWindowMonitor(window, nullptr, 0, 0, 800, 600, 60);
			}
			break;
		default:break;
		}
	}
}
