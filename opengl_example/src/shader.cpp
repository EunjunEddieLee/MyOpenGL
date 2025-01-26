#include "shader.h"

//ShaderUPtr : return type(unique_ptr<Shader>)
//Shader::CreateFromFile : function name
ShaderUPtr Shader::CreateFromFile(const std::string& filename, GLenum shaderType) {
  auto shader = ShaderUPtr(new Shader());
  if (!shader->LoadFile(filename, shaderType)) // file load 실패 시
    return nullptr;
  return std::move(shader); // unique pointer의 소유권 이전
}

// Shader 소멸자
// glDeleteShader(): shader object 제거
Shader::~Shader() {
  if (m_shader) {
    glDeleteShader(m_shader);
  }
}

bool Shader::LoadFile(const std::string& filename, GLenum shaderType) {
  auto result = LoadTextFile(filename);
  if (!result.has_value())
    return false;

                               // auto -> 타입 추측해줌 (= string)
                               // string& code -> 주소값 받아오기
  auto& code = result.value(); // --> result string의 주소값만 받아옴
  const char* codePtr = code.c_str();
  int32_t codeLength = (int32_t)code.length();

  // create and compile shader
  /* 
  glCreateShader(): OpenGL shader object 생성
  glShaderSource(): shader에 소스 코드 설정
  glCompileShader(): shader 컴파일
  */
  m_shader = glCreateShader(shaderType);
  glShaderSource(m_shader, 1, (const GLchar* const*)&codePtr, &codeLength);
  glCompileShader(m_shader);

  // check compile error
  /*
  glGetShaderiv(): shader에 대한 정수형 정보를 얻어옴
  glGetShaderInfoLog(): shader에 대한 로그를 얻어옴. 컴파일 에러 얻기 용
  */
  int success = 0;
  glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success); // iv : i=int, v=vector(or pointer)
  if (!success) { // 실패한 경우 log, error message 등 출력
    char infoLog[1024];
    glGetShaderInfoLog(m_shader, 1024, nullptr, infoLog);
    SPDLOG_ERROR("failed to compile shader: \"{}\"", filename);
    SPDLOG_ERROR("reason: {}", infoLog);
    return false;
  }
  return true;
}