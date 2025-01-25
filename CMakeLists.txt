# 컴파일러에 /utf-8 플래그 추가
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME MyOpenGL)
set(CMAKE_CXX_STANDARD 17)

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} src/main.cpp)

include(Dependency.cmake)

# 
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

# 
add_dependencies(${PROJECT_NAME} ${DEP_LIST})