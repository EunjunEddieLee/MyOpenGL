#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;
uniform mat4 modelTransform;

out vec3 normal;
out vec2 texCoord;
out vec3 position;

void main() {
  // gl_Position : 화면 상에서 점의 좌표 (canonical space) (카메라 입장)
  // position : World coordinate에서의 점의 좌표 -> diffusion 값 계산 가능
  gl_Position = transform * vec4(aPos, 1.0);
  // inverse transpose를 곱하는 이유 : 점이 아닌 벡터의 변환된 값을 계산하기 위한 방법
  normal = (transpose(inverse(modelTransform)) * vec4(aNormal, 0.0)).xyz;
  texCoord = aTexCoord;
  position = (modelTransform * vec4(aPos, 1.0)).xyz;
}