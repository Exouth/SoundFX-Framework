#include "SettingsUI.h"
#include "ImGui/Renderers/SoundMarker.h"
#include "ImGui/Settings/DefaultSettings.h"
#include "ImGui/Settings/Types/Bool/BoolCheckboxSetting.h"
#include "ImGui/Settings/Types/Color/ColorSetting.h"
#include "ImGui/Settings/Types/Float/FloatSliderSetting.h"
#include "ImGui/Settings/Types/Int/IntComboBoxSetting.h"
#include "ImGui/Settings/Types/Int/IntSliderSetting.h"

namespace SoundFX {

    std::vector<std::unique_ptr<BaseSetting>> SettingsUI::settings;
    std::vector<std::unique_ptr<BaseSetting>> SettingsUI::colorSettings;

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

        settings.push_back(std::make_unique<BoolCheckboxSetting>(
            "Enable Radius Indicator",
            DefaultSettings::GetRadiusIndicator(),
            [](bool value) { SoundMarker::SetRadiusIndicator(value); },
            "Displays a visual representation of the sound's radius around each marker. "
            "This helps to understand the area affected by the sound event. (Usually only for 3D "
            "sounds)"));

        settings.push_back(std::make_unique<BoolCheckboxSetting>(
            "Enable Tracers",
            DefaultSettings::GetTracers(),
            [](bool value) { SoundMarker::EnableTracers(value); },
            "Displays a visual line from the player to each sound marker, helping to identify "
            "the source and position of sounds in the environment."));

        settings.push_back(std::make_unique<BoolCheckboxSetting>(
            "Enable Text Hover",
            DefaultSettings::GetTextHover(),
            [](bool value) { SoundMarker::EnableTextHover(value); },
            "Displays the name and sound effect above each sound marker when enabled. "
            "The name is how the sound is named, and the sound effect is the file set for the "
            "sound"));

        settings.push_back(std::make_unique<FloatSliderSetting>(
            "Max Render Distance",
            DefaultSettings::GetMaxRenderDistance(),
            0.0f,
            10000.0f,
            [](float value) { SoundMarker::SetMaxRenderDistance(value); },
            "Defines the maximum distance at which sound markers will be rendered. "
            "Markers beyond this range will not be displayed."));

        settings.push_back(std::make_unique<FloatSliderSetting>(
            "Radius Outline Thickness",
            DefaultSettings::GetRadiusOutlineThickness(),
            0.0f,
            25.0f,
            [](float value) { SoundMarker::SetRadiusOutlineThickness(value); },
            "Controls the thickness of the outline around the sound radius indicator. "
            "A thicker outline makes the radius more prominent, while a thinner one keeps it "
            "subtle."));

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

        settings.push_back(std::make_unique<IntComboBoxSetting>(
            "Max Sound Markers",
            DefaultSettings::GetMaxSoundMarkers(),
            std::vector {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, -1},
            [](int value) { SoundMarker::SetMaxSoundMarkers(value); },
            "Sets the maximum number of sound markers to render. Select -1 to render all "
            "markers."));

        settings.push_back(std::make_unique<IntSliderSetting>(
            "Circle Segments",
            DefaultSettings::GetNumSegmentsCircle(),
            10,
            300,
            [](int value) { SoundMarker::SetNumSegmentsCircle(value); },
            "Sets the number of segments for drawing circles. Higher values result in smoother "
            "circles."));

        settings.push_back(std::make_unique<IntSliderSetting>(
            "Sphere Segments",
            DefaultSettings::GetNumSegmentsSphere(),
            10,
            150,
            [](int value) { SoundMarker::SetNumSegmentsSphere(value); },
            "Sets the number of segments for drawing spheres. Higher values result in smoother "
            "spheres."));

        colorSettings.push_back(std::make_unique<ColorSetting>(
            "Marker Color",
            DefaultSettings::GetMarkerColor(),
            [](const ImVec4 &color) { SoundMarker::SetMarkerColor(color); },
            "Sets the color of the sound markers."));

        colorSettings.push_back(std::make_unique<ColorSetting>(
            "Radius Indicator Color",
            DefaultSettings::GetRadiusIndicatorColor(),
            [](const ImVec4 &color) { SoundMarker::SetRadiusIndicatorColor(color); },
            "Sets the color of the radius indicator visualizing sound area."));

        colorSettings.push_back(std::make_unique<ColorSetting>(
            "Tracer Color",
            DefaultSettings::GetTracerColor(),
            [](const ImVec4 &color) { SoundMarker::SetTracerColor(color); },
            "Sets the color of tracer lines connecting markers to the player."));

        colorSettings.push_back(std::make_unique<ColorSetting>(
            "Text Hover Color",
            DefaultSettings::GetTextHoverColor(),
            [](const ImVec4 &color) { SoundMarker::SetTextHoverColor(color); },
            "Sets the color of the text displayed above sound markers. This includes the sound "
            "name and effect."));
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

        ImGui::Spacing();

        if (ImGui::CollapsingHeader("Color Settings")) {
            for (const auto &colorSetting : colorSettings) {
                colorSetting->Render();
                ImGui::Spacing();
            }
        }

        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Restore default settings:");
        if (ImGui::Button("Reset all to default")) {
            for (const auto &setting : settings) {
                setting->Reset();
            }

            for (const auto &colorSetting : colorSettings) {
                colorSetting->Reset();
            }
        }
    }
}