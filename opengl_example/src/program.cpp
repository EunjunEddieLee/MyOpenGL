#include "program.h"

ProgramUPtr Program::Create(const std::vector<ShaderPtr>& shaders) {
  auto program = ProgramUPtr(new Program());
  if (!program->Link(shaders))
    return nullptr;
  return std::move(program);
}

// glDeleteProgram(): program object 제거
Program::~Program() {
  if (m_program) {
    glDeleteProgram(m_program);
  }
}

/*
glCreateProgram(): OpenGL program object 생성
glAttachShader(): program에 shader 붙이기
glLinkProgram(): program 링크

glGetProgramiv(): program에 대한 정수형 정보 불러오기
glGetProgramInfoLog(): program에 대한 로그 얻어옴. 링크 에러 얻기 용
*/
bool Program::Link(const std::vector<ShaderPtr>& shaders) {
  m_program = glCreateProgram();
  for (auto& shader: shaders)
    glAttachShader(m_program, shader->Get());
  glLinkProgram(m_program);

  int success = 0;
  glGetProgramiv(m_program, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[1024];
    glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
    SPDLOG_ERROR("failed to link program: {}", infoLog);
    return false;
  }
  return true;
}

void Program::Use() const {
  glUseProgram(m_program);
}