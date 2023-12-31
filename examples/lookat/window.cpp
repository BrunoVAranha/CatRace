#include "window.hpp"

#include <unordered_map>
#include <random>


// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};

// VARIAVEIS QUE DEFINEM O FOCO
bool focoAzul = false;
bool focoBranco = true;
bool focoAmarelo = false;
bool focoVermelho = false;
////////////

// GERADOR DE NUM ALEATORIO PARA VELOCIDADES
std::random_device rd;
std::default_random_engine generator(rd());
std::uniform_real_distribution<float> distribution(0.02f, 0.08f);
float vel1 = distribution(generator);
float vel2 = distribution(generator);
float vel3 = distribution(generator);
float vel4 = distribution(generator);
//////////////////////////

void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_KEYDOWN) {
    // if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
    //   m_dollySpeed = 1.0f;
    // if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
    //   m_dollySpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    // if (event.key.keysym.sym == SDLK_q)
    //   m_truckSpeed = -1.0f;
    // if (event.key.keysym.sym == SDLK_e)
    //   m_truckSpeed = 1.0f;
  }
  if (event.type == SDL_KEYUP) {
    // if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) &&
    //     m_dollySpeed > 0)
    //   m_dollySpeed = 0.0f;
    // if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) &&
    //     m_dollySpeed < 0)
    //   m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_RIGHT ||
         event.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    // if (event.key.keysym.sym == SDLK_q && m_truckSpeed < 0)
    //   m_truckSpeed = 0.0f;
    // if (event.key.keysym.sym == SDLK_e && m_truckSpeed > 0)
    //   m_truckSpeed = 0.0f;
    if(event.key.keysym.sym == SDLK_SPACE){
      if(focoAzul){
        focoAzul = false;
        focoVermelho = true;
      }
      else if(focoVermelho){
        focoVermelho = false;
        focoBranco = true;
      }
      else if(focoBranco){
        focoBranco = false;
        focoAmarelo = true;
      }
      else if(focoAmarelo){
        focoAmarelo = false;
        focoAzul = true;
      }
    }
  }
}

void Window::onCreate() {

  fimCorrida = false;
  
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  gatoAmarelo.gato_position = glm::vec3(2.2f, 0.0f, 0.0f);
  gatoBranco.gato_position = glm::vec3(3.4f, 0.0f, 0.0f);
  gatoVermelho.gato_position = glm::vec3(4.6f, 0.0f, 0.0f);
  gatoAzul.gato_position = glm::vec3(5.8f, 0.0f, 0.0f);
  
  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "lookat.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "lookat.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  m_ground.create(m_program);

  // Get location of uniform variables
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");

  // Load model
  loadModelFromFile(assetsPath + "catn0.obj");

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_vertices.at(0)) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_indices.at(0)) * m_indices.size(),
                     m_indices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  m_camera.trocarAlturaCamera(3.0f);
}

void Window::loadModelFromFile(std::string_view path) {
  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(path.data())) {
    if (!reader.Error().empty()) {
      throw abcg::RuntimeError(
          fmt::format("Failed to load model {} ({})", path, reader.Error()));
    }
    throw abcg::RuntimeError(fmt::format("Failed to load model {}", path));
  }

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  auto const &attributes{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};

  m_vertices.clear();
  m_indices.clear();

  // A key:value map with key=Vertex and value=index
  std::unordered_map<Vertex, GLuint> hash{};

  // Loop over shapes
  for (auto const &shape : shapes) {
    // Loop over indices
    for (auto const offset : iter::range(shape.mesh.indices.size())) {
      // Access to vertex
      auto const index{shape.mesh.indices.at(offset)};

      // Vertex position
      auto const startIndex{3 * index.vertex_index};
      auto const vx{attributes.vertices.at(startIndex + 0)};
      auto const vy{attributes.vertices.at(startIndex + 1)};
      auto const vz{attributes.vertices.at(startIndex + 2)};

      Vertex const vertex{.position = {vx, vy, vz}};

      // If map doesn't contain this vertex
      if (!hash.contains(vertex)) {
        // Add this index (size of m_vertices)
        hash[vertex] = m_vertices.size();
        // Add this vertex
        m_vertices.push_back(vertex);
      }

      m_indices.push_back(hash[vertex]);m_camera.syncGato(gatoBranco.gato_position);
    }
  }
}

void Window::onPaint() {
  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  abcg::glBindVertexArray(m_VAO);

  // GATO BRANCO
  glm::mat4 model{1.0f};
  if(!fimCorrida) gatoBranco.gato_position.z += vel1;
  model = glm::translate(model, gatoBranco.gato_position);
  model = glm::scale(model, glm::vec3(0.1f));
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);

  focoBranco ? m_camera.syncGato(gatoBranco.gato_position) : void() ;

  // GATO AMARELO
  if(!fimCorrida) gatoAmarelo.gato_position.z += vel2;
  model = glm::mat4(1.0);
  model = glm::translate(model, gatoAmarelo.gato_position);
  model = glm::scale(model, glm::vec3(0.1f));
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 1.0f, 0.8f, 0.0f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);
  focoAmarelo ? m_camera.syncGato(gatoAmarelo.gato_position) : void();
  

  // GATO AZUL
  if(!fimCorrida) gatoAzul.gato_position.z += vel3;     
  model = glm::mat4(1.0);
  model = glm::translate(model, gatoAzul.gato_position);
  model = glm::scale(model, glm::vec3(0.1f));
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 0.0f, 0.8f, 1.0f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);
  focoAzul ? m_camera.syncGato(gatoAzul.gato_position) : void();
  

  // GATO VERMELHO
  if(!fimCorrida) gatoVermelho.gato_position.z += vel4;     
  model = glm::mat4(1.0);
  model = glm::translate(model, gatoVermelho.gato_position);
  model = glm::scale(model, glm::vec3(0.1f));
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 0.8f, 0.0f, 0.0f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);
  focoVermelho ? m_camera.syncGato(gatoVermelho.gato_position) : void();

  abcg::glBindVertexArray(0);

  // Draw ground
  m_ground.paint();

  if(gatoBranco.gato_position.z >= 29.0f ||
      gatoAmarelo.gato_position.z >= 29.0f ||
       gatoAzul.gato_position.z >= 29.0f || 
       gatoVermelho.gato_position.z >= 29.0f){
    fimCorrida =true;
  }

  abcg::glUseProgram(0);
}

void Window::onPaintUI() { abcg::OpenGLWindow::onPaintUI(); }

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  m_ground.destroy();

  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}