/*
 * main.cpp
 *
 *  Created on: 18-Mar-2018
 *      Author: naveen
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <imageloader/stb_image.h>

using namespace std;
using namespace glm;

GLFWmonitor* monitor;
int fullscreen;

void glfw_key_callback(GLFWwindow* window,int key,int scancode,int action,int mods);
void glfw_error_callback(int errorCode,const char* errorMessage);
GLuint loadShaderProgram(const char* vShaderFilePath,const char* fShaderFilePath);

GLuint loadTriangle(GLfloat);
GLuint loadTexture(const char* texturePath);
float loadRandomNumber();
int main(){
	srand(time(0));
	if(!glfwInit()){
		cout <<"Failed to initialize glfw library" << endl;
		return EXIT_FAILURE;
	}

	glfwSetErrorCallback(glfw_error_callback);

	monitor = glfwGetPrimaryMonitor();
	if(!monitor){
		cout << "Failed to get primary monitor details" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	GLFWwindow* window = glfwCreateWindow(800,600,"Naveen Kumar",nullptr,nullptr);
	if(!window){
		cout << "Failed to create the primary window" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		cout << "Failed to initialize GLEW library" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}


	GLuint shaderProgram = loadShaderProgram("src/shaders/vertexShader.glsl","src/shaders/fragmentShader.glsl");
	if(!shaderProgram){
		cout << "Failed to load shader program" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	GLint mvpId = glGetUniformLocation(shaderProgram,"MVP");
	if(mvpId == -1){
		cout << "Failed to get uniform location" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	GLint colorIntensity = glGetUniformLocation(shaderProgram,"colorVal");
	if(colorIntensity  == -1){
		cout << "Failed to get color intensity uniform location" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}


	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800/(float)600, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(2,1,-5), glm::vec3(0,0,0), glm::vec3(0,1,0));
	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 mvp = projection * view * model;


	GLuint texture = loadTexture("src/textures/wall.jpg");
	GLuint triangle1 = loadTriangle(0.0f);
	GLuint triangle2 = loadTriangle(1.0f);
	GLuint triangle3 = loadTriangle(-1.0f);


	glfwSetKeyCallback(window, glfw_key_callback);
	while(!glfwWindowShouldClose(window)){
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glBindTexture(GL_TEXTURE_2D,texture);
		glUseProgram(shaderProgram);
		glUniformMatrix4fv(mvpId,1,GL_FALSE,&mvp[0][0]);
		float timeValue= glfwGetTime();
		float colorVal = abs(10 * sin(timeValue));
		glUniform1i(colorIntensity,colorVal);
		glBindVertexArray(triangle1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glBindVertexArray(triangle2);
		glDrawArrays(GL_TRIANGLES,0,3);
		glBindVertexArray(0);
		glBindVertexArray(triangle3);
		glDrawArrays(GL_TRIANGLES,0,3);
		glBindVertexArray(0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return EXIT_SUCCESS;
}


void glfw_key_callback(GLFWwindow* window,int key,int scancode,int action,int mods){
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
				glfwSetWindowMonitor(window,NULL,0,0,800,600,60);
			}
			break;
		default:
			break;
		}
	}
}

void glfw_error_callback(int errorCode,const char* errorMessage){
	cout << "Error Code: " << errorCode << " : " << errorMessage << endl;
}

GLuint loadShaderProgram(const char* vShaderFilePath,const char* fShaderFilePath){
	ifstream vShaderFile(vShaderFilePath,std::ios::in);
	if(!vShaderFile.is_open()){
		cout << "Failed to open vertex shader file" << endl;
		return 0;
	}
	stringstream vShaderStream;
	vShaderStream << vShaderFile.rdbuf();
	vShaderFile.close();
	string vShaderSource = vShaderStream.str();
	const char* vShaderCode = vShaderSource.c_str();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,1,&vShaderCode,NULL);
	glCompileShader(vertexShader);
	int result;
	glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&result);
	if(!result){
		char message[512];
		glGetShaderInfoLog(vertexShader,512,NULL,message);
		cout << "Vertex Shader Error: " << message << endl;
		return 0;
	}
	delete(vShaderCode);

	ifstream fShaderFile(fShaderFilePath,std::ios::in);
	if(!fShaderFile.is_open()){
		cout << "Failed to open fragment shader file" << endl;
		return 0;
	}
	stringstream fShaderStream;
	fShaderStream << fShaderFile.rdbuf();
	fShaderFile.close();
	string fShaderSource = fShaderStream.str();
	const char* fShaderCode = fShaderSource.c_str();
	GLuint fragmentShader  = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1,&fShaderCode,NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&result);
	if(!result){
		char message[512];
		glGetShaderInfoLog(fragmentShader,512,NULL,message);
		cout << "Fragment shader error" << message << endl;
		return 0;
	}
	delete(fShaderCode);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram,GL_LINK_STATUS,&result);
	if(!result){
		char message[512];
		glGetProgramInfoLog(shaderProgram,512,NULL,message);
		cout <<"Shader Program Error: " << message << endl;
		return 0;
	}

	glDetachShader(shaderProgram,vertexShader);
	glDetachShader(shaderProgram,fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

GLuint loadTriangle(GLfloat zValue){

		const GLfloat triangle[] = {
				-1.0f,-1.0f,zValue, 1.0f,0.0f,0.0f,  0.0f,0.0f,
				1.0f,-1.0f,zValue, 0.0f,1.0f,0.0f,   1.0f,0.0f,
				0.0f,1.0f,zValue, 0.0f,0.0f,1.0f,    0.5f,1.0f
		};

		GLuint vao;
		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);

		GLuint vboVertex;
		glGenBuffers(1,&vboVertex);
		glBindBuffer(GL_ARRAY_BUFFER,vboVertex);
		glBufferData(GL_ARRAY_BUFFER,sizeof(triangle),triangle,GL_STATIC_DRAW);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(void*)(3*sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(void*)(6*sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		return vao;
}

GLuint loadTexture(const char* texturePath){
	int width,height,nrChannels;
		unsigned char *data = stbi_load(texturePath,&width,&height,&nrChannels,0);
		if(!data){
			cout<<"Failed to load the texture file" << endl;
			return 0;
		}
	GLuint texture;
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	return texture;
}

float loadRandomNumber(){
	return static_cast<float>(rand())/static_cast<float>(RAND_MAX);
}
