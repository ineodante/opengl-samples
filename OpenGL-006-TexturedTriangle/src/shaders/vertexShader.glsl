#version 440 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 uv;

uniform int colorVal;
uniform mat4 MVP;

out vec3 vColor;
out vec2 texCoord;
void main(){
	gl_Position = MVP * vec4(position,1.0f);
	
	vColor = vec3(1.0f,1.0f,1.0f);
	if( colorVal > 1)
		vColor = color * colorVal;
			
	texCoord = uv;
}