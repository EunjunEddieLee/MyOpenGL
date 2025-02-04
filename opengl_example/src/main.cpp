#include "context.h"

#include <spdlog/spdlog.h>
#include <glad/glad.h> // GLFW library 전에 include
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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

/*
** OpenGL을 이용하여 삼각형을 그리는 과정 **
  - shader object 생성 / 소스 컴파일
  - program object 생성 / shader link (vs, fs 들)
  - VAO: VBO 구조에 대한 description, 바인딩 된 VBO, EBO를 기억
  - VBO: 정점 데이터를 GPU 메모리 상에 위치시킨 object
  - EBO: 인덱스 데이터를 GPU 메모리 상에 위치시킨 object

  대부분의 OpenGL object는 glBindXX()라는 함수를 이용하여 지금부터 사용할 object를 선택한 뒤 이용한다.
  - glBindBuffer()
  - glBindVertexArray()
  - glBindTexture()
  - glBindFramebuffer()
    ...
*/

/*
** C++ Code Refactoring
  Smart pointer의 기능을 이용한 리소스 자동 해제
  static method / private constructor를 이용한 인스턴스 생성 방식 제한
  --> 런타임 단계가 아닌 컴파일 단계에서 에러를 검출할 수 있도록
  private member 설정을 통한 수정 권한 제한

*/

void OnFrameBufferSizeChange(GLFWwindow* window, int width, int height) {
  SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
  // ** Typecast in C, C++
  // int i = (int)0.0f; // C
  // 
  // int j = static_cast<int>(0.0f); // C++
  // dynamic_cast<>
  // const_cast<>
  // reinterpret_cast<>
  auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
  context->Reshape(width, height);
}

void OnCursorPos(GLFWwindow* window, double x, double y) {
  auto context = (Context*)glfwGetWindowUserPointer(window);
  context->MouseMove(x, y);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int modifier) {
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, modifier);
  auto context = (Context*)glfwGetWindowUserPointer(window);
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  context->MouseButton(button, action, x, y);
}

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
  ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

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

void OnCharEvent(GLFWwindow* window, unsigned int ch) {
  ImGui_ImplGlfw_CharCallback(window, ch);
}

void OnScroll(GLFWwindow* window, double xoffset, double yoffset) {
  ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
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

  // glad를 활용한 OpenGL 함수 로딩
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    SPDLOG_ERROR("failed to initialize glad");
    glfwTerminate();
    return -1;
  }
  auto glVersion = glGetString(GL_VERSION);
  // reinterpret_cast<const char*> : error 해결용
  SPDLOG_INFO("OpenGL context version: {}", reinterpret_cast<const char*>(glVersion));
  
  auto imguiContext = ImGui::CreateContext();
  ImGui::SetCurrentContext(imguiContext);
  ImGui_ImplGlfw_InitForOpenGL(window, false);
  ImGui_ImplOpenGL3_Init();
  ImGui_ImplOpenGL3_CreateFontsTexture();
  ImGui_ImplOpenGL3_CreateDeviceObjects();

  auto context = Context::Create();
  if (!context) {
    SPDLOG_ERROR("failed to create context");
    glfwTerminate();
    return -1;
  }
  glfwSetWindowUserPointer(window, context.get()); // glfw User Pointer

  OnFrameBufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
  glfwSetFramebufferSizeCallback(window, OnFrameBufferSizeChange);
  glfwSetKeyCallback(window, OnKeyEvent);
  glfwSetCharCallback(window, OnCharEvent);
  glfwSetCursorPosCallback(window, OnCursorPos);
  glfwSetMouseButtonCallback(window, OnMouseButton);
  glfwSetScrollCallback(window, OnScroll);


  glfwSwapInterval(1); // Vsync 활성화 (60FPS로 맞추기)
  // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
  SPDLOG_INFO("Start main loop");
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    context->ProcessInput(window);
    context->Render();
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }
  context.reset();

  ImGui_ImplOpenGL3_DestroyFontsTexture();
  ImGui_ImplOpenGL3_DestroyDeviceObjects();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext(imguiContext);

  glfwSwapInterval(0);
  glfwTerminate();
  return 0;
}