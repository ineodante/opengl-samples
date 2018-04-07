/*
 * ShaderProgram.cpp
 *
 *  Created on: 31-Mar-2018
 *      Author: naveen
 */

#include "ShaderProgram.h"

GLint ShaderProgram::loadShaderProgram(const char* vertexShaderPath,const char* fragmentShaderPath){
	std::ifstream vertexShaderFile(vertexShaderPath,std::ios::in);
	if(!vertexShaderFile.is_open()){
		std::cout << "Failed to open vertex shader file" << std::endl;
		return -1;
	}
	std::stringstream vertexShaderStream;
	vertexShaderStream << vertexShaderFile.rdbuf();
	vertexShaderFile.close();
	std::string vertexShaderSource = vertexShaderStream.str();
	const char* vertexShaderCode = vertexShaderSource.c_str();
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,1,&vertexShaderCode,NULL);
	glCompileShader(vertexShader);
	int result;
	glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&result);
	delete(vertexShaderCode);
	if(!result){
		char message[512];
		glGetShaderInfoLog(vertexShader,512,NULL,message);
		std::cout << "Vertex Shader Error: " << message << std::endl;

		return -1;
	}

	std::ifstream fragmentShaderFile(fragmentShaderPath,std::ios::in);
	if(!fragmentShaderFile.is_open()){
		std::cout << "Failed to open the fragment shader file" << std::endl;
		return -1;
	}
	std::stringstream fragmentShaderStream;
	fragmentShaderStream << fragmentShaderFile.rdbuf();
	fragmentShaderFile.close();
	std::string fragmentShaderSource = fragmentShaderStream.str();
	const char* fragmentShaderCode = fragmentShaderSource.c_str();
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1,&fragmentShaderCode,NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&result);
	delete(fragmentShaderCode);
	if(!result){
		char message[512];
		glGetShaderInfoLog(fragmentShader,512,NULL,message);
		std::cout <<"Fragment Shader Error: " << message << std::endl;
		return -1;
	}
	GLint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram,GL_LINK_STATUS,&result);
	if(!result){
		char message[512];
		glGetProgramInfoLog(shaderProgram,512,NULL,message);
		std::cout << "Shader Program Link Error: " << message << std::endl;
		return -1;
	}
	glDetachShader(shaderProgram,vertexShader);
	glDetachShader(shaderProgram,fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}
