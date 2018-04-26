#version 440 core

in vec3 normals;
in vec2 texCoords;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

out vec4 fragColor;

void main(){
	fragColor = mix(texture(diffuseTexture,texCoords),texture(specularTexture,texCoords),0.2f);
}