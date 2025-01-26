#include "context.h"

ContextUPtr Context::Create() {
  auto context = ContextUPtr(new Context());
  if (!context->Init())
    return nullptr;
  return std::move(context);
}

bool Context::Init() {
  // OpenGL 함수 로딩 후에야 shader 불러오기 위한 함수들 사용 가능
  ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
  ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
  if (!vertShader || !fragShader)
    return false;
  SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
  SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

  m_program = Program::Create({fragShader, vertShader});
  if (!m_program)
    return false;
  SPDLOG_INFO("prigram id: {}", m_program->Get());
  
  // 화면 클리어
  glClearColor(0.0f, 0.1f, 0.2f, 0.3f); // 컬러 프레임버퍼 화면을 클리어 할 색상 지정

  // shader progam test
  uint32_t vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  return true;
}

void Context::Render() {
  glClear(GL_COLOR_BUFFER_BIT); // 프레임 색상 버퍼 클리어

  glUseProgram(m_program->Get());
  glDrawArrays(GL_POINTS, 0, 1);
}