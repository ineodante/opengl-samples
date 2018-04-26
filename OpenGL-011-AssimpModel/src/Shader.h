/*
 * Shader.h
 *
 *  Created on: 26-Apr-2018
 *      Author: naveen
 */

#ifndef SHADER_H_
#define SHADER_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace naveen {

class Shader {
private:
	GLuint shaderProgram;
public:
	Shader();
	virtual ~Shader();
	bool createShaderProgram(const char* vShaderFilePath,const char* fShaderFilePath);
	void useShader();
	void setUniformMatrix4fv(const char* uniformName,glm::mat4 data);
};

} /* namespace naveen */

#endif /* SHADER_H_ */
