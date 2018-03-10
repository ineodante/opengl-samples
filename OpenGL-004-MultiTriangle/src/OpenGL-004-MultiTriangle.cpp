//============================================================================
// Name        : OpenGL-004-MultiTriangle.cpp
// Author      : Naveen Kumar
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;


GLFWmonitor* monitor;
int fullscreen;

void glfw_error_callback(int errorCode,const char* message);
void glfw_key_callback(GLFWwindow* window, int key, int scancode,int action,int mods);
GLuint loadShaderProgram(const char* vertexShader,const char* fragmentShader);
int main() {
	if(!glfwInit()){
		cout << "Failed to initialize glfw library" << endl;
		return EXIT_FAILURE;
	}
	glfwSetErrorCallback(glfw_error_callback);
	monitor = glfwGetPrimaryMonitor();
	if(!monitor){
		cout << "Failed to get the primary monitor details" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "Naveen Kumar", nullptr, nullptr);
	if(!window){
		cout << "Failed to create the window" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	glewExperimental=GL_TRUE;
	if(glewInit() != GLEW_OK){
		cout << "Failed to initialize the GLEW Library" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwSetKeyCallback(window, glfw_key_callback);

	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	const GLfloat triangle[]={
			-0.5f,-0.5f,0.0f, 1.0f,0.0f,0.0f,
			0.5f,-0.5f,0.0f, 0.0f,1.0f,0.0f,
			0.0f,0.5f,0.0f, 0.0f,0.0f,1.0f
	};
	GLuint vbo;
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(triangle),triangle,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(void*)(3*sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);


	GLuint vao2;
	glGenVertexArrays(1,&vao2);
	glBindVertexArray(vao2);
	const GLfloat triangle2[]={
			-1.0f,-1.0f,-0.5f, 0.0f,1.0f,0.0f,
			1.0f,-1.0f,-0.5f, 0.0f,0.0f,1.0f,
			0.0f,1.0f,-0.5f, 1.0f,0.0f,0.0f
	};
	GLuint vbo2;
	glGenBuffers(2,&vbo2);
	glBindBuffer(GL_ARRAY_BUFFER,vbo2);
	glBufferData(GL_ARRAY_BUFFER,sizeof(triangle2),triangle2,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(void*)(3*sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);



	GLuint shaderProgram = loadShaderProgram("src/shaders/vertexShader.glsl", "src/shaders/fragmentShader.glsl");
	if(!shaderProgram){
		cout << "Failed to load shader program " << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	GLuint mvpId = glGetUniformLocation(shaderProgram,"MVP");

	glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)800/(float)600,0.1f,100.0f);

	glm::mat4 view = glm::lookAt(glm::vec3(0,0,4), glm::vec3(0,0,0), glm::vec3(0,1,0));

	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 mvp = projection * view * model;


	while(!glfwWindowShouldClose(window)){
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glUseProgram(shaderProgram);
		glUniformMatrix4fv(mvpId,1,GL_FALSE,&mvp[0][0]);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES,0,3);
		glBindVertexArray(0);
		glBindVertexArray(vao2);
		glDrawArrays(GL_TRIANGLES,0,3);
		glBindVertexArray(0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return EXIT_SUCCESS;
}

void glfw_error_callback(int errorCode, const char* message){
	cout <<"Error Code: "<< errorCode << " - " << message << endl;
}

void glfw_key_callback(GLFWwindow* window,int key,int scancode,int action,int mods){
	if(action == GLFW_PRESS){
		switch(key){
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;

		case GLFW_KEY_F:
			fullscreen = !fullscreen;
			if(fullscreen){
				glfwSetWindowMonitor(window, monitor, 0, 0, 1024, 768, 60);
			}else{
				glfwSetWindowMonitor(window,nullptr,0,0,1024,768,60);
			}
			break;
		default:
			break;
		}
	}
}


GLuint loadShaderProgram(const char* vertexShaderFilePath,const char* fragmentShaderFilePath){
	ifstream vShaderFile(vertexShaderFilePath,std::ios::in);
	stringstream vShaderStream ;
	vShaderStream << vShaderFile.rdbuf();
	vShaderFile.close();
	string vShaderSource = vShaderStream.str();
	const char* vShaderCode = vShaderSource.c_str();
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vShaderCode, NULL);
	glCompileShader(vShader);
	int result;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		char message[512];
		glGetShaderInfoLog(vShader, 512, NULL, message);
		cout << "Vertex Shader error message: " << message << endl;
		return 0;
	}
	delete (vShaderCode);

	ifstream fShaderFile(fragmentShaderFilePath, std::ios::in);
	stringstream fShaderStream;
	fShaderStream << fShaderFile.rdbuf();
	fShaderFile.close();
	string fShaderSource = fShaderStream.str();
	const char* fShaderCode = fShaderSource.c_str();
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fShaderCode, NULL);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		char message[512];
		glGetShaderInfoLog(fShader, 512, NULL, message);
		cout << "Fragment Shader error message: " << message << endl;
		return 0;
	}
	delete (fShaderCode);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vShader);
	glAttachShader(shaderProgram, fShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result) {
		char message[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, message);
		cout << "Shader Program error message: " << message << endl;
		return 0;
	}

	glDetachShader(shaderProgram, vShader);
	glDetachShader(shaderProgram, fShader);
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	return shaderProgram;
}
