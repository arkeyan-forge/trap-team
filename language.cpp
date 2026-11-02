// Trap Team: the text and voice language (docs: games/trapteam/docs/boot.md, "Language"). The title has one language
// for everything. It picks it in sub_821D29B8 from the console language (XGetLanguage, wrapped by sub_8258A230: 3
// German, 4 French, 16 Dutch...) and from which language files the disc has (3d_font_msg_<language>, the packs), and
// keeps an IG_CORE_LANGUAGE value that Language_Get makes from the same console language (17 cases); each archive
// "<name>.bld" holds one language pack per language, "<LANGUAGE>.pak", with the archive's text and the localized voice
// lines [V]. The host sets both reads, the console language where the console has the language (Dutch is 16; Danish and
// Finnish have none) and the value for every language of the title's list.
#include "game_hooks.h"
#include "trapteam.h"
#include <cstdio>
#include <string>
#include <ppc_config.h>
#include <ppc_context.h>

namespace
{
    struct Language
    {
        std::string_view code;
        uint32_t index; // its IG_CORE_LANGUAGE value; the pack is <NAME>.pak
        uint32_t console; // its XGetLanguage value; 0: the console has no such language
    };

    // The IG_CORE_LANGUAGE values come from Language_Get's own table (en 6, ja 15, de 9, fr 8, es 21, it 14, ko 16, pt
    // 19, pl 18, ru 20, sv 23, no 17, nl 5) and from the order of the enum's names in the executable (sorted by name:
    // ARABIC 0 ... SPANISH 21, SPANISH_MEXICAN 22, SWEDISH 23, TAIWANESE 24, TURKISH 25), which gives Danish 4, Finnish
    // 7, Czech 3, Greek 10 and Hungarian 13 [?]: only English, French, German and Dutch were seen on a disc. The
    // console values are the XDK's (Language_Get's switch).
    constexpr Language Languages[] = {
        { "en", 6, 1 },
        { "fr", 8, 4 },
        { "de", 9, 3 },
        { "es", 21, 5 },
        { "es-419", 22, 5 },
        { "it", 14, 6 },
        { "nl", 5, 16 },
        { "pt", 19, 9 },
        { "sv", 23, 13 },
        { "no", 17, 15 },
        { "da", 4, 0 },
        { "fi", 7, 0 },
        { "pl", 18, 11 },
        { "ru", 20, 12 },
        { "cs", 3, 0 },
        { "hu", 13, 0 },
        { "tr", 25, 14 },
        { "el", 10, 0 },
        { "ja", 15, 2 },
        { "ko", 16, 7 },
        { "zh", 2, 10 },
    };

    const Language* g_text = nullptr; // nullptr: the console language's, as the game maps it
}

bool trapteam::SetTextLanguage(std::string_view code)
{
    for (const Language& language : Languages)
    {
        if (language.code == code)
        {
            g_text = &language;
            return true;
        }
    }
    return false;
}

// The console language (XGetLanguage's wrapper): the title's language chooser reads it, and Language_Get too.
#ifdef GAME_HOOK_Console_Language
PPC_FUNC_IMPL(GAME_IMP(Console_Language));
PPC_FUNC(GAME_HOOK(Console_Language))
{
    GAME_IMP(Console_Language)(ctx, base);
    if (g_text != nullptr && g_text->console != 0)
        ctx.r3.u64 = g_text->console;
}
#endif

#ifdef GAME_HOOK_Language_Get
PPC_FUNC_IMPL(GAME_IMP(Language_Get));
PPC_FUNC(GAME_HOOK(Language_Get))
{
    GAME_IMP(Language_Get)(ctx, base);
    if (g_text != nullptr)
    {
        static bool logged = false;
        if (!logged)
        {
            fprintf(stderr, "[language] text %s: language %u, the title's was %u\n", std::string(g_text->code).c_str(),
                g_text->index, ctx.r3.u32);
            logged = true;
        }
        ctx.r3.u64 = g_text->index;
    }
}
#endif
