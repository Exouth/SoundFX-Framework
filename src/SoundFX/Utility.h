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
                            float maxSize     = 35.0f,
                            float scaleFactor = 1500.0f);
}