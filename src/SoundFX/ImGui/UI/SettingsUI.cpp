#include "SettingsUI.h"
#include "ImGui/Renderers/SoundMarker.h"
#include "ImGui/Settings/Types/Bool/BoolCheckboxSetting.h"
#include "ImGui/Settings/Types/Float/FloatSliderSetting.h"

namespace SoundFX {

    std::vector<std::unique_ptr<BaseSetting>> SettingsUI::settings;

    void
        SettingsUI::InitializeSettings() {
        settings.push_back(std::make_unique<BoolCheckboxSetting>(
            "Show Sound Markers", SoundMarker::IsVisible(), [](bool value) {
                SoundMarker::ToggleVisibility();
            }));

        settings.push_back(
            std::make_unique<BoolCheckboxSetting>("Enable Distance Filter", true, [](bool value) {
                SoundMarker::EnableDistanceFilter(value);
            }));

        settings.push_back(std::make_unique<FloatSliderSetting>(
            "Max Render Distance", 4000.0f, 0.0f, 10000.0f, [](float value) {
                SoundMarker::SetMaxRenderDistance(value);
            }));

        // Testing Purpose (Delete later)
        settings.push_back(std::make_unique<FloatSliderSetting>(
            "Sound Radius", 100.0f, 0.0f, 1000.0f, [](float value) {
                SoundMarker::SetSoundRadius(value);
            }));
    }

    void
        SettingsUI::RenderSettingsUI() {
        if (settings.empty()) {
            InitializeSettings();
        }

        for (const auto &setting : settings) {
            setting->Render();
        }
    }
}