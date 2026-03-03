#include "Config.hpp"
#include "ShaderProgram.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <cstdlib>
#include <glad/glad.h>
#include <iostream>
#include <vector>

inline GLfloat getFixedXFromNDC(GLfloat x) {
  return ((x + 1) * WINDOW_SIZE_PX / 2);
}

inline GLfloat getFixedYFromNDC(GLfloat y) {
  return ((1 - y) * WINDOW_SIZE_PX / 2);
}

inline GLfloat getNDCXFromFixed(GLfloat x) {
  return (x * 2.0f / WINDOW_SIZE_PX) - 1.0f;
}

inline GLfloat getNDCYFromFixed(GLfloat y) {
  return 1.0f - (y * 2.0f / WINDOW_SIZE_PX);
}

class Square {
public:
  bool checked;
  const GLfloat x, y;
  inline bool contains(double xpos, double ypos) {
    return (xpos >= this->x && xpos <= (this->x + TILE_SIZE_PX)) &&
           (ypos >= this->y && ypos <= (this->y + TILE_SIZE_PX));
  }
  Square(GLfloat x, GLfloat y)
      : x(getFixedXFromNDC(x)), y(getFixedYFromNDC(y)), checked(false) {}
};

inline bool win(std::array<Square, 9> *squares){
    return (squares->at(0).checked && squares->at(1).checked && squares->at(2).checked) ||
           (squares->at(3).checked && squares->at(4).checked && squares->at(5).checked) ||
           (squares->at(6).checked && squares->at(7).checked && squares->at(8).checked) ||
           (squares->at(0).checked && squares->at(3).checked && squares->at(6).checked) ||
           (squares->at(1).checked && squares->at(4).checked && squares->at(7).checked) ||
           (squares->at(2).checked && squares->at(5).checked && squares->at(8).checked) ||
           (squares->at(0).checked && squares->at(4).checked && squares->at(8).checked) ||
           (squares->at(2).checked && squares->at(3).checked && squares->at(6).checked);
}

class Data {
public:
  bool xTurn;
  VBO *vbo;
  VAO *vao;
  ShaderProgram *shaderProgram;
  std::array<Square, 9> *squares;
  std::vector<GLfloat> *vertices;
  Data(std::array<Square, 9> *squares, std::vector<GLfloat> *vertices, VBO *vbo, VAO *vao, ShaderProgram *shaderProgram,
       bool xTurn)
      : squares(squares), vertices(vertices), vbo(vbo), vao(vao), shaderProgram(shaderProgram), xTurn(xTurn) {}
};

void mouseClickCallback(GLFWwindow *window, int button, int action, int mods) {
  if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
    double cX, cY;
    glfwGetCursorPos(window, &cX, &cY);

    Data *data = (Data *)glfwGetWindowUserPointer(window);
    Square *sqr = nullptr;
    for (int i = 0; i < data->squares->size(); i++) {
      if (data->squares->at(i).contains(cX, cY) && !data->squares->at(i).checked) {
        sqr = &data->squares->at(i);
        data->squares->at(i).checked = true;
      }
    }
    if (sqr == nullptr)
      return;

    if (data->xTurn) {
      data->vertices->push_back(getNDCXFromFixed(sqr->x));
      data->vertices->push_back(getNDCYFromFixed(sqr->y));
      data->vertices->push_back(1.0f);
      data->vertices->push_back(0.0f);
      data->vertices->push_back(0.0f);
      data->vertices->push_back(getNDCXFromFixed(sqr->x + TILE_SIZE_PX));
      data->vertices->push_back(getNDCYFromFixed(sqr->y + TILE_SIZE_PX));
      data->vertices->push_back(1.0f);
      data->vertices->push_back(0.0f);
      data->vertices->push_back(0.0f);
      data->vertices->push_back(getNDCXFromFixed(sqr->x + TILE_SIZE_PX));
      data->vertices->push_back(getNDCYFromFixed(sqr->y));
      data->vertices->push_back(1.0f);
      data->vertices->push_back(0.0f);
      data->vertices->push_back(0.0f);
      data->vertices->push_back(getNDCXFromFixed(sqr->x));
      data->vertices->push_back(getNDCYFromFixed(sqr->y + TILE_SIZE_PX));
      data->vertices->push_back(1.0f);
      data->vertices->push_back(0.0f);
      data->vertices->push_back(0.0f);
    }
    else{
        data->vertices->push_back(getNDCXFromFixed(sqr->x));
      data->vertices->push_back(getNDCYFromFixed(sqr->y));
      data->vertices->push_back(0.0f);
      data->vertices->push_back(0.0f);
      data->vertices->push_back(1.0f);
      data->vertices->push_back(getNDCXFromFixed(sqr->x + TILE_SIZE_PX));
      data->vertices->push_back(getNDCYFromFixed(sqr->y + TILE_SIZE_PX));
      data->vertices->push_back(0.0f);
      data->vertices->push_back(0.0f);
      data->vertices->push_back(1.0f);
      data->vertices->push_back(getNDCXFromFixed(sqr->x + TILE_SIZE_PX));
      data->vertices->push_back(getNDCYFromFixed(sqr->y));
      data->vertices->push_back(0.0f);
      data->vertices->push_back(0.0f);
      data->vertices->push_back(1.0f);
      data->vertices->push_back(getNDCXFromFixed(sqr->x));
      data->vertices->push_back(getNDCYFromFixed(sqr->y + TILE_SIZE_PX));
      data->vertices->push_back(0.0f);
      data->vertices->push_back(0.0f);
      data->vertices->push_back(1.0f);
    }
    data->vbo->Bind();
    glBufferData(GL_ARRAY_BUFFER, data->vertices->size() * sizeof(GLfloat),
                 data->vertices->data(), GL_STATIC_DRAW);

    
     if (win(data->squares)){
      std::cout << (data->xTurn ? "Red" : "Blue") << " won!" << '\n';
      data->shaderProgram->Delete();
      data->vao->Delete();
      data->vbo->Delete();
      glfwDestroyWindow(window);
      glfwTerminate();
      exit(EXIT_SUCCESS);
     }  
     else{
      data->xTurn = !data->xTurn;
     }
  }
}

