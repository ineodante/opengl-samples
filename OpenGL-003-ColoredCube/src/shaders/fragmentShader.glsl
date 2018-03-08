#version 440 core

in vec4 mycolor;
out vec3 color;
void main(){
color = vec3(mycolor.x,mycolor.y,mycolor.z);
}
