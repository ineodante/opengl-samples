/*
 * main.cpp
 *
 *  Created on: 24-Mar-2018
 *      Author: naveen
 */

#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ModelLoader.h"

using namespace std;
using namespace glm;
using namespace naveen;

void glfw_error_callback(int errorCode ,const char* message);
void glfw_key_callback(GLFWwindow* window,int key, int scancode, int action, int mods);
GLint loadShaderProgram(const char* vertexShaderFilePath,const char* fragmentShaderFilePath);

int main(int argc,char** argv){

	if(!glfwInit()){
		cout<< "Failed to initialize the GLFW library" << endl;
		return EXIT_FAILURE;
	}
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);
	glfwSetErrorCallback(glfw_error_callback);

	GLFWwindow* window = glfwCreateWindow(800,600,"Naveen Kumar",nullptr,nullptr);
	if(!window){
		cout <<"Failed to create the window" <<endl;
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	glewExperimental=GL_TRUE;
	if(glewInit()!=GLEW_OK){
		cout << "Failed to initialize the glew library" << endl;
		return EXIT_FAILURE;
	}
	naveen::Model obj;
	ModelLoader loader;
	if(!loader.loadObjFile("src/models/cube.obj", obj)){
		cout <<"Failed to load the model file" <<endl;
		return EXIT_FAILURE;
	}
	cout <<"No of vertices: " << obj.vertices.size() << endl;
	cout <<"No of indices: " << obj.indices.size() << endl;

	GLint shaderProgram = loadShaderProgram("src/shaders/vertexShader.glsl","src/shaders/fragmentShader.glsl");
	if(shaderProgram == -1){
		cout << "Failed to load shader program " << endl;
		return EXIT_FAILURE;
	}
	GLint mvpId = glGetUniformLocation(shaderProgram,"MVP");
	if(mvpId == -1){
		cout <<"Failed to get mvp uniform" << endl;
		return EXIT_FAILURE;
	}

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800/(float)600,0.1f,100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(4,2,2),glm::vec3(0,0,0),glm::vec3(0,1,0));
	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 mvp = projection * view * model;

	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,obj.vertices.size() * sizeof(glm::vec3),&obj.vertices[0],GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1,&ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,obj.indices.size() * sizeof(unsigned int),&obj.indices[0],GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(void*)0);
	glEnableVertexAttribArray(0);

	glfwSetKeyCallback(window, glfw_key_callback);
	while(!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glUseProgram(shaderProgram);
		glUniformMatrix4fv(mvpId,1,GL_FALSE,&mvp[0][0]);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, obj.indices.size(), GL_UNSIGNED_INT,0);
		glBindVertexArray(0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	return EXIT_SUCCESS;
}

GLint loadShaderProgram(const char* vertexShaderFilePath,const char* fragmentShaderFilePath){
	ifstream vertexShaderFile(vertexShaderFilePath,std::ios::in);
	if(!vertexShaderFile.is_open()){
		cout << "Failed to open vertex shader file path" <<endl;
		return -1;
	}
	stringstream vertexShaderStream;
	vertexShaderStream << vertexShaderFile.rdbuf();
	vertexShaderFile.close();
	string vertexShaderCode = vertexShaderStream.str();
	const char* vertexShaderSource = vertexShaderCode.c_str();
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
	glCompileShader(vertexShader);
	int result;
	glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&result);
	if(!result){
		char message[512];
		glGetShaderInfoLog(vertexShader,512,NULL,message);
		cout <<"Vertex Shader Error: " << message << endl;
		return -1;
	}
	delete(vertexShaderSource);

	ifstream fragmentShaderFile(fragmentShaderFilePath,std::ios::in);
	if(!fragmentShaderFile.is_open()){
		cout <<"Failed to open the fragment shader path" <<endl;
		return -1;
	}
	stringstream fragmentShaderStream;
	fragmentShaderStream << fragmentShaderFile.rdbuf();
	fragmentShaderFile.close();
	string fragmentShaderCode = fragmentShaderStream.str();
	const char* fragmentShaderSource = fragmentShaderCode.c_str();
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&result);
	if(!result){
		char message[512];
		glGetShaderInfoLog(fragmentShader,512,NULL,message);
		cout <<"Fragment Shader Error: " << message << endl;
		return -1;
	}
	delete(fragmentShaderSource);

	GLint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram,GL_LINK_STATUS,&result);
	if(!result){
		char message[512];
		glGetProgramInfoLog(shaderProgram,512,NULL,message);
		cout<<"Link Program Error: " << message << endl;
		return -1;
	}

	glDetachShader(shaderProgram,vertexShader);
	glDetachShader(shaderProgram,fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

void glfw_error_callback(int errorCode,const char* message){
	cout <<"Error Code: " <<errorCode << ": "<< message << endl;
}

void glfw_key_callback(GLFWwindow* window,int key, int scancode, int action, int mods){
	if(action == GLFW_PRESS){
		switch(key){
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window,GL_TRUE);
			break;
		default:
			break;
		}
	}
}

