#version 440 core
in vec3 vColor;
in vec2 texCoord;

out vec3 fColor;

uniform sampler2D myTextureSampler;

void main(){
	fColor = texture(myTextureSampler,texCoord).rgb * vColor;
}