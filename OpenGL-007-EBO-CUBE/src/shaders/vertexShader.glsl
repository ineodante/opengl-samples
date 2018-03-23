#version 440 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 uv;

out vec3 vColor;
out vec2 texCoord;

uniform mat4 transform;
uniform mat4 MVP;
void main(){
	gl_Position = MVP * transform *  vec4(position,1.0f);	
	vColor = color;
	texCoord = uv;
}