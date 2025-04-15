#include "Theme.h"
#include <imgui.h>

namespace SoundFX {

    void
        Theme::ApplyTheme() {
        ImGuiStyle &style  = ImGui::GetStyle();
        ImVec4     *colors = style.Colors;

        colors[ImGuiCol_WindowBg]         = ImVec4(0.12f, 0.13f, 0.14f, 1.00f);
        colors[ImGuiCol_TitleBg]          = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
        colors[ImGuiCol_TitleBgActive]    = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.05f, 0.05f, 0.06f, 1.00f);

        colors[ImGuiCol_Button]        = ImVec4(0.20f, 0.22f, 0.25f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.30f, 0.34f, 1.00f);
        colors[ImGuiCol_ButtonActive]  = ImVec4(0.37f, 0.40f, 0.45f, 1.00f);

        colors[ImGuiCol_Border]       = ImVec4(0.25f, 0.27f, 0.30f, 1.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        colors[ImGuiCol_Text]         = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

        colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);

        colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.30f, 0.30f, 0.35f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.45f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);

        style.ScrollbarSize = 10.0f;
        style.GrabMinSize   = 8.0f;

        style.FrameRounding = 0.0f;

        style.ColorButtonPosition     = ImGuiDir_Left;
        style.SeparatorTextBorderSize = 2.0f;

        style.Alpha = 0.97f;

        style.WindowBorderSize = 0.0f;

        style.WindowPadding = ImVec2(12.0f, 12.0f);
        style.FramePadding  = ImVec2(8.0f, 5.0f);
        style.ItemSpacing   = ImVec2(10.0f, 4.0f);
    }

    void
        Theme::RenderFooterLine() {
        ImDrawList *drawList = ImGui::GetWindowDrawList();

        const ImVec2 windowPos  = ImGui::GetWindowPos();
        const ImVec2 windowSize = ImGui::GetWindowSize();

        const auto startPoint = ImVec2(windowPos.x, windowPos.y + windowSize.y - 1);
        const auto endPoint   = ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y - 1);

        ImVec4 resizeGripColor = ImGui::GetStyleColorVec4(ImGuiCol_ResizeGrip);
        resizeGripColor.w      = 1.0f;

        drawList->AddLine(startPoint, endPoint, ImGui::GetColorU32(resizeGripColor), 8.0f);
    }

    void
        Theme::DrawVerticalSeparator(float height) {
        const auto  cursorPos = ImGui::GetCursorScreenPos();
        ImDrawList *drawList  = ImGui::GetWindowDrawList();
        ImU32       color     = ImGui::GetColorU32(ImGuiCol_Separator);

        drawList->AddLine(ImVec2(cursorPos.x + 4.0f, cursorPos.y),
                          ImVec2(cursorPos.x + 4.0f, cursorPos.y + height),
                          color,
                          1.0f);

        ImGui::InvisibleButton("##vseparator", ImVec2(8.0f, height));
    }
}