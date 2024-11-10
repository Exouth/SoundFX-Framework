#pragma once

#include <RE/Skyrim.h>
#include <string>
#include <vector>

namespace SoundFX {

    std::string
        FormIDToString(RE::FormID formID, bool asHex = true);

    std::vector<RE::TESForm *>
        GetAllFormsByEditorID(const std::string &editorID);

    RE::FormID
        GetFormIDFromEditorIDAndPluginName(const std::string &editorID,
                                           const std::string &pluginName);
}