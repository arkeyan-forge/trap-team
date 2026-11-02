// Trap Team's adapter: what the runtime asks of the game (runtime/game.h). Every optional member is empty (no portal prompt, no
// voice language apart from the text's, no start-up logo list yet) except the text language (language.cpp) and the first
// level: it presents at every blank like the other
// Alchemy games, and SKY_LEVEL redirects the first level's two files (level\level_313_tutorial.arc and .bld, as Giants'
// tutorial) to the level asked for. Its button glyphs are the cells of one colour atlas (games/trapteam/docs/glyphs.md). fullScene: the 3D scene at the frame's 1280x720 instead of the console's 1040x620 (ultrawide.cpp).
#include "game.h"
#include "trapteam.h"

namespace
{
    // fonts\360\font_001_rgb.png (bootstrap.bld), 512x256, 16 bits a pixel with its mips: the coloured pictures of the text
    // (buttons, arrows, an envelope); the letters are another atlas (font_001.png, a distance field). Cells are the buttons'
    // bounding boxes in the stored rows. The four arrows, the envelope and the Start and Back pills' arrows are not glyphs here.
    using glyphs::Cell;
    using glyphs::Glyph;
    constexpr Cell ButtonCells[] = {
        { Glyph::A, 1, 1, 80, 80 },
        { Glyph::X, 88, 1, 167, 80 },
        { Glyph::Y, 175, 1, 254, 80 },
        { Glyph::B, 261, 1, 340, 79 },
        { Glyph::LeftTrigger, 349, 0, 426, 77 },
        { Glyph::LeftStickIcon, 92, 163, 174, 243 },
        { Glyph::RightStickIcon, 1, 163, 84, 242 },
        { Glyph::Start, 182, 178, 258, 232 },
        { Glyph::Back, 266, 178, 342, 232 },
        { Glyph::RightTrigger, 349, 161, 425, 237 },
    };
    // The title's "Press (A) to begin": a whole green A of title.bld, 64x64 DXT5, stored upside down (as Giants' a_button).
    constexpr glyphs::Texture GlyphTextures[] = {
        { 0x67DFCE1E73302C9Full, "font rgb atlas 512x256", Glyph::A, ButtonCells },
        { 0xA3E90BC0BE57F49Aull, "title a_button 64", Glyph::A, {}, true },
    };

    // presentIntervalPacesGame: at fps_limit 60 the game still held 33.3 ms a frame (cutscene runs, 29.6 fps), as Swap Force's
    // Alchemy does at interval 2. maxFpsLimit 60 until a rate above it is measured (games/trapteam/docs/research.md).
    const game::Adapter g_adapter = { g_gameInfo, nullptr, true, nullptr, GlyphTextures, {}, true,
        trapteam::SetTextLanguage, { "\\level\\level_313_tutorial", std::string_view("\0.", 2) }, {}, true,
        { 1280, 720 }, nullptr, nullptr, 60 };
}

const game::Adapter& game::Current() { return g_adapter; }
