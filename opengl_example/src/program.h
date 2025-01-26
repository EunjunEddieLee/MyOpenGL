#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "common.h"
#include "shader.h"

CLASS_PTR(Program)
class Program {
public:
  static ProgramUPtr Create(const std::vector<ShaderPtr>& shaders); // shader들은 다른 program instance 생성할 때 재사용 가능, 따라서 shared pointer 사용

  ~Program();
  uint32_t Get() const { return m_program; }
  void Use() const;
private:
  Program() {}
  bool Link(const std::vector<ShaderPtr>& shaders);
  uint32_t m_program { 0 };
};


#endif // __PROGRAM_H__