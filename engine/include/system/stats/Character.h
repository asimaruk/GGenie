#ifndef SYSTEM_STATS_CHARACTER_H
#define SYSTEM_STATS_CHARACTER_H

#include "math/algebras.h"

namespace GGenie {

struct Character {
  unsigned int textureID; // ID handle of the glyph texture
  Vec2 size;              // Size of glyph
  Vec2 bearing;           // Offset from baseline to left/top of glyph
  unsigned int advance;   // Offset to advance to next glyph
};

} // namespace GGenie

#endif
