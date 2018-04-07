/*
 * OpenGL-009-Camera-Rotation.cpp
 *
 *  Created on: 31-Mar-2018
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

#include "ModelLoader.h"
#include "ShaderProgram.h"

using namespace std;
using namespace glm;

bool fullscreen = false;
GLFWmonitor* monitor;
float loadRandomNumber(void);
void glfw_error_callback(int errorCode,const char* message);
void glfw_key_callback(GLFWwindow* window,int key,int scancode,int action, int mods);
int main(){
	srand(time(0));
	if(!glfwInit()){
		cout << "Failed to initialize glfw library" << endl;
		return EXIT_FAILURE;
	}
	glfwSetErrorCallback(glfw_error_callback);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);
	monitor = glfwGetPrimaryMonitor();
	if(!monitor){
		cout << "Failed to get primary monitor" << endl;
		return EXIT_FAILURE;
	}

	GLFWwindow* window = glfwCreateWindow(800,600,"Naveen Kumar",nullptr,nullptr);
	if(!window){
		cout <<"Failed to create the window" << endl;
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		cout <<"Failed to initialize glew library" << endl;
		return EXIT_FAILURE;
	}

	ShaderProgram shaderProgramGenerator;

	GLint shaderProgram = shaderProgramGenerator.loadShaderProgram("src/shaders/vertexShader.glsl", "src/shaders/fragmentShader.glsl");
	if(shaderProgram == -1){
		cout <<"Failed to generate shader program" << endl;
		return EXIT_FAILURE;
	}

	GLint projectionId = glGetUniformLocation(shaderProgram,"projection");
	GLint viewId = glGetUniformLocation(shaderProgram,"view");
	GLint modelId = glGetUniformLocation(shaderProgram,"model");

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800/(float)600, 0.1f,100.0f);



	naveen::Model objModel;
	ModelLoader modelLoader;
	modelLoader.loadObjFile("src/models/sphere.obj", objModel);

	cout << objModel.vertices.size() << endl;
	cout << sizeof(glm::vec3)<< endl;
	cout << objModel.indices.size() << endl;
	int vertexColorLength = objModel.vertices.size() * 3;
	GLfloat vertexColor[vertexColorLength];
	for(int i=0;i<vertexColorLength;i++){
		vertexColor[i] = loadRandomNumber();
	}
	 glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3 (2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -2.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

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

	glfwSetKeyCallback(window, glfw_key_callback);
	while(!glfwWindowShouldClose(window)){
		glEnable(GL_DEPTH_TEST | GL_POLYGON_SMOOTH);
		glDepthFunc(GL_LESS);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		float camX = glm::sin(glfwGetTime()) * 10.0f;
		float camY = glm::sin(glfwGetTime()) * 10.0f;
		float camZ = glm::cos(glfwGetTime()) * 10.0f;
		glm::mat4 view = glm::lookAt(glm::vec3(camX,0,camZ), glm::vec3(0,0,0), glm::vec3(0,1,0));
		glUseProgram(shaderProgram);
		glUniformMatrix4fv(projectionId,1,GL_FALSE,&projection[0][0]);
		glUniformMatrix4fv(viewId,1,GL_FALSE,&view[0][0]);
		glBindVertexArray(vao);
		for(int i=0;i<10;i++){
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;

			model = glm::rotate(model, angle, glm::vec3(camX,camY,camZ));
			model = glm::scale(model, vec3(0.8f,0.8f,0.8f));
			glUniformMatrix4fv(modelId,1,GL_FALSE,&model[0][0]);
			glDrawElements(GL_TRIANGLES, objModel.indices.size(), GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void glfw_error_callback(int errorCode,const char* message){
	cout <<"Error Code: " << errorCode << ": " << message << endl;
}

void glfw_key_callback(GLFWwindow* window,int key,int scancode,int action,inglEnableVertexAttribArray(1);t mods){
	if(action == GLFW_PRESS){
		switch(key){
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window,GL_TRUE);
			break;
		case GLFW_KEY_F:
			fullscreen = !fullscreen;
			if(fullscreen){
				glfwSetWindowMonitor(window, monitor, 0, 0, 800, 600, 60);
			}else{
				glfwSetWindowMonitor(window,NULL,0,0,800,600,60);
			}
			glViewport(0, 0, 800, 600);
			break;
		default:
			break;
		}
	}
}
float loadRandomNumber(){
	return static_cast<float>(rand())/static_cast<float>(RAND_MAX);
}
