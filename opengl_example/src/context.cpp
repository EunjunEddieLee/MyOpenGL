#include "context.h"
#include "image.h"
#include <imgui.h>

ContextUPtr Context::Create() {
  auto context = ContextUPtr(new Context());
  if (!context->Init())
    return nullptr;
  return std::move(context);
}

// glfwGetKey() -> window에서 어떤 키가 눌렸는지 판단하는 함수
void Context::ProcessInput(GLFWwindow* window) {
  if (!m_cameraControl)
    return;

  const float cameraSpeed = 0.05f;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    m_cameraPos += cameraSpeed * m_cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    m_cameraPos -= cameraSpeed * m_cameraFront;

  auto cameraRight = glm::normalize(glm::cross(m_cameraUp, -m_cameraFront));
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    m_cameraPos += cameraSpeed * cameraRight;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    m_cameraPos -= cameraSpeed * cameraRight;    

  auto cameraUp = glm::normalize(glm::cross(-m_cameraFront, cameraRight));
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    m_cameraPos += cameraSpeed * cameraUp;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    m_cameraPos -= cameraSpeed * cameraUp;
}

void Context::Reshape(int width, int height) {
  m_width = width;
  m_height = height;
  glViewport(0, 0, m_width, m_height);
}

void Context::MouseMove(double x, double y) {
  if (!m_cameraControl)
    return;

  auto pos = glm::vec2((float)x, (float)y);
  auto deltaPos = pos - m_prevMousePos;

  const float cameraRotSpeed = WINDOW_HEIGHT/(float)m_height * 0.8f;
  m_cameraYaw -= deltaPos.x * cameraRotSpeed;
  m_cameraPitch -= deltaPos.y * cameraRotSpeed;

  if (m_cameraYaw < 0.0f)   m_cameraYaw += 360.0f;
  if (m_cameraYaw > 360.0f) m_cameraYaw -= 360.0f;

  if (m_cameraPitch > 89.0f)  m_cameraPitch = 89.0f;
  if (m_cameraPitch < -89.0f) m_cameraPitch = -89.0f;

  m_prevMousePos = pos;
}

void Context::MouseButton(int button, int action, double x, double y) {
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS) {
      // 마우스 조작 시작 시점에 현재 마우스 커서 위치 저장
      m_prevMousePos = glm::vec2((float)x, (float)y);
      m_cameraControl = true;
    }
    else if (action == GLFW_RELEASE) {
      m_cameraControl = false;
    }
  }
}

