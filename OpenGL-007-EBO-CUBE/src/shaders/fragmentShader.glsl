#version 440 core
in vec3 vColor;
in vec2 texCoord;
out vec3 color;
out vec4 fragColor;
uniform sampler2D texture1;
uniform sampler2D texture2;
void main(){
	fragColor = mix(texture(texture2,texCoord),texture(texture1,texCoord),0.2) * vec4(vColor,1.0f);
}