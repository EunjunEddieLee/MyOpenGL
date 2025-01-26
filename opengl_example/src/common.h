#ifndef __COMMON_H__
#define __COMMON_H__

#include <memory>
#include <string>
#include <optional>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>

// CLASS_PTR이라는 함수를 define (klassName이 파라미터, 아래 줄들이 함수가 하는 명령)
#define CLASS_PTR(klassName) \
class klassName; \
using klassName ## UPtr = std::unique_ptr<klassName>; \
using klassName ## Ptr = std::shared_ptr<klassName>; \
using klassName ## WPtr = std::weak_ptr<klassName>;
/*
ex)
CLASS_PTR(Shader);
-->
class Shader;
using ShaderUPtr = std::unique_ptr<Shader>;
using ShaderPtr = std::shared_ptr<Shader>;
using ShaderWPtr = std::weak_ptr<Shader>;
(using : 위의 단어를 옆의 것으로 사용 - define같은 기능능)
*/

std::optional<std::string> LoadTextFile(const std::string& filename);

#endif // __COMMON_H__