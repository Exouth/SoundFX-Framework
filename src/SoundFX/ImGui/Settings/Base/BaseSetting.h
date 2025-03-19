#pragma once

#define ICON_FA_UNDO "\xef\x8b\xaa"      // Unicode: f2ea / fa-rotate-left
#define ICON_FA_QUESTION "\xef\x81\x99"  // Unicode: f059 / fa-circle-question

namespace SoundFX {
    class BaseSetting {
      public:
        virtual ~BaseSetting() = default;

        BaseSetting(const BaseSetting &other) = default;
        BaseSetting &
            operator=(const BaseSetting &other) = default;

        BaseSetting(BaseSetting &&other) noexcept = default;
        BaseSetting &
            operator=(BaseSetting &&other) noexcept = default;

        virtual void
            Render() = 0;
        virtual void
            Reset() = 0;
        [[nodiscard]] virtual std::string
            GetName() const = 0;

        void
            RenderTooltip() const {
            if (!description.empty() && ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 20);
                ImGui::Text("%s", GetName().c_str());
                ImGui::Spacing();
                ImGui::TextWrapped("%s", description.c_str());
                ImGui::EndTooltip();
            }
        }

      protected:
        explicit BaseSetting(std::string desc = "") : description(std::move(desc)) {
        }

        std::string description;
    };
}