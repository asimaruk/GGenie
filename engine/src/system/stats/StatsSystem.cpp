#include "system/stats/StatsSystem.h"
#include "config/Config.h"
#include "ecs/System.h"
#include "glad.h"
#include "graphics/shader.h"
#include "math/algebras.h"
#include "system/stats/Character.h"
#include "window/EngineWindow.h"
#include <filesystem>
#include <format>
#include <freetype/freetype.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <iostream>
#include <map>
#include <numeric>
#include <print>
#include <span>
#include <string>
#include <string_view>

using namespace std::string_literals;

// https://learnopengl.com/In-Practice/Text-Rendering
class StatsSystem::Impl {
public:
  Impl(const Config &config, const EngineWindow &window) : config(config), window(window) {}

  int init() {
    if (FT_Init_FreeType(&ft)) {
      std::println(std::cerr, "FREETYPE: Could not init FreeType Library");
      return -1;
    }
    auto fontPath = std::filesystem::path(config.resPath) / "fonts/Michroma-Regular.ttf";
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
      std::println(std::cerr, "FREETYPE: Failed to load font");
      return -1;
    }

    FT_Set_Pixel_Sizes(face, 0, LINE_HEIGHT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    createCharacters();

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    compileShader();
    setupBuffers();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
  }

  void update(float dt) {
    updateFPS(dt);

    bindBuffers();
    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(window.getWidth()), 0.0f, static_cast<float>(window.getHeight()));
    glUseProgram(shaderProgram);
    glUniform3f(textColorUniformLoc, textColor.x, textColor.y, textColor.z);
    glUniformMatrix4fv(projectionUniformLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    std::string lines[]{std::format("FPS: {}", getFPS())};
    renderStats(lines, PADDING, PADDING);
    unbindBuffers();
  }

  int getFPS() {
    return frames.size();
  }

private:
  static constexpr auto LINE_HEIGHT = 32;
  static constexpr auto PADDING = 25.f;

  FT_Library ft;
  FT_Face face;
  const Config &config;
  const EngineWindow &window;
  std::map<char, Character> characters;
  unsigned int shaderProgram;
  unsigned int textColorUniformLoc;
  unsigned int projectionUniformLoc;
  unsigned int VAO{}, VBO{};
  const Vec3 textColor = Vec3(1.0f, 1.0f, 1.0f);
  std::vector<float> frames;

  void createCharacters() {
    for (unsigned char c = 0; c < 128; c++) {
      // load character glyph
      if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
        std::println(std::cerr, "FREETYTPE: Failed to load Glyph '{}'", c);
        continue;
      }
      // generate texture
      unsigned int texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(
          GL_TEXTURE_2D,
          0,
          GL_RED,
          face->glyph->bitmap.width,
          face->glyph->bitmap.rows,
          0,
          GL_RED,
          GL_UNSIGNED_BYTE,
          face->glyph->bitmap.buffer
      );
      // set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // now store character for later use
      Character character = {
          texture,
          Vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
          Vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
          static_cast<unsigned int>(face->glyph->advance.x)
      };
      characters.insert(std::pair<char, Character>(c, character));
    }
  }

  void compileShader() {
    shaderProgram = graphics::compileShader(
        R"(
        #version 330 core
        layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
        out vec2 TexCoords;

        uniform mat4 projection;

        void main()
        {
            gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
            TexCoords = vertex.zw;
        }
      )",
        R"(
        #version 330 core
        in vec2 TexCoords;
        out vec4 color;

        uniform sampler2D text;
        uniform vec3 textColor;

        void main()
        {    
            vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
            color = vec4(textColor, 1.0) * sampled;
        }
      )"
    );
    textColorUniformLoc = glGetUniformLocation(shaderProgram, "textColor");
    projectionUniformLoc = glGetUniformLocation(shaderProgram, "projection");
  }

  void setupBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
  }

  void bindBuffers() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  }

  void unbindBuffers() {
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void renderStats(std::span<const std::string> lines, float x, float y) {
    if (lines.size() == 0) {
      return;
    }

    const float startY = y + (lines.size() - 1) * LINE_HEIGHT;
    for (size_t i = 0; i < lines.size(); ++i) {
      renderStatLine(lines[i], x, startY - i * LINE_HEIGHT * 1.05f);
    }
  }

  void renderStatLine(std::string_view text, float x, float y) {
    for (auto c = text.begin(); c != text.end(); c++) {
      Character ch = characters[*c];

      float xpos = x + ch.bearing.x;
      float ypos = y - (ch.size.y - ch.bearing.y);

      float w = ch.size.x;
      float h = ch.size.y;
      // update VBO for each character
      float vertices[6][4] = {
          {xpos, ypos + h, 0.0f, 0.0f},
          {xpos, ypos, 0.0f, 1.0f},
          {xpos + w, ypos, 1.0f, 1.0f},

          {xpos, ypos + h, 0.0f, 0.0f},
          {xpos + w, ypos, 1.0f, 1.0f},
          {xpos + w, ypos + h, 1.0f, 0.0f}
      };
      // render glyph texture over quad
      glBindTexture(GL_TEXTURE_2D, ch.textureID);
      // update content of VBO memory
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      // render quad
      glDrawArrays(GL_TRIANGLES, 0, 6);
      // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
      x += (ch.advance >> 6); // bitshift by 6 to get value in pixels (2^6 = 64)
    }
  }

  void updateFPS(float dt) {
    frames.push_back(dt);
    float sum = std::accumulate(frames.begin(), frames.end(), 0.0f);
    auto secondExtra = sum - 1.0f;
    if (secondExtra <= 0) {
      return;
    }

    int oldCount = 0;
    float oldSum = 0.f;
    for (auto &frame: frames) {
      oldSum += frame;
      ++oldCount;
      if (oldSum >= secondExtra) {
        break;
      }
    }
    frames.erase(frames.begin(), frames.begin() + oldCount);
  }
};

StatsSystem::StatsSystem(SystemID id, int priority, const Config &config, const EngineWindow &window)
    : System(id, priority)
    , pimpl(new Impl(config, window)) {}

StatsSystem::~StatsSystem() = default;

void StatsSystem::start() {
  pimpl->init();
}

void StatsSystem::update(float dt) {
  pimpl->update(dt);
}
