# Trap Team's button glyphs

> **Purpose:** which pictures show a button in Trap Team, how they are found on the disc, the cells the adapter redraws, and what still shows the 360's picture · **Status:** current (2026-10-30) · **Tags:** [Xbox] [Alchemy] [TrapTeam] [V] [?]

## What the game has
| Picture | Where | Status |
|---|---|---|
| **Colour atlas** `fonts\360\font_001_rgb.png`, 512x256, 16 bits a pixel (format hash `1CB93819`, tiled, with mips) | `permanent\bootstrap.bld`; guest hash (XXH3 of the 262,144-byte base level) `67DFCE1E73302C9F` | **redrawn** [V]: A X Y B, LT RT, LS RS, Start, Back (cells in `games/trapteam/adapter.cpp`); the pause menu's "Accept" / "Back" hints follow the device |
| The same atlas: four arrows, an envelope, the Start and Back pills' inner arrows | | stay the 360's (no `Glyph` kind for them) |
| **The title's A**, a whole picture, 64x64 DXT5, stored upside down, guest hash `A3E90BC0BE57F49A` | `level\title.bld` (the title's text is three strings, "Press", the picture, "to begin") | **redrawn** [V] (`GlyphTextures`, as Giants' `a_button`); the first guess, that it came from the distance-field atlas, was wrong |
| **Distance-field atlas** `fonts\360\font_001.png`, 1024x1024 (`1CB93819`), guest hash `2949820980389064` | `permanent\bootstrap.bld` | the letters (blue channel) and shapes without letters (arrows, circles, bubbles, pills); not a button picture, left alone |
| Controller diagrams `ui_main\sprites\controls\sky2\controls_{xb1,ps4,ps3,360,wiiu_all,wii}.png`, 955x331 | `level\title.arc`, `misc\ui_options.arc` | not replaced |

## How they were found (about 2 h)
1. **Archives.** IGA v11 is read by `runtime/host/art/alchemy_archive.cpp`; **IGZ v8 was not** (2026-10-30: it needs the tags byte-swapped as v7, the sections at 0x14, and the types' names padded to 2 bytes; no `TSTR`). Fixed, the whole disc lists: 62,727 pictures in 1,916 archives. A texture's name is the archive entry's source path only in an `.arc` (`levels/includes/ui_main/...png`); in a `.bld` the one entry is `level.bld` and its pictures have no name (`fonts/360/font_001*.png` come from the `igImage2` of `bootstrap.bld`).
2. **Guest hash.** XXH3 of the **base level only**, `width * height * bytes a pixel` for a linear size: the hash of the whole storage (mips included) matches nothing. A tutorial run with `SKY_TEXTURE_DUMP=1 SKY_TEXTURE_DUMP_LOG=1` dumps 446 textures; 104 hashes are on the disc by the whole-storage hash, 2 more (the two font atlases) by the base level.
3. **Cells.** Measured on the dumped DDS (RGBA8, alpha 255 everywhere in the dump: non-white pixels, 8-neighbour components).
4. `GuestBaseRgba` (`runtime/d3d/render.cpp`) read DXT1 and DXT5 only: the 16-bit formats (1_5_5_5, 5_6_5, 4_4_4_4) are expanded as the renderer uploads them, so the atlas now reads (without it: `[glyphs] font rgb atlas 512x256: its format cannot be read here`). Other games' atlases are DXT: unchanged.

## Where the text uses a button
The English string tables (472 distinct of the 653 `ENGLISH.pak`, UTF-16BE, 2026-10-30) spell a button as **one character** in the text: `]` (271 "Press/Hold ... to"), `{` (232), `}` (213), `[` (8), `<` (9, "Press < to Continue"), `&` (6, "Use & to guide the Lock Master") and `_` (2, "Press _ to Invert Up/Down Controls") [V]: seven codes, the colour atlas's A X Y B, the triggers and the sticks; which code is which button was not read [?], but the context says: `[` "Press [ to jump" (A), `]` "Press ] for Primary attack", `}` "} Show Help" and "Use ] and } to defeat monsters", `<` the title and menus ("< Continue Game", "< New Game", "< Next", "Press < to Start") [V] (title and tutorial string tables, 2026-10-31). No text names a bumper, an arrow or the envelope ("shoulder" is an ability's name; "PRESS D-PAD UP TO BEGIN" is a demo screen's plain words). So the arrows, the envelope and LB/RB are **left as the game's**: no prompt uses them [V] for the text; the in-world prompts were not reached.

## Cells (base level, inclusive)
A 1,1-80,80 · X 88,1-167,80 · Y 175,1-254,80 · B 261,1-340,79 · LT 349,0-426,77 · RS 1,163-84,242 · LS 92,163-174,243 · Start 182,178-258,232 · Back 266,178-342,232 · RT 349,161-425,237. No LB or RB in the atlas, and no text asks for one.

## Captures [V]
Isolated, offscreen, `SKY_PORTAL=emulated SKY_PORTAL_PLACE=new:450 SKY_LEVEL=level_313_tutorial`, A every 4 s to 76 s, Start at 90 s (the pause menu), `SKY_GLYPHS_SCRIPT=ps5:97,switch:107,xboxone:117,ps3:127,xbox360:137`, `SKY_GPU_SNAPSHOT_SECONDS=94,102,112,122,132,142`. Frames (local, not in git): `private/runs/tg5/snap_t<s>.ppm`.

| Frame | Style | The "Accept" / "Back" hints |
|---|---|---|
| `t94` | keyboard (no pad yet) | keycaps |
| `t102` | PlayStation (PS5 pad) | cross, circle |
| `t112` | Nintendo (Switch pad) | letters by position (B, A) |
| `t122` | Xbox One | the Xbox One buttons |
| `t132` | PlayStation 3 | PS3 shapes |
| `t142` | Xbox 360 | the game's own green A and red B |

The log has `[glyphs] font rgb atlas 512x256 drawn as <style>` for each. The five hint frames are darker than the 360's: the menu was fading in at 94-132 s, the 360 frame is 10 s older [?] (not checked at the same age).

## The title [V]
`SKY_GLYPHS_SCRIPT=ps5:34,switch:44,xboxone:56,ps3:68,key:80`, no presses, `SKY_GPU_SNAPSHOT_SECONDS=26,40,50,62,74,86` (local frames `private/runs/tg6/snap_t<s>.ppm`, run through `../gamelock.sh`): "Press (A) to begin" shows, in turn, the keycap (SPACE, 26 s), the PlayStation cross (40 s), the Switch's B (50 s), the Xbox One A (62 s), the PS3 cross (74 s) and the keycap again (86 s), all upright. The log has `title a_button 64 drawn as <style>` each time.

## The launcher's keys [V] (2026-10-31)
`[trapteam.input]` with `key_a=G key_b=H key_x=F key_y=B key_start=Tab key_lb=R key_rb=Y key_lt=U key_rt=O` in `SKY_SETTINGS`: the log says `keycaps: a=G b=H x=F y=B start=Tab back=Bksp`, the difficulty screen's hint reads "G Accept" and the pause's "G Accept, H Back" (keycaps); `SKY_GLYPHS_SCRIPT=ps5:100,key:115`: the cross and circle at 108 s, the keycaps again at 122 s. Frames (local) `private/runs/tgk1/t{26,94,108,122}.png`. The defaults are the runtime's and the launcher's shared Swap Force table. LB, RB, Start and the sticks were not on a screen.

## What still shows the wrong picture
- The four arrows, the envelope (and no LB/RB exists): nothing in the text uses them, see above.
- The controller diagrams (`controls_*.png`, 955x331 photographs of a pad with leader lines; the "Game Paused" screen reached showed only Accept and Back, no Options page). Not replaced: the screen was not reached, so the guest hash of `controls_360` is unverified (the storage hash `044282E6C5B32129` is the disc's, not checked against the renderer's), and the keyboard version needs its own layout (a new `Glyph` kind and drawing path), which Giants does not have either.
- **Not reached:** a prompt in a sentence of the tutorial ("Press X to ...") and the in-world prompts: no scripted play (A every 4 s, then the stick) met one in 190 s of `level_313_tutorial`; Start, Back, the triggers and the sticks were not seen on a screen either (only A and B in the pause menu and the title).
