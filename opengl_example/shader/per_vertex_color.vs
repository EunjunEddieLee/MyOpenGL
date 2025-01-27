#version 330 core
/*
- layout :
  이 vertex shader에 원하는 attribute의 index를 지정
*/
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec4 vertexColor;

void main() {
  gl_Position = vec4(aPos, 1.0);
  vertexColor = vec4(aColor, 1.0);
}