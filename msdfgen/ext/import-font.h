
#pragma once

#include <cstdlib>
#include <vector>
#include "../core/Shape.h"

namespace msdfgen {

class FreetypeHandle;
class FontHandle;

/// Initializes the FreeType library
FreetypeHandle * initializeFreetype();
/// Deinitializes the FreeType library
void deinitializeFreetype(FreetypeHandle *library);
/// Loads a font file and returns its handle
FontHandle * loadFont(FreetypeHandle *library, const char *filename);
FontHandle * loadFontMemory(FreetypeHandle *library, std::vector<unsigned char> &&data);
/// Unloads a font file
void destroyFont(FontHandle *font);
/// Returns the size of one EM in the font's coordinate system
bool getFontScale(double &output, FontHandle *font);
/// Returns the width of space and tab
bool getFontWhitespaceWidth(double &spaceAdvance, double &tabAdvance, FontHandle *font);
/// Loads the shape prototype of a glyph from font file
bool loadGlyph(Shape &output, FontHandle *font, int unicode, double *advance = NULL);
/// Loads the shape prototype of a glyph from font file by glyph index
bool loadGlyphByIndex(Shape &output, FontHandle *font, int index, double *advance = NULL);
/// Returns the kerning distance adjustment between two specific glyphs.
bool getKerning(double &output, FontHandle *font, int unicode1, int unicode2);

}
