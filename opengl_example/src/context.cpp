#include "context.h"

ContextUPtr Context::Create() {
  auto context = ContextUPtr(new Context());
  if (!context->Init())
    return nullptr;
  return std::move(context);
}

bool Context::Init() {
  
  float vertices[] = {
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top right, red
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right, green
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left, blue
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // top left, yellow
  };
  uint32_t indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3, // second triangle
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

  */

  m_vertexLayout = VertexLayout::Create();
  //vao 생성 후에 buffer를 만들어야 연결 가능
  m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 24);

  m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
  m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, sizeof(float) * 3);

  m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

  // OpenGL 함수 로딩 후에야 shader 불러오기 위한 함수들 사용 가능
  ShaderPtr vertShader = Shader::CreateFromFile("./shader/per_vertex_color.vs", GL_VERTEX_SHADER);
  ShaderPtr fragShader = Shader::CreateFromFile("./shader/per_vertex_color.fs", GL_FRAGMENT_SHADER);
  if (!vertShader || !fragShader)
    return false;
  SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
  SPDLOG_INFO("fragment shader id: {}", fragShader->Get());

  m_program = Program::Create({fragShader, vertShader});
  if (!m_program)
    return false;
  SPDLOG_INFO("prigram id: {}", m_program->Get());

  // // shader에 uniform variable 지정, 값 전달
  // auto loc = glGetUniformLocation(m_program->Get(), "color");
  // m_program->Use();
  // glUniform4f(loc, 1.0f, 1.0f, 0.0f, 1.0f);
  
  // 화면 클리어
  glClearColor(0.0f, 0.1f, 0.2f, 0.3f); // 컬러 프레임버퍼 화면을 클리어 할 색상 지정

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
  glClear(GL_COLOR_BUFFER_BIT); // 프레임 색상 버퍼 클리어

  // // First Animation code
  // static float time = 0.0f;
  // float t = sinf(time) * 0.5f + 0.5f;
  // auto loc = glGetUniformLocation(m_program->Get(), "color");
  // m_program->Use();
  // glUniform4f(loc, t*t, 2.0f*t*(1.0f-t), (1.0f-t)*(1.0f-t), 1.0f);
  // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  // time += 0.016f;

  m_program->Use();
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}