bool Context::Init() {
  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
  };

  uint32_t indices[] = {
    0,  2,  1,  2,  0,  3,
    4,  5,  6,  6,  7,  4,
    8,  9, 10, 10, 11,  8,
    12, 14, 13, 14, 12, 15,
    16, 17, 18, 18, 19, 16,
    20, 22, 21, 22, 20, 23,
  };

  /*
  ** Vertex Buffer Object (VBO)

  glGenBuffers(): 새로운 buffer object를 만든다
  glBindBuffer(): 지금부터 사용할 buffer object 지정
    - GL_ARRAY_BUFFER: 사용할 buffer object가 vertex data를 저장할 용도임을 알려줌
  glBufferData(): 지정된 buffer에 데이터를 복사
    - 데이터의 총 크기, 데이터 포인터, 용도를 지정
    - GL_STATIC|DINAMIC|STREAM_DRAW|COPY|READ : 용도를 위한 flag

    - 정점 데이터 구조 :
    | vertex 1  | vertex 2  | vertex 3  |
    | x | y | z | x | y | z | x | y | z |
    0   4   8   12          24          36

    * 위치(x, y, z) 말고도 다양한 attribute 존재
    -> color, normal, tangent 등


  위의 정점 데이터의 구조를 알려주는 오브젝트가 필요함
  (각 정점이 몇 byte? 두 정점이 몇 byte만큼 떨어짐? 등)
  --> VAO
  --------------------------------------------------
  ** Vertex Array Object (VAO)

  glGenVertexArrays(): VAO 생성
  glBindVertexArray(): 지금부터 사용할 VAO 설정
  glEnableVertexAttribArray(n): 정점 attribute 중 n번째를 사용하도록 설정
  glVertexAttribPointer(n, size, type, normalize, stride, offset):
    - n: 정점의 n번째 attribute
    - size: 해당 attribute가 몇 개의 값으로 구성되어 있는가?
    - type: 해당 attribute의 데이터 타입
    - normalized: 0~1 사이의 값인가
    - stride: 두 정점 간의 간격 (byte 단위)
    - offset: 첫 정점의 해당 attribute 까지의 간격 (byte 단위)

  순서 주의 - VAO binding -> VBO binding -> vertex attribute setting
  --------------------------------------------------
  ** Element Buffer Object (EBO)
    정점의 인덱스를 저장할 수 있는 버퍼 (= 인덱스 버퍼)
    정점 정보와 별개로 정점의 인덱스로만 구성된 삼각형 정보를 전달 가능
    indexed drawing
  --------------------------------------------------
  glGenTexture(): OpenGL texture object 생성
  glBindTexture(): 사용하고자 하는 텍스처 바인딩
  glTexParameteri(): 텍스처 필터 / 래핑 방식 등 파라미터 설정

  glTexImage2D(target, level, internalFormat, width, height, border, format, type, data)
  -> 바인딩된 텍스처의 크기/픽셀 포맷을 설정하고 GPU에 이미지 데이터를 복사
    - target: 대상이 될 바인딩 텍스처
    - level: 설정할 텍스처 레벨. 0레벨이 base
    - internalFormat: 텍스처의 픽셀 포맷


  */

  m_vertexLayout = VertexLayout::Create();
  //vao 생성 후에 buffer를 만들어야 연결 가능
  m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 8 * 6 * 4);

  m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
  m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
  m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);

  m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 36);

  // OpenGL 함수 로딩 후에야 shader 불러오기 위한 함수들 사용 가능
  m_simpleProgram = Program::Create("./shader/simple.vs", "./shader/simple.fs");
  if (!m_simpleProgram)
    return false;

  m_program = Program::Create("./shader/lighting.vs", "./shader/lighting.fs");
  if (!m_program)
    return false;
  
  // 화면 클리어
  glClearColor(0.0f, 0.1f, 0.2f, 0.3f); // 컬러 프레임버퍼 화면을 클리어 할 색상 지정
  

  auto image = Image::Load("./image/container.jpg");
  m_texture = Texture::CreateFromImage(image.get());

  auto image2 = Image::Load("./image/chillguy.png");
  m_texture2 = Texture::CreateFromImage(image2.get());

  m_material.diffuse = Texture::CreateFromImage(Image::Load("./image/container2.png").get());
  
  m_material.specular = Texture::CreateFromImage(Image::Load("./image/container2_specular.png").get()); 

  // 두 개 이상의 이미지로 텍스처를 만드려면 텍스처 슬롯을 이용해야 한다.
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texture->Get());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

  m_program->Use();
  m_program->SetUniform("tex", 0);
  m_program->SetUniform("tex2", 1);

  return true;
}



