#include "SettingsUI.h"
#include "ImGui/Renderers/SoundMarker.h"
#include "ImGui/Settings/DefaultSettings.h"
#include "ImGui/Settings/Types/Bool/BoolCheckboxSetting.h"
#include "ImGui/Settings/Types/Float/FloatSliderSetting.h"

namespace SoundFX {

    std::vector<std::unique_ptr<BaseSetting>> SettingsUI::settings;

    void
        SettingsUI::InitializeSettings() {
        settings.push_back(std::make_unique<BoolCheckboxSetting>(
            "Show Sound Markers",
            DefaultSettings::GetShowSoundMarkers(),
            [](bool value) { SoundMarker::ToggleVisibility(value); },
            "Toggles the visibility of sound markers in the world. When enabled, "
            "sound events will be visually represented."));

        settings.push_back(std::make_unique<BoolCheckboxSetting>(
            "Enable Distance Filter",
            DefaultSettings::GetDistanceFilterEnabled(),
            [](bool value) { SoundMarker::EnableDistanceFilter(value); },
            "When enabled, sound markers will only be displayed if they are within the "
            "maximum render distance."));

        settings.push_back(std::make_unique<BoolCheckboxSetting>(
            "Enable Obstruction Effect",
            DefaultSettings::GetObstructionEffectEnabled(),
            [](bool value) { SoundMarker::EnableObstructionEffect(value); },
            "When enabled, sound markers will turn gray when they are obstructed "
            "by objects in the world, indicating that they are not in direct view or behind "
            "something."));

        settings.push_back(std::make_unique<FloatSliderSetting>(
            "Max Render Distance",
            DefaultSettings::GetMaxRenderDistance(),
            0.0f,
            10000.0f,
            [](float value) { SoundMarker::SetMaxRenderDistance(value); },
            "Defines the maximum distance at which sound markers will be rendered. "
            "Markers beyond this range will not be displayed."));

        // Testing Purpose (Delete later)
        settings.push_back(std::make_unique<FloatSliderSetting>(
            "Sound Radius", DefaultSettings::GetSoundRadius(), 0.0f, 1000.0f, [](float value) {
                SoundMarker::SetSoundRadius(value);
            }));

        settings.push_back(std::make_unique<FloatSliderSetting>(
            "Obstruction Threshold",
            DefaultSettings::GetObstructionThreshold(),
            0.0f,
            1.0f,
            [](float value) { SoundMarker::SetObstructionThreshold(value); },
            "Adjusts how much of a marker needs to be hidden before it turns gray. Higher values "
            "require more obstruction before changing color."));
    }

    void
        SettingsUI::RenderSettingsUI() {
        if (settings.empty()) {
            InitializeSettings();
        }

        for (const auto &setting : settings) {
            setting->Render();
            ImGui::Spacing();
        }

        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Restore default settings:");
        if (ImGui::Button("Reset all to default")) {
            for (const auto &setting : settings) {
                setting->Reset();
            }
        }
    }
}