/*
 * ShaderProgram.h
 *
 *  Created on: 31-Mar-2018
 *      Author: naveen
 */

#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_
#include <iostream>
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <string>

class ShaderProgram{
public:
	GLint loadShaderProgram(const char* vertexShaderPath,const char* fragmentShaderPath);
};
#endif /* SHADERPROGRAM_H_ */