int main() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(WINDOW_SIZE_PX, WINDOW_SIZE_PX,
                                        "Tic-Tac-Toe", NULL, NULL);
  if (window == NULL) {
    std::cerr << "Error making window !" << '\n';
    glfwTerminate();
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);

  gladLoadGL();
  glViewport(0, 0, WINDOW_SIZE_PX, WINDOW_SIZE_PX);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  ShaderProgram shaderProgram(
      "/home/azilcoff/Dev/VS "
      "code/C++/OpenGL/Tic-Tac-Toe/shaders/vertexShader.vert",
      "/home/azilcoff/Dev/VS "
      "code/C++/OpenGL/Tic-Tac-Toe/shaders/fragmentShader.frag");

  // SET TIC-TAC-TOE BASE LINES
  std::vector<GLfloat> vertices{
      // HORIZONTAL LINES
      -0.3f,
      -0.8f,
      1.0f,
      1.0f,
      1.0f,
      -0.3f,
      0.8f,
      1.0f,
      1.0f,
      1.0f,
      0.3f,
      -0.8f,
      1.0f,
      1.0f,
      1.0f,
      0.3f,
      0.8f,
      1.0f,
      1.0f,
      1.0f,
      // VERTICAL LINES
      -0.8f,
      -0.3f,
      1.0f,
      1.0f,
      1.0f,
      0.8f,
      -0.3f,
      1.0f,
      1.0f,
      1.0f,
      -0.8f,
      0.3f,
      1.0f,
      1.0f,
      1.0f,
      0.8f,
      0.3f,
      1.0f,
      1.0f,
      1.0f,
  };
  // OBJECTS & SETTING UP LOGIC
  std::array<Square, 9> squares = {
      Square(-0.8f, 0.8f),  Square(-0.3f, 0.8f),  Square(0.3f, 0.8f),
      Square(-0.8f, 0.3f),  Square(-0.3f, 0.3f),  Square(0.3f, 0.3f),
      Square(-0.8f, -0.3f), Square(-0.3f, -0.3f), Square(0.3f, -0.3f),
  };

  bool xTurn = true;
  //

  VAO vao;
  VBO vbo(vertices.data(), vertices.size() * sizeof(GLfloat), GL_STATIC_DRAW);

  vao.LinkVBO(vbo, 0, 2, GL_FLOAT, 5 * sizeof(GLfloat), (void *)0);
  vao.LinkVBO(vbo, 1, 3, GL_FLOAT, 5 * sizeof(GLfloat),
              (void *)(2 * sizeof(GLfloat)));

  vao.UnBind();
  vbo.UnBind();

  Data data(&squares, &vertices, &vbo, &vao, &shaderProgram, xTurn);
  glfwSetWindowUserPointer(window, &data);
  glfwSetMouseButtonCallback(window, mouseClickCallback);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    vao.Bind();
    shaderProgram.Activate();
    glLineWidth(LINE_WIDTH_PX);
    glDrawArrays(GL_LINES, 0, vertices.size() / 2);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  shaderProgram.Delete();
  vao.Delete();
  vbo.Delete();
  glfwDestroyWindow(window);
  glfwTerminate();
  return EXIT_SUCCESS;
}
