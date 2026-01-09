#include "system/stats/StatsSystem.h"
#include "config/Config.h"
#include "ecs/System.h"
#include "glad.h"
#include "graphics/shader.h"
#include "logging/Logg.h"
#include "math/algebras.h"
#include "system/stats/Character.h"
#include "utils/narrow.h"
#include "window/EngineWindow.h"
#include <array>
#include <cstddef>
#include <filesystem>
#include <format>
#include <freetype/freetype.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <print>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace GGenie;
using namespace std::string_literals;

// https://learnopengl.com/In-Practice/Text-Rendering
class StatsSystem::Impl {
public:
  Impl(std::shared_ptr<Config> config, const EngineWindow &window) : config(config), window(window) {}
  Impl(const Impl &) = delete;
  Impl(Impl &&) = delete;
  ~Impl() = default;

  auto operator=(const Impl &) -> Impl & = delete;
  auto operator=(Impl &&) -> Impl & = delete;

  auto init() -> int {
    if (auto error = FT_Init_FreeType(&ft) != 0) {
      log::Logg::error(std::format("FREETYPE: Could not init FreeType Library, error {}", error));
      return -1;
    }
    auto fontPath = std::filesystem::path(config->resPath) / "fonts/Michroma-Regular.ttf";
    if (auto error = FT_New_Face(ft, fontPath.string().c_str(), 0, &face) != 0) {
      log::Logg::error(std::format("FREETYPE: Failed to load font, error {}", error));
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
        glm::ortho(0.0F, static_cast<float>(window.getWidth()), 0.0F, static_cast<float>(window.getHeight()));
    glUseProgram(shaderProgram);
    glUniform3f(textColorUniformLoc, textColor.x, textColor.y, textColor.z);
    glUniformMatrix4fv(projectionUniformLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    const std::array<const std::string, 1> lines{std::format("FPS: {}", getFPS())};
    renderStats(lines, PADDING, PADDING);
    unbindBuffers();
  }

  auto getFPS() -> unsigned long {
    return frames.size();
  }

private:
  static constexpr auto LINE_HEIGHT = 32;
  static constexpr auto PADDING = 25.F;
  static constexpr auto GLYPH_VERTS_NUMBER = 6;
  static constexpr auto GLYPH_VBO_SIZE = 24;
  static constexpr auto LINE_MULTIPLIER = 1.05F;
  static constexpr auto CHARACTERS_COUNT = 128;
  static constexpr auto MUL_64_SHIFT = 6U;

  FT_Library ft = nullptr;
  FT_Face face = nullptr;
  std::shared_ptr<Config> config;
  const EngineWindow &window;
  std::map<char, Character> characters;
  unsigned int shaderProgram = 0;
  int textColorUniformLoc = 0;
  int projectionUniformLoc = 0;
  unsigned int VAO{}, VBO{};
  const Vec3 textColor = Vec3(1.0F, 1.0F, 1.0F);
  std::vector<float> frames;

  void createCharacters() {
    for (unsigned char charCode = 0; charCode < CHARACTERS_COUNT; charCode++) {
      // load character glyph
      if (FT_Load_Char(face, charCode, FT_LOAD_RENDER) != 0) {
        log::Logg::error(std::format("FREETYTPE: Failed to load Glyph '{}'", charCode));
        continue;
      }
      // generate glyph texture
      unsigned int texture = 0;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(
          GL_TEXTURE_2D,
          0,
          GL_RED,
          narrow<GLsizei>(face->glyph->bitmap.width),
          narrow<GLsizei>(face->glyph->bitmap.rows),
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

      const Character character = {
          .textureID = texture,
          .size = Vec2(narrow<float>(face->glyph->bitmap.width), narrow<float>(face->glyph->bitmap.rows)),
          .bearing = Vec2(narrow<float>(face->glyph->bitmap_left), narrow<float>(face->glyph->bitmap_top)),
          .advance = static_cast<unsigned int>(face->glyph->advance.x)
      };
      characters.insert(std::pair(charCode, character));
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

  void bindBuffers() const {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GLYPH_VERTS_NUMBER * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
  }

  void unbindBuffers() {
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void renderStats(std::span<const std::string> lines, float x, float y) {
    if (lines.empty()) {
      return;
    }

    const float startY = y + narrow<float>((lines.size() - 1) * LINE_HEIGHT);
    for (size_t i = 0; i < lines.size(); ++i) {
      renderStatLine(lines[i], x, startY - (narrow<float>(i * LINE_HEIGHT) * LINE_MULTIPLIER));
    }
  }

  void renderStatLine(std::string_view text, float x, float y) {
    for (const auto symbol : text) {
      const Character measures = characters[symbol];

      const float xpos = x + measures.bearing.x;
      const float ypos = y - (measures.size.y - measures.bearing.y);

      const float width = measures.size.x;
      const float height = measures.size.y;
      // update VBO for each character
      std::array<float, GLYPH_VBO_SIZE> vertices = {
          // clang-format off
        xpos,         ypos + height, 0.0F, 0.0F,
        xpos,         ypos,          0.0F, 1.0F,
        xpos + width, ypos,          1.0F, 1.0F,

        xpos,         ypos + height, 0.0F, 0.0F,
        xpos + width, ypos,          1.0F, 1.0F,
        xpos + width, ypos + height, 1.0F, 0.0F
          // clang-format on
      };
      // render glyph texture over quad
      glBindTexture(GL_TEXTURE_2D, measures.textureID);
      // update content of VBO memory
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data());
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      // render quad
      glDrawArrays(GL_TRIANGLES, 0, GLYPH_VERTS_NUMBER);
      // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
      x += narrow<float>(measures.advance >> MUL_64_SHIFT); // bitshift by 6 to get value in pixels (2^6 = 64)
    }
  }

  void updateFPS(float dt) {
    frames.push_back(dt);
    const float sum = std::accumulate(frames.begin(), frames.end(), 0.0F);
    auto secondExtra = sum - 1.0F;
    if (secondExtra <= 0) {
      return;
    }

    int oldCount = 0;
    float oldSum = 0.F;
    for (auto &frame : frames) {
      oldSum += frame;
      ++oldCount;
      if (oldSum >= secondExtra) {
        break;
      }
    }
    frames.erase(frames.begin(), frames.begin() + oldCount);
  }
};

StatsSystem::StatsSystem(SystemID id, int priority, std::shared_ptr<Config> config, const EngineWindow &window)
    : System(id, priority)
    , pimpl(new Impl(config, window)) {}

StatsSystem::~StatsSystem() = default;

void StatsSystem::start() {
  pimpl->init();
}

void StatsSystem::update(float dt) {
  pimpl->update(dt);
}
