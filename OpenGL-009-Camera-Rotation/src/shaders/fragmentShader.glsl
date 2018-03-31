#version 440 core

out vec3 color;
in vec3 vColor;
void main(){
	color = vColor;
	//color = vec3(1.0f,0.0f,0.0f);
}