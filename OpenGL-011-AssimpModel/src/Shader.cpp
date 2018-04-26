/*
 * Shader.cpp
 *
 *  Created on: 26-Apr-2018
 *      Author: naveen
 */

#include "Shader.h"

namespace naveen {

Shader::Shader() {
	shaderProgram = 0;

}

Shader::~Shader() {
	// TODO Auto-generated destructor stub
}

bool Shader::createShaderProgram(const char* vShaderFilePath,const char* fShaderFilePath){
	std::ifstream vShaderFile(vShaderFilePath,std::ios::in);
	if(!vShaderFile.is_open()){
		std::cout <<"Failed to open vertex shader file" << std::endl;
		return false;
	}
	std::stringstream vShaderStream;
	vShaderStream << vShaderFile.rdbuf();
	vShaderFile.close();
	std::string vShaderSource = vShaderStream.str();
	const char* vShaderCode = vShaderSource.c_str();
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader,1,&vShaderCode,NULL);
	glCompileShader(vShader);
	int result;
	glGetShaderiv(vShader,GL_COMPILE_STATUS,&result);
	if(!result){
		char message[512];
		glGetShaderInfoLog(vShader,512,NULL,message);
		std::cout <<"Vertex Shader Error: " << message << std::endl;
		return false;
	}

	std::ifstream fShaderFile(fShaderFilePath,std::ios::in);
	if(!fShaderFile.is_open()){
		std::cout <<"Failed to open fragment shader file" << std::endl;
		return false;
	}
	std::stringstream fShaderStream;
	fShaderStream << fShaderFile.rdbuf();
	fShaderFile.close();
	std::string fShaderSource = fShaderStream.str();
	const char* fShaderCode  = fShaderSource.c_str();
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader,1,&fShaderCode,NULL);
	glCompileShader(fShader);
	glGetShaderiv(fShader,GL_COMPILE_STATUS,&result);
	if(!result){
		char message[512];
		glGetShaderInfoLog(fShader,512,NULL,message);
		std::cout <<"Fragment Shader Error: " << message << std::endl;
		return false;
	}
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram,vShader);
	glAttachShader(shaderProgram,fShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram,GL_LINK_STATUS,&result);
	if(!result){
		char message[512];
		glGetProgramInfoLog(shaderProgram,512,NULL,message);
		std::cout <<"Shader Program Error: " << message << std::endl;
		return false;
	}
	glDetachShader(shaderProgram,vShader);
	glDetachShader(shaderProgram,fShader);
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	return true;
}

void Shader::useShader(){
	glUseProgram(shaderProgram);
}

void Shader::setUniformMatrix4fv(const char* uniformName,glm::mat4 data){
	GLuint uniform = glGetUniformLocation(shaderProgram,"MVP");
	glUniformMatrix4fv(uniform,1,GL_FALSE,&data[0][0]);
}

} /* namespace naveen */
