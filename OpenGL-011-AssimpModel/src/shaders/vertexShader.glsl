#version 440 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormals;
layout(location = 2) in vec2 aTexCoords;

out vec3 normals;
out vec2 texCoords;

uniform mat4 MVP;

void main(){
	gl_Position = MVP * vec4(aPos,1.0f);
	normals = aNormals;
	texCoords = aTexCoords;
}