//============================================================================
// Name        : OpenGL-002-BasicTriangleAndShaders.cpp
// Author      : Naveen Kumar
// Version     :
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;
GLFWmonitor* monitor;
bool fullscreen = false;
void glfw_callback(int errorCode,const char* message);
void glfw_keycallback(GLFWwindow* window,int key,int scancode,int action,int mods);
GLuint loadShaderProgram(const char* vertexShaderFilePath,const char* fragmentShaderFilePath);
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

	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	const GLfloat triangle[] = {
			-1.0f,-1.0f,0.0f,
			1.0f,-1.0f,0.0f,
			0.0f,1.0f,0.0f
	};

	GLuint vbo;
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(triangle),triangle,GL_STATIC_DRAW);

	GLuint shaderProgram = loadShaderProgram("src/shaders/vertexShader.glsl", "src/shaders/fragmentShader.glsl");
	if(!shaderProgram){
		cout << "Failed to compile shader program"<<endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	while(!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f,0.4f,0.4f,1.0f);
		glfwPollEvents();
		glUseProgram(shaderProgram);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);
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
				glfwSetWindowMonitor(window, monitor, 0, 0, 1024, 768, 60);
			}else{
				glfwSetWindowMonitor(window, nullptr, 0, 0, 800, 600, 60);
			}
			break;
		default:break;
		}
	}
}

GLuint loadShaderProgram(const char* vertexShaderFilePath,const char* fragmentShaderFilePath){
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	ifstream vertexShaderStream(vertexShaderFilePath,std::ios::in);
	stringstream vertexShaderStringStream;
	vertexShaderStringStream << vertexShaderStream.rdbuf();
	vertexShaderStream.close();
	string vertexShaderData = vertexShaderStringStream.str();
	if(vertexShaderData.length() == 0){
		cout << "Failed to open vertex shader file" << endl;
		return EXIT_FAILURE;
	}
	const char* vertexShaderCode = vertexShaderData.c_str();
	glShaderSource(vertexShader,1,&vertexShaderCode,NULL);
	glCompileShader(vertexShader);
	int result;
	glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&result);
	if(!result){
		char message[512];
		glGetShaderInfoLog(vertexShader,512,NULL,message);
		cout << message << endl;
		return 0;
	}
	delete(vertexShaderCode);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	ifstream fragmentShaderStream(fragmentShaderFilePath,std::ios::in);
	stringstream fragmentShaderStringStream;
	fragmentShaderStringStream << fragmentShaderStream.rdbuf();
	fragmentShaderStream.close();
	string fragmentShaderData = fragmentShaderStringStream.str();
	if(fragmentShaderData.length() == 0){
		cout << "Failed to open fragment shader file" << endl;
		return 0;
	}
	const char* fragmentShaderCode = fragmentShaderData.c_str();
	glShaderSource(fragmentShader,1,&fragmentShaderCode,NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&result);
	if(!result){
		char message[512];
		glGetShaderInfoLog(fragmentShader,512,NULL,message);
		cout << message << endl;
		return 0;
	}
	delete(fragmentShaderCode);
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram,GL_LINK_STATUS,&result);
	if(!result){
		char message[512];
		glGetProgramInfoLog(shaderProgram,512,NULL,message);
		cout << message << endl;
		return 0;
	}
	glDetachShader(shaderProgram,vertexShader);
	glDetachShader(shaderProgram,fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}
