#pragma once

#include "math/algebras.h"

struct Character {
  unsigned int textureID; // ID handle of the glyph texture
  Vec2 size;              // Size of glyph
  Vec2 bearing;           // Offset from baseline to left/top of glyph
  unsigned int advance;   // Offset to advance to next glyph
};
