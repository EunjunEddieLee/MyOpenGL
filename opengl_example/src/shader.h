#ifndef __SHADER_H__
#define __SHADER_H__

#include "common.h"

CLASS_PTR(Shader); // from common.h
class Shader {
public:
  static ShaderUPtr CreateFromFile(const std::string& filename, GLenum shaderType); // static ShaderUPtr --> 결과적으로 ShaderUPtr이라는 unique_ptr의 형태로만 생성 가능

  ~Shader();
  uint32_t Get() const { return m_shader; } // Get만 있고 Set은 없음. Shader object는 Shader 내부에서만 관리
private:
  Shader() {} // Shader 생성자가 private --> 외부에서 일반적으로는 생성 불가. public 영역인 CreateFromFile 함수를 통해서만 생성 가능
  bool LoadFile(const std::string& filename, GLenum shaderType); // bool --> 생성 실패 시 false return
  uint32_t m_shader { 0 };
};

#endif //__SHADER_H__