/*
 * OpenGL-010-FirstPersonCamera.cpp
 *
 *  Created on: 07-Apr-2018
 *      Author: naveen
 */


#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "ModelLoader.h"

using namespace std;
using namespace glm;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f,0.0f,-1.0f);
glm::vec3 cameraSide = glm::vec3(1.0f,0.0f,0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f,1.0f,0.0f);

float loadRandomNumber(void);
void glfw_key_callback(GLFWwindow* window,int key,int scancode,int action,int mods);
int main(){
	srand(time(0));
	if(!glfwInit()){
		cout << "Failed to initialize the glfw library" << endl;
		return EXIT_FAILURE;
	}
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Naveen Kumar", nullptr, nullptr);
	if(!window){
		cout << "Failed to initialize the window" << endl;
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		cout << "Failed to initialize the glew library" << endl;
		return EXIT_FAILURE;
	}

	ShaderProgram shaderProgramLoader;
	GLint shaderProgram = shaderProgramLoader.loadShaderProgram("src/shaders/vertexShader.glsl", "src/shaders/fragmentShader.glsl");
	if(shaderProgram == -1){
		cout << "Failed to load the shader program" <<endl;
		return EXIT_FAILURE;
	}
	GLint projectionId= glGetUniformLocation(shaderProgram,"projection");
	GLint viewId = glGetUniformLocation(shaderProgram,"view");
	GLint modelId = glGetUniformLocation(shaderProgram,"model");

	naveen::Model objModel;
	ModelLoader modelLoader;
	if(!modelLoader.loadObjFile("src/models/sphere.obj", objModel)){
		cout << "Failed to load the model object" << endl;
		return EXIT_FAILURE;
	}
	int vertexColorLength = objModel.vertices.size() * 3;
	GLfloat vertexColor[vertexColorLength];
	for(int i=0;i<vertexColorLength;i++){
		vertexColor[i] = loadRandomNumber();
	}

	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,objModel.vertices.size() * sizeof(glm::vec3),&objModel.vertices[0],GL_STATIC_DRAW);
	GLuint ebo;
	glGenBuffers(1,&ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,objModel.indices.size() * sizeof(unsigned int),&objModel.indices[0],GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(void*)0);
	glEnableVertexAttribArray(0);

	GLuint vboColors;
	glGenBuffers(1,&vboColors);
	glBindBuffer(GL_ARRAY_BUFFER,vboColors);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexColor),vertexColor,GL_STATIC_DRAW);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(void*)0);
	glEnableVertexAttribArray(1);



	glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)800/(float)600,0.1f,100.0f);

	glm::mat4 model = glm::mat4(1.0f);

	glfwSetKeyCallback(window, glfw_key_callback);
	while(!glfwWindowShouldClose(window)){
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glUseProgram(shaderProgram);
		glUniformMatrix4fv(projectionId,1,GL_FALSE,&projection[0][0]);
		glm::mat4 view = glm::lookAt(cameraPos,cameraPos + cameraFront,cameraUp);
		glUniformMatrix4fv(viewId,1,GL_FALSE,&view[0][0]);
		glUniformMatrix4fv(modelId,1,GL_FALSE,&model[0][0]);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, objModel.indices.size(), GL_UNSIGNED_INT, 0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return EXIT_SUCCESS;
}

void glfw_key_callback(GLFWwindow* window,int key, int scancode,int action,int mods){
	float cameraSpeed = 50.0f * deltaTime;
	switch(key){
	case GLFW_KEY_W:
		cameraPos += cameraSpeed * cameraFront;
		break;
	case GLFW_KEY_S:
		cameraPos -= cameraSpeed * cameraFront;
		break;
	case GLFW_KEY_D:
		cameraPos += cameraSpeed * cameraSide;
		break;
	case GLFW_KEY_A:
		cameraPos -= cameraSpeed * cameraSide;
		break;
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window,GL_TRUE);
		break;
	default:
		break;
	}
}
float loadRandomNumber(){
	return static_cast<float>(rand())/static_cast<float>(RAND_MAX);
}
