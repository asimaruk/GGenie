#include "graphics/shader.h"
#include "glad.h"
#include <string_view>

using namespace GGenie;

auto graphics::compileShader(std::string_view vertex, std::string_view fragment) -> unsigned int {
  unsigned int const vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char *vertexShaderSource = vertex.data();
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  unsigned int const fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fragmentShaderSource = fragment.data();
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  unsigned int const shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Удаляем шейдеры (они уже в программе)
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}
