#include <spdlog/spdlog.h>
#include <glad/glad.h> // GLFW library 전에 include
#include <GLFW/glfw3.h>

/*
  glViewport() : OpenGL이 그림을 그릴 화면의 위치 및 크기 설정
  
  - 화면 클리어(두 단계)
  glClearColor() : 컬러 프레임버퍼 화면을 클리어 할 색상 지정
  glClear() : 프레임 버퍼 클리어
  
  - 클리어 된 화면 보이기
  glfwSwapBuffers();
  ^^
  - 그림 그려지는 원리
      front/back 두 buffer가 존재. 그림이 그려지는 과정은 back buffer에서 진행(지저분) 그림이 완성되면 swapBuffer를 통해 front, back buffer swap


  - State-setting function
      State가 OpenGL context에 저장됨
      glViewport(), glClearColor()
  - State-using function
      OpenGL context에 저장된 State를 이용
      glClear()
*/


void OnFrameBufferSizeChange(GLFWwindow* window, int width, int height) {
  SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
  glViewport(0, 0, width, height);
}

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
  SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
    key, scancode,
    action == GLFW_PRESS ? "Pressed" :
    action == GLFW_RELEASE ? "Released" :
    action == GLFW_REPEAT ? "Repeat" : "Unknown",
    mods & GLFW_MOD_CONTROL ? "C" : "-",
    mods & GLFW_MOD_SHIFT ? "S" : "-",
    mods & GLFW_MOD_ALT ? "A" : "-");
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}


int main(int argc, const char** argv) {
   SPDLOG_INFO("Start Program");

  // glfw 라이브러리 초기화, 실패하면 에러 출력 후 종료
  SPDLOG_INFO("Initialize glfw");
  if (!glfwInit()) {
    const char* description = nullptr;
    glfwGetError(&description);
    SPDLOG_ERROR("failed to initialize glfw: {}", description);
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw 윈도우 생성, 실패하면 에러 출력 후 종료
  SPDLOG_INFO("Create glfw window");
  auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
  if (!window) {
    SPDLOG_ERROR("failed to create glfw window");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // glad를 활용한한 OpenGL 함수 로딩
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    SPDLOG_ERROR("failed to initialize glad");
    glfwTerminate();
    return -1;
  }
  auto glVersion = glGetString(GL_VERSION);
  SPDLOG_INFO("OpenGL context version: {}", reinterpret_cast<const char*>(glVersion));

  OnFrameBufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(window, OnFrameBufferSizeChange);
  glfwSetKeyCallback(window, OnKeyEvent);

  // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
  SPDLOG_INFO("Start main loop");
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    
    // 화면 클리어
    glClearColor(0.0f, 0.1f, 0.2f, 0.3f); // 컬러 프레임버퍼 화면을 클리어 할 색상 지정
    glClear(GL_COLOR_BUFFER_BIT); // 프레임 버퍼 클리어
    
    // 클리어 된 화면 보이기
    glfwSwapBuffers(window);
    /* 그림 그려지는 원리 : front/back 두 buffer가 존재. 그림이 그려지는 과정은 back buffer에서 진행(지저분) 그림이 완성되면 swapBuffer를 통해 front, back buffer swap */
  }

  glfwTerminate();
  return 0;
}