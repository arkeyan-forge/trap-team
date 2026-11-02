# Skylanders: Trap Team (trapteam)

> **Purpose:** which disc Trap Team needs, where the port stands, and where to start · **Status:** current (2026-10-30) · **Tags:** [Xbox] [XDK] [Alchemy] [TrapTeam] [V]

## Disc
| | |
|---|---|
| Release | 2014; Redump "Skylanders - Trap Team (Europe) (En,De)", 8738846720 bytes (the only disc tried) |
| Title ID, media ID, version | `41560911`, `7A74A451`, `0x00000014` [V] (read from the XEX execution info) |
| `default.xex` SHA-256 | `f1d2373e...` (full: `f1d2373e4507e21cc371e6f22d02e909602a1b5bc4704896c3c099cc64002a35`), 9252864 bytes; the image is 0x00980000 bytes at `0x82000000`, entry `0x8258A820`, `.text` `0x821CCA00` + `0x615EE0` |
| Engine | **Alchemy** (Giants' hooks 35 of 38, SSA's Lux hooks none), XDK **2.0.21256** (Swap Force 21250, Giants 21076) [V] |

The disc stays outside git; everything derived from it is in `private/trapteam/` (day 1: `private/trapteam/day1/`).

## Status
Two discs: Europe (En,De) and Europe (Fr,Nl), the same loaded image byte for byte; only the XEX header differs (media id, version) [V].
Day 1 (2026-10-30): codegen, build and boot work; the title ("Press A to begin"), the autosave notice, the opening movie and **the first level
(`level_313_tutorial`)** render and run for the 130 s tried (its intro stream opens, about 850 draws at 30 fps) after the vertex-buffer fix
([boot.md](boot.md): the video pool was full, `enableVramCopy` is forced off, commit `24060f0`: a workaround **[?]**, why the console has the room is unproven). The main menu **was seen once** (a slow run, 2026-10-30: "CHOOSE PLAY", Story 0/100, fully drawn, before the shader fix below); in the faster runs the presses went
from the title to the movie `fmv1` and the tutorial, which the game starts on a fresh profile.
**With a Trap Master on the emulated portal (`SKY_PORTAL_PLACE=new:450`) the game goes past "Place a Skylander" and the tutorial is playable**: Gusto, the HUD, 30 fps,
movement and a jump seen over 190 s, no fault ([boot.md](boot.md) "Past the prompt").
**`SKY_LEVEL` works** (2026-10-30): the adapter's `firstLevel` is `\level\level_313_tutorial` (the three-line change in `adapter.cpp`, as Giants' tutorial): the game's first
level's two files are read from the level asked for. One tour on three levels (`tools/regress/coverage.sh trapteam <disc> private/runs/tt-tour --levels
level_300_birdcage,level_301_the_machine,level_302_chompywurm --no-credits`): the three `coverage.tsv` lines are `ok`, each opened, 0 faults, lowest fps 11.0, 30.0 and 29.0 [V].
**The first trap is not reached by play, but a crafted trap works** (2026-10-31: [figures.md](https://github.com/arkeyan-forge/forge/blob/main/docs/runtime/host/figures.md#acceptance-in-the-game-2026-10-31-v); blind scripted play, five runs; [boot.md](boot.md) "The first trap"): the game rejects Gusto plus ten traps ("too many Traps"), accepts one, and no villain met in 600 s of the tutorial or 480 s of `level_300_birdcage`. A later crafted villain trap works; a real capture remains unproved (`d448b32`).
Numbers and how to repeat them: [research.md](research.md) "Day 1".

### Arenas in the tour (2026-10-31) [V]
**Verdict: the faults of `arena_challenge_309_000`, `_311_000` and `_361_000` are artefacts of the `SKY_LEVEL` redirect, not
bugs a player hits**, as Giants' challenge levels ([history.md](https://github.com/arkeyan-forge/giants/blob/main/docs/history.md), "Challenge levels in the tour").
- The fault: `sub_8264B3F0` (`lwz r11,140(r3)`) reads `0x8C` of the global at `0x8284EB50`, null: the Havok world (the
  level's `tfbPhysicsWorld`). Called from `sub_8229C920` <- `sub_822AC078` <- `sub_822ABC90` <- `sub_822434E8`, under 0.4 s
  after the arena's `.bld` opens (reproduced on `_361_000`: `.bld` at 28.9 s, last frame 29.3 s, `r3` 0). The world is made when the level's physics object is activated (`sub_822486C0` ->
  `sub_8229F808` -> `sub_8229EC98` stores it) and released by `sub_8229FA88`.
- On the disc, `tfbPhysicsWorld` (with its `hkpWorldCinfo`) is in each of the six `level\Arena_Mesh_{300,302,303,309,311,361}.bld`
  and in none of the `Arena_Challenge_*_000.bld`. The redirect swaps the tutorial's two files for the arena's only: the
  log opens no `Arena_Mesh`, so the world is never made. 300, 302 and 303 have no world either; they did not reach that
  call in the 130 s.
- The game's own path loads the mesh: `permanent\global.bld` has `Arena_Mesh_BaseString` ("base mesh load string") and
  `misc\UI_ArenaSelect.bld` its "mesh load" with `Arena Challenge Def` and `System - Set Game Mode` [V]; which mesh goes
  with which arena is decided there [?]. A run through the arena select menu is not done.
- The coverage report (`tools/regress/coverage.sh`) keeps the verdict `crash` and prefixes such a row with
  "SKY_LEVEL artefact, not a player bug".

**The Toy Box is complete with the game's own pictures (2026-10-30, lane L):** logo, background and 441 pictures (toys, traps, villains) read from the disc ([art.md](art.md)), the catalogue from the game's own tables (46 villains, 61 trap shapes), 5 seats with a Trap seat, a trap's sheet listing its element's villains, Add a figure by category ([toybox.md](toybox.md)). A crafted villain trap works (`d448b32`); a real capture remains unproved.

**The window was black in the owner's run, the logo movies at least (2026-10-30, lane H):** the cause found [V] is the shader cache: it lacked Bink's two movie
shaders (the day-1 dump skipped the movies), so every movie draw was skipped and the presented texture had 0 lit pixels during the movies, offscreen as well
(`SKY_PRESENT_PROBE`). With the two added (880 shaders, [shaders.md](shaders.md)) the movies draw and a windowed run shows the picture. Not the cause, by forced
runs: the gamma ramp, the front buffer's format. **Not reproduced:** a window black *after* the movies (my windowed runs showed the title); if the owner's
build lacks the movie shaders, rebuild its cache as in shaders.md. Offscreen frames never proved a window ([render_bugs.md](https://github.com/arkeyan-forge/forge/blob/main/docs/runtime/gpu/render_bugs.md)).

**The portal change is merged** (`85ccfc2`). Commit `8347ed7` (`EmulatedPortal::SetStatusInterval`, 8 ms, `runtime/host/portal*`) makes Trap Team boot; it touches every game's emulated portal.

**Day-1 blockers resolved:** `TexCoord 8` has a location (`046227a`), and the IGA v11 reader was added (`a9cd75b`).

**Lead for the pool question (log only, nothing changed; 2026-10-30 [V] for the values, [?] for what a console gives):** the 160 MB block of the video pool
might be sized from a free-memory query. `SKY_ALLOC_LOG=1` over a run to the first level, for every memory query the game imports
([boot.md](boot.md) "Memory queries"): **it calls no free-memory query**: `MmQueryStatistics` is not in its import table, `NtQueryVirtualMemory` is
imported but never called. So the block does not follow a value we report; its size comes from the game ([boot.md](boot.md)).

## The picture: scale, anisotropy, ultrawide (2026-10-30, lane J) [V]
Same tutorial frame (`level_313_tutorial`, Gusto idle, `SKY_DUMP_FRAME` at 150-160 s, offscreen), captures under `private/runs/` (`s1b`, `s2b`, `s2k`, `s2n`, `a16`, `bc`, `uw8`).
- **Scale 2 applies as in the other Alchemy games.** The scene chain is 1040x620 (half 520x310, quarter 260x155, reductions 104x62, 520x620, 104x620; the
  log's "3D renders at 1120x704" is the generic line) and every resolve is x2 (2080x1240, 1040x620, 520x310...), the frame 2560x1440.
  **Colour tables are render targets here too:** the 64x2 (4 resolves a frame) and the 256x16 (1) stay at their guest size (`IsTableSize`).
  Proof: with the rule off (binary `s2n`) the green histogram of the upper frame piles one bin to **16.1 x** its neighbours (5 bins above 3 x), the lava's
  posterization of Swap Force; with the rule 1.16 (`s2b`) and 1.21 (`s2k`), scale 1 gives 1.22 (`analyze.py banding`'s measure).
- **FXAA is in the tone-map pass:** pixel shader `6BD91E6572C6581C` (vertex `A6F60BE72C1BA743`), one draw into a 1040x620 target that reads 16 textures
  including the 64x2 ramps; c0 = +-0.5 / size, c1 = (8, 8, -4, -4) / size, sharpness 8, threshold 0.125, minimum 0.05: FXAA 3.11 as Swap Force's, but not a pass of its own.
  `game::HostKernelPixelShader` is that hash (`games/trapteam/addresses.h`): its kernel stays in guest pixels at scale N. Four static ground regions at scale 2,
  Laplacian before and after: 3.62 -> 4.13, 3.02 -> 3.28, 5.78 -> 6.70, 5.31 -> 6.16 (+9 to +16 %; two runs of an idle scene, so [?] to a point: clouds and goo move).
- **Tiles:** at the console's 1040x620 there is no split, at either scale, in the two levels tried (every resolve is a full rectangle, `rect 0 0 w h`; tutorial at scale 1 and 2, `level_300_birdcage` at 2);
  tiling is the guest's and the host scale does not change it. **At the frame's 1280x720 (the default since 2026-10-31, below) the game splits the G-buffer into 2 tiles** for the console's EDRAM
  (`[scene] 1280x720 targets kept in one piece (the game split them into 2 tiles ...)`): `Scene_Tiles` (`0x823EBCD0`) and `Scene_SurfaceTiles` (`0x823EF930`) are hooked as Swap Force's (same offsets
  +32, +36, +40, +44, +88, +108, +112, +132, byte +125; the plan is `sub_82443DF8`, return `0x82443E44`, targets are set by `sub_823EC8E0`).
- **Cliff textures:** soft at both scales, not blocky or posterized; at scale 2 more of their strata show. The softness is the texture's own resolution [?] (no console frame to compare).
- **Anisotropy reaches the samplers:** scale 2, `--anisotropy 16` against 1 (`a16`, `s2k`): the same static ground regions' Laplacian 4.13 -> 4.93, 3.28 -> 4.79, 6.70 -> 7.85, 6.16 -> 7.38
  (+17 to +46 %); the far grass keeps its blades instead of smearing. Only textures with mipmaps get it (`SamplerFor`); the guest never asks.
- **Ultrawide works** (`games/trapteam/ultrawide.cpp`, `SKY_WINDOW_SIZE=2560x1080`, scale 1; `uw8` with `original_resolution`: scene 1232x512, the 1040x620's pixel count at the window's shape; `uw9` with the default full scene: **1696x720**, 47 resolves, 1 tile, no fault), frame 1707x720,
  the HUD (Gusto's portrait and bars) in the centred 16:9 area, more world on both sides, round things round, the camera where it is at 16:9. Hooks: `sub_8243F650` (the pass size,
  result at +92/+96) and the projection builder `sub_823B1AF0` (a horizontal field of view in radians and an aspect: 84.3 -> 100.7 degrees, the vertical one stays). The same camera is built by two
  callers: `sub_8243C700` (the scene target's shape, already wide) and `sub_8230F3D8` (a global 16:9 at `0x82826308`, two calls; their aspect is replaced in the hook).
  **Do not put the window's aspect in that global:** the camera rig reads it and with 2.37 it pulled the camera back and away from Gusto (`uw6`, `uw7`) [V], why is [?].
  Limits: the HUD's edge shading stays at the 16:9 borders (a step in the sky, as Giants); not tried: scale 2 at 21:9, 32:9, the title and menus, movies (the Bink hooks exist), other levels.

## The constant blur (2026-10-31, lane J) [V]
The owner: the whole picture is soft, all the time. Same tutorial frame, scale 2 (the 3D renders at 2080x1240, the frame is 2560x1440, the window 2560x1504 on a Retina screen), Laplacian of the
same static regions (ground, path, near grass, far ground, cliff: 5 regions) of the presented frame; offscreen dumps and **real window captures** (`screencapture -l <window id>`, `SDL_MAC_BACKGROUND_APP=1`, `win0`, `win1`).
Causes, biggest first:
1. **The scene is drawn at 1040x620 and stretched to 1280x720** (x1.23, x1.16, bilinear), then x2 at scale 2. Fixed by rendering the scene at the frame's size, as Swap Force does (`fullScene`
   = 1280x720, `games/trapteam/ultrawide.cpp`, commit `aaf5c85`): **+52 to +70 % detail** in the five regions (offscreen, `os1` against `fs1`), **+53 to +65 %** in the real window (`win0` against `win1`).
   Same frame structure as before (47 resolves, about 1410 draws, 30 fps, no halo, no fault); `--original-resolution 1` keeps the console's 1040x620. About 43 % more scene pixels.
2. **FXAA's kernel was in guest pixels at scale 2** (`HostKernelPixelShader` is 0 on the release branch, the real hash is in `f5a59cb`): the same frame with the kernel in host pixels, **+5 to +17 %**, one run (three frames 6 s apart, idle scene).
3. **FXAA itself (the game's look on a console), still on, now a setting** ("Edge smoothing", `edge_smoothing`, [settings.md](https://github.com/arkeyan-forge/forge/blob/main/docs/getting-started/settings.md)): one run, four consecutive frames with the pass's constants changed per frame
   (a temporary patch), full scene, scale 2. Detail against the game's setting (threshold 0.125, minimum 0.05): threshold 0.25 / 0.1 **+0 to +17 %**, 0.5 / 0.2 **+1 to +34 %**, FXAA off **+1 to +41 %**
   (near grass +41 %, far ground +34 %, cliff +30 %, ground +22 %, path +1 %). At scale 2 the silhouettes show little stair-stepping without it (the thin rope edge a little), at scale 1 more.
   **The setting (2026-10-31):** Game (the game's constants), Light (0.5 / 0.2), Off (1000, a plain copy); the launcher and the F3 menu call them Game, Gentle, None. Default (not chosen): Light from resolution scale 2, Game at 1;
   a choice stays. `analyze.py sharpness` (the 3D area's detail against Game), one frame each: **Light +30.3 %, Off +34.3 %**; in a real window (`edge-win-*`, five regions) **Light +1 to +37 %, Off +1 to +42 %**.
Not the cause: **the present** (`gpu/gpu.cpp` `Letterbox`: bilinear, exactly 1:1 when the window is as large as the frame, 2560x1440 in 2560x1440 of content: the window capture's numbers equal the offscreen's within 2 %);
**depth of field and bloom** (the near-field ground is bit for bit the same Laplacian from the lit scene's resolve to the one before the tone map, 16.43 at file 273 to 286; the far islands' blur is the game's depth of field);
**half-size targets** (every non-table target of the frame is x2: 2560x1440, 1280x720, 640x360, 256x144, 1280x1440, 512x512, 1024x4096; the 64x2 and 256x16 tables stay guest-sized).
Not measured: Swap Force at the same scale (no Swap Force disc in these folders); the tone-map curve's own contrast (the Laplacian also falls with contrast).

## Where to start
- Build: `cmake -S . -B build-trapteam -G Ninja -DCMAKE_BUILD_TYPE=Release -DSSA_GAME=trapteam`, then
  `nice ninja -j4 -C build-trapteam SSARecomp` (the executable is `TTRecomp`). `private/trapteam/ppc/` is made by
  XenonRecomp from `config/` ([day1.md](https://github.com/arkeyan-forge/forge/blob/main/docs/porting/day1.md) step 2) and `shader_cache.cpp` by
  `SKY_GAME=trapteam tools/shaders/build_cache.sh <disc> <dump dir>`.
- The shaders come from run-time dumps (`SKY_DUMP_SHADERS`): the disc's archives are IGA **v11**, which no extractor reads yet.
- Run it isolated ([testing.md](https://github.com/arkeyan-forge/forge/blob/main/docs/getting-started/testing.md)) with `SKY_SKIP_MOVIES=1 SKY_PORTAL=emulated`; the title
  is up at about 15-40 s.

## Open problems
0. **LB and RB** (tutorial, Gusto, 2026-10-31 [V]): X, B and RB each start an attack, Y (held 1 s) and LB (held 1 s) nothing; RB looks like B's dash. Y's silence may be the tutorial's (no Attack 3 yet): LB is not proved a Y alias, so the launcher has an RB row and no LB row.
1. The video pool has too little room for the vertex-buffer copy (worked around **[?]**: why the console has the room is unproven) [boot.md](boot.md).
2. **36 of 225 vertex shaders are not translated** (`TexCoord 8`, a usage XenosRecomp has no location for).
3. The 20 s wait on the legal screen in some runs (portal `V`, `L`, `C` unanswered?) is unexplained.
4. No portal prompt, no voice language apart from the text's (the voices follow `language`: [boot.md](boot.md#language)), no save-task hook. Glyphs: the pause menu's hints and the title's "Press A" follow the device ([glyphs.md](glyphs.md)); in-sentence prompts not seen yet. Ultrawide and the full scene are in the adapter.
