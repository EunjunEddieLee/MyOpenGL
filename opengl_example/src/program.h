#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "common.h"
#include "shader.h"

CLASS_PTR(Program)
class Program {
public:
  // shader들은 다른 program instance 생성할 때 재사용 가능, 따라서 shared pointer 사용
  static ProgramUPtr Create(const std::vector<ShaderPtr>& shaders);
  // 여러 개의 Program을 간단하게 사용하기 위한 함수
  static ProgramUPtr Create(
    const std::string& vertShaderFilename,
    const std::string& fragShaderFilename);

  ~Program();
  uint32_t Get() const { return m_program; }
  void Use() const;

  void SetUniform(const std::string& name, int value) const;
  void SetUniform(const std::string& name, float value) const;
  void SetUniform(const std::string& name, const glm::vec3& value) const;
  void SetUniform(const std::string& name, const glm::vec4& value) const;
  void SetUniform(const std::string& name, const glm::mat4& value) const;
private:
  Program() {}
  bool Link(const std::vector<ShaderPtr>& shaders);
  uint32_t m_program { 0 };
};


#endif // __PROGRAM_H__