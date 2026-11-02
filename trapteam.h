#pragma once

#include <string_view>

// Trap Team's own code, shared by the files of games/trapteam/.
namespace trapteam
{
    // The text language in any language of the title's list, whatever the console language (language.cpp).
    bool SetTextLanguage(std::string_view code);
}
