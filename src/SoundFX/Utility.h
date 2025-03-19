#pragma once

namespace SoundFX {

    std::string
        FormIDToString(RE::FormID formID, bool asHex = true);

    std::vector<RE::TESForm *>
        GetAllFormsByEditorID(const std::string &editorID);

    RE::FormID
        GetFormIDFromEditorIDAndPluginName(const std::string &editorID,
                                           const std::string &pluginName);

    void
        DelayExec(float delayInSeconds, const std::function<void()> &task);

    float
        GenerateRandomFloat();

    float
        CalculateMarkerSize(float distance,
                            float minSize     = 5.0f,
                            float maxSize     = 10.0f,
                            float scaleFactor = 3000.0f);

    ImU32
        ConvertColor(const ImVec4 &color);

    bool
        NaturalStringCompare(const std::string &a, const std::string &b);
}