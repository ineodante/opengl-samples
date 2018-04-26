/*
 * main.cpp
 *
 *  Created on: 26-Apr-2018
 *      Author: naveen
 */

#include <iostream>
#include "Window.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Mesh.h"
using namespace std;
int main(int argc,char** argv){
	naveen::Window* window = new naveen::Window();
	if(!window->createWindow(1024, 567, "Naveen Kumar", false)){
		cout <<"Failed to initialize primary window" << endl;
		return EXIT_FAILURE;
	}

	naveen::Shader* shader = new naveen::Shader();
	if(!shader->createShaderProgram("src/shaders/vertexShader.glsl", "src/shaders/fragmentShader.glsl")){
		std::cout<<"Failed to create the shader program " << std::endl;
		return EXIT_FAILURE;
	}

	naveen::Mesh* mesh = new naveen::Mesh();
	if(!mesh->loadModel("src/models/nanosuit/nanosuit.obj")){
		std::cout <<"Failed to load the model" <<std::endl;
		return EXIT_FAILURE;
	}

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.78f, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(2,1,1),glm::vec3(0,1,0),glm::vec3(0,1,0));
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::scale(model, glm::vec3(0.1f,0.1f,0.1f));

	float deltaTime = 0.0f;
	float lastFrame= 0.0f;

	while(window->isOpen()){
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame-lastFrame;
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		model = glm::rotate(model,(deltaTime * 0.4f),glm::vec3(0,1,0));
		glm::mat4 mvp = projection * view * model;
		shader->setUniformMatrix4fv("MVP",mvp);
		mesh->drawMesh(shader);
		window->pollEvents();
		window->swapBuffers();
		lastFrame = currentFrame;
	}
	delete(window);
	return EXIT_SUCCESS;
}


