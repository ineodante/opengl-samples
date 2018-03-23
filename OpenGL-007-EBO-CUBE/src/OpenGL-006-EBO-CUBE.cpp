/*
 * OpenGL-006-EBO-CUBE.cpp
 *
 *  Created on: 23-Mar-2018
 *      Author: naveen
 */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <imageloader/stb_image.h>

using namespace std;
using namespace glm;

void glfw_error_callback(int errorCode,const char* message);
void glfw_key_callback(GLFWwindow* window,int key,int scancode,int action,int mods);
GLint loadShaderProgram(const char* vertexShaderFilePath,const char* fragmentShaderFilePath);
GLuint loadCube();
GLuint loadTexture(const char* texturePath);
int main(int argc, char** argv){
	if(!glfwInit()){
		cout << "Failed to initialize GLFW Library" << endl;
		return EXIT_FAILURE;
	}
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);

	GLFWwindow* window = glfwCreateWindow(800,600,"Naveen Kumar",nullptr,nullptr);
	if(!window){
		cout << "Failed to create window" << endl;
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if(glewInit()!= GLEW_OK){
		cout <<"Failed to initialize the glew library" << endl;
		return EXIT_FAILURE;
	}

	GLint shaderProgram = loadShaderProgram("src/shaders/vertexShader.glsl","src/shaders/fragmentShader.glsl");
	if(shaderProgram == -1){
		cout << "Failed to load shader program" << endl;
		return EXIT_FAILURE;
	}

	GLuint texture1 = loadTexture("src/textures/wall.jpg");
	GLuint texture2 = loadTexture("src/textures/wrapper.jpg");
	cout << texture2 << endl;
	GLuint mvpId = glGetUniformLocation(shaderProgram,"MVP");
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800/(float)600,0.1f,100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(4,2,2), glm::vec3(0,0,0), glm::vec3(0,1,0));
	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 mvp = projection * view * model;

	GLuint cube = loadCube();
	glfwSetKeyCallback(window, glfw_key_callback);
	while(!glfwWindowShouldClose(window)){
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(0.5f,0.0f,0.0f));
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(2.0f,1.0,3.0f));
		transform = glm::scale(transform, glm::vec3(-0.5f,-0.5f,-0.5f));
		glUseProgram(shaderProgram);
		GLuint transformId= glGetUniformLocation(shaderProgram,"transform");
		glUniformMatrix4fv(transformId,1,GL_FALSE,glm::value_ptr(transform));
		glUniformMatrix4fv(mvpId,1,GL_FALSE,glm::value_ptr(mvp));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texture1);
		glUniform1i(glGetUniformLocation(shaderProgram,"texture1"),0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,texture2);
		glUniform1i(glGetUniformLocation(shaderProgram,"texture2"),1);
		glBindVertexArray(cube);
		glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void glfw_error_callback(int errorCode,const char* message){
	cout << "Error Code: " << errorCode << " : " << message << endl;
}

void glfw_key_callback(GLFWwindow* window,int key,int scancode, int action, int mods){
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

GLint loadShaderProgram(const char* vertexShaderFilePath,const char* fragmentShaderFilePath){
	ifstream vertexFile(vertexShaderFilePath,std::ios::in);
	if(!vertexFile.is_open()){
		cout << "Failed to open the vertex shader file path" << endl;
		return -1;
	}
	stringstream vertexShaderStream;
	vertexShaderStream << vertexFile.rdbuf();
	vertexFile.close();
	string vertexShaderCode = vertexShaderStream.str();
	const char* vertexShaderSource = vertexShaderCode.c_str();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
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

	ifstream fragmentFile(fragmentShaderFilePath,std::ios::in);
	if(!fragmentFile.is_open()){
		cout <<"Failed to open the fragment shader file path" << endl;
		return -1;
	}
	stringstream fragmentShaderStream;
	fragmentShaderStream << fragmentFile.rdbuf();
	fragmentFile.close();
	string fragmentShaderCode = fragmentShaderStream.str();
	const char* fragmentShaderSource = fragmentShaderCode.c_str();
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&result);
	if(!result){
		char message[512];
		glGetShaderInfoLog(fragmentShader,512,NULL,message);
		cout <<"Fragment Shader Error: " << message  << endl;
		return -1;
	}

	GLint shaderProgram =glCreateProgram();
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram,GL_LINK_STATUS,&result);
	if(!result){
		char message[512];
		glGetShaderInfoLog(shaderProgram,512,NULL,message);
		cout << "Shader Program Error: " << message << endl;
		return -1;
	}
	glDetachShader(shaderProgram,vertexShader);
	glDetachShader(shaderProgram,fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

GLuint loadCube(){

	float cube[] = {
		-1.0f,-1.0f,-1.0f,  //0
		-1.0f,1.0f,-1.0f,	//1
		1.0f,1.0f,-1.0f, 	//2
		1.0f,-1.0f,-1.0f,	//3

		-1.0f,-1.0f,1.0f,	//4
		-1.0f,1.0f,1.0f,	//5
		1.0f,1.0f,1.0f,		//6
		1.0f,-1.0f,1.0f,	//7
	};
	unsigned int indices[] = {
			0,1,3,
			1,2,3,
			4,5,7,
			5,6,7,
			5,1,6,
			1,2,6,
			4,0,7,
			0,3,7,
			4,5,0,
			5,1,0,
			7,6,3,
			6,2,3
	};
	float colors[] = {
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
	};

	float uv[]={
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
	};


	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	GLuint ebo;
	glGenBuffers(1,&ebo);


	GLuint vbo;
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cube),cube,GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(void*)0);
	glEnableVertexAttribArray(0);

	GLuint vboColors;
	glGenBuffers(1,&vboColors);
	glBindBuffer(GL_ARRAY_BUFFER,vboColors);
	glBufferData(GL_ARRAY_BUFFER,sizeof(colors),colors,GL_STATIC_DRAW);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(void*)0);
	glEnableVertexAttribArray(1);

	GLuint vboUV;
	glGenBuffers(1,&vboUV);
	glBindBuffer(GL_ARRAY_BUFFER,vboUV);
	glBufferData(GL_ARRAY_BUFFER,sizeof(uv),uv,GL_STATIC_DRAW);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2*sizeof(GL_FLOAT),(void*)0);
	glEnableVertexAttribArray(2);

	return vao;
}

GLuint loadTexture(const char* texturePath){
	int width,height,nrChannels;
	unsigned char* data = stbi_load(texturePath, &width,&height, &nrChannels, 0);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	return texture;

}
