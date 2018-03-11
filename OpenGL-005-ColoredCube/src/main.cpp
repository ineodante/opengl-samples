/*
 * main.cpp
 *
 *  Created on: 11-Mar-2018
 *      Author: naveen
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;
GLFWmonitor* monitor;
int fullscreen = false;

void glfw_error_callback(int errorCode,const char* message);
void glfw_key_callback(GLFWwindow* window,int key,int scancode,int action,int mods);
GLuint loadShaderProgram(const char* vShaderFilePath,const char* fShaderFilePath);
int main(){
	if(!glfwInit()){
		cout << "Failed to initialize glfw library" << endl;
		return EXIT_FAILURE;
	}
	glfwSetErrorCallback(glfw_error_callback);
	monitor = glfwGetPrimaryMonitor();
	if(!monitor){
		cout << "Failed to get primary monitor details" << endl;
		return EXIT_FAILURE;
	}
	GLFWwindow* window = glfwCreateWindow(800,600,"Naveen Kumar",nullptr,nullptr);
	if(!window){
		cout << "Failed to create window" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if(glewInit()!=GLEW_OK){
		cout <<"Failed to initialize glew library" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}


	GLuint shaderProgram = loadShaderProgram("src/shaders/vertexShader.glsl","src/shaders/fragmentShader.glsl");
	if(!shaderProgram){
		cout<< "Failed to create shader program " << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	GLuint mvpId = glGetUniformLocation(shaderProgram,"MVP");

	glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)800/(float)600,0.1f,100.0f);

	glm::mat4 view = glm::lookAt(glm::vec3(4,3,3), glm::vec3(0,0,0), glm::vec3(0,1,0));

	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 mvp = projection * view * model;

	const GLfloat cubeData[]={
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f,-1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f,-1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f,-1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f,-1.0f, 1.0f
	};
	const GLfloat cubeColor[] = {
			0.583f,  0.771f,  0.014f,
			0.609f,  0.115f,  0.436f,
			0.327f,  0.483f,  0.844f,
			0.822f,  0.569f,  0.201f,
			0.435f,  0.602f,  0.223f,
			0.310f,  0.747f,  0.185f,
			0.597f,  0.770f,  0.761f,
			0.559f,  0.436f,  0.730f,
			0.359f,  0.583f,  0.152f,
			0.483f,  0.596f,  0.789f,
			0.559f,  0.861f,  0.639f,
			0.195f,  0.548f,  0.859f,
			0.014f,  0.184f,  0.576f,
			0.771f,  0.328f,  0.970f,
			0.406f,  0.615f,  0.116f,
			0.676f,  0.977f,  0.133f,
			0.971f,  0.572f,  0.833f,
			0.140f,  0.616f,  0.489f,
			0.997f,  0.513f,  0.064f,
			0.945f,  0.719f,  0.592f,
			0.543f,  0.021f,  0.978f,
			0.279f,  0.317f,  0.505f,
			0.167f,  0.620f,  0.077f,
			0.347f,  0.857f,  0.137f,
			0.055f,  0.953f,  0.042f,
			0.714f,  0.505f,  0.345f,
			0.783f,  0.290f,  0.734f,
			0.722f,  0.645f,  0.174f,
			0.302f,  0.455f,  0.848f,
			0.225f,  0.587f,  0.040f,
			0.517f,  0.713f,  0.338f,
			0.053f,  0.959f,  0.120f,
			0.393f,  0.621f,  0.362f,
			0.673f,  0.211f,  0.457f,
			0.820f,  0.883f,  0.371f,
			0.982f,  0.099f,  0.879f
	};

	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cubeData),cubeData,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(void*)0);
	glEnableVertexAttribArray(0);

	GLuint vbo2;
	glGenBuffers(1,&vbo2);
	glBindBuffer(GL_ARRAY_BUFFER,vbo2);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cubeColor),cubeColor,GL_STATIC_DRAW);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(void*)0);
	glEnableVertexAttribArray(1);

	glfwSetKeyCallback(window,glfw_key_callback);
	while(!glfwWindowShouldClose(window)){

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glUseProgram(shaderProgram);
		glUniformMatrix4fv(mvpId,1,GL_FALSE,&mvp[0][0]);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES,0,3 * 12);
		glBindVertexArray(0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return EXIT_SUCCESS;
}

void glfw_error_callback(int errorCode,const char* message){
	cout <<"Error Code: " << errorCode << "-" << message << endl;
}

void glfw_key_callback(GLFWwindow* window,int key,int scancode,int action, int mods){
	if(action == GLFW_PRESS){
		switch(key){
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window,GL_TRUE);
			break;
		case GLFW_KEY_F:
			fullscreen = !fullscreen;
			if(fullscreen){
				glfwSetWindowMonitor(window,monitor,0,0,800,600,60);
			}else{
				glfwSetWindowMonitor(window,nullptr,0,0,800,600,60);
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