/*
glDrawArray(primitive, offset, count)
  - 현재 설정된 program, VBO, VAO로 그림을 그린다
  - primitive: 그리고자 하는 primitive 타입
  - offset: 그리고자 하는 첫 정점의 index
  - count: 몇 개의 정점이 있는가

glDrawElements(primitive, count, type, pointer/offset)
  - 현재 바인딩된 VAO, VBO, EBO 바탕으로 그림
  - primitive: 그려낼 기본 primitive 타입
  - count: 그리고자 하는 EBO 내 index의 개수
  - type: index의 데이터형
  - pointer/offset: 그리고자 하는 EBO의 첫 데이터로부터의 오프셋
*/
void Context::Render() {
  if (ImGui::Begin("ui window")) {
    // color
    if (ImGui::ColorEdit4("clear color", glm::value_ptr(m_clearColor))) {
      glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
    }
    ImGui::Separator();
    // camera
    ImGui::DragFloat3("camera pos", glm::value_ptr(m_cameraPos), 0.01f);
    ImGui::DragFloat("camera yaw", &m_cameraYaw, 0.5f);
    ImGui::DragFloat("camera pitch", &m_cameraPitch, 0.5f, -89.0f, 89.0f);
    ImGui::Separator();
    if (ImGui::Button("reset camera")) {
      m_cameraYaw = 0.0f;
      m_cameraPitch = 0.0f;
      m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    }
    // lighting
    if (ImGui::CollapsingHeader("light", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::DragFloat3("l.position", glm::value_ptr(m_light.position), 0.01f);
      ImGui::ColorEdit3("l.ambient", glm::value_ptr(m_light.ambient));
      ImGui::ColorEdit3("l.diffuse", glm::value_ptr(m_light.diffuse));
      ImGui::ColorEdit3("l.specular", glm::value_ptr(m_light.specular));
    }
    // material-lighting
    if (ImGui::CollapsingHeader("material", ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::DragFloat("m.shininess", &m_material.shininess, 1.0f, 1.0f, 256.0f);
    }
    // animation
    ImGui::Checkbox("animation", &m_animation);
  }
  ImGui::End();

  std::vector<glm::vec3> cubePositions = {
    glm::vec3( 0.0f, 0.0f, 0.0f),
    glm::vec3( 2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f, 2.0f, -2.5f),
    glm::vec3( 1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f),
  };

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  // 기존의 바라보는 방향인 (0, 0, -1)을 Pitch, Yaw만큼 각각의 축 따라 회전
  m_cameraFront =
    glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
    glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraPitch),
      glm::vec3(1.0f, 0.0f, 0.0f)) *
    glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

  auto projection = glm::perspective(glm::radians(45.0f),
    (float)m_width / (float)m_height, 0.01f, 40.0f);
  auto view = glm::lookAt(
    m_cameraPos,
    m_cameraPos + m_cameraFront,
    m_cameraUp);

  // light box
  auto lightModelTransform =
    glm::translate(glm::mat4(1.0), m_light.position) *
    glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
  m_simpleProgram->Use();
  m_simpleProgram->SetUniform("color", glm::vec4(m_light.ambient + m_light.diffuse, 1.0f));
  m_simpleProgram->SetUniform("transform", projection * view * lightModelTransform);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  
  m_program->Use();
  m_program->SetUniform("viewPos", m_cameraPos);
  m_program->SetUniform("light.position", m_light.position);
  m_program->SetUniform("light.ambient", m_light.ambient);
  m_program->SetUniform("light.diffuse", m_light.diffuse);
  m_program->SetUniform("light.specular", m_light.specular);
  m_program->SetUniform("material.diffuse", 0);
  m_program->SetUniform("material.specular", 1);
  m_program->SetUniform("material.shininess", m_material.shininess);

  glActiveTexture(GL_TEXTURE0);
  m_material.diffuse->Bind();
  glActiveTexture(GL_TEXTURE1);
  m_material.specular->Bind();


  for (size_t i = 0; i < cubePositions.size(); i++){
    auto& pos = cubePositions[i];
    auto model = glm::translate(glm::mat4(1.0f), pos);
    model = glm::rotate(model,
      glm::radians((m_animation ? (float)glfwGetTime() : 0.0f) * 120.0f + 20.0f * (float)i),
      glm::vec3(1.0f, 0.5f, 0.0f));
    auto transform = projection * view * model;
    m_program->SetUniform("transform", transform);
    m_program->SetUniform("modelTransform", model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }
}