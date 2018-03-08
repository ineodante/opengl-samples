#version 440 core
layout (location=0) in vec3 position;
out vec4 mycolor;
uniform mat4 MVP;
uniform vec4 colorData;
void main(){
	gl_Position = MVP * vec4(position,1);
	mycolor = colorData;
	
}