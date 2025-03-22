#pragma once

#include <imgui.h>

namespace SoundFX {
    class RenderObject {
      public:
        static void
            Draw3DCircle(const RE::NiPoint3 &center,
                         float               radius,
                         ImDrawList         *drawList,
                         ImU32               color,
                         int                 numSegmentsCircle);
        static void
            Draw3DSphere(const RE::NiPoint3 &center,
                         float               radius,
                         ImDrawList         *drawList,
                         ImU32               color,
                         int                 numSegmentsSphere);
        static void
            Draw3DCircleOutline(const RE::NiPoint3 &center,
                                float               radius,
                                ImDrawList         *drawList,
                                ImU32               color,
                                float               thickness,
                                int                 numSegmentsCircle);
        static bool
            WorldToScreen(const RE::NiPoint3 &worldPos, ImVec2 &screenPos, float *depth = nullptr);

        static bool
            IsObjectObstructed(const RE::NiPoint3 &position,
                               float               radius,
                               int                 numSamples,
                               float               obstructionThreshold);

        static void
            DrawTracerLine(const RE::NiPoint3 &markerPos,
                           const RE::NiPoint3 &playerPos,
                           ImDrawList         *drawList,
                           ImU32               color,
                           float               thickness);

        static void
            DrawTextAboveSphere(const RE::NiPoint3 &center,
                                float               radius,
                                ImU32               color,
                                ImDrawList         *drawList,
                                const std::string  &name,
                                const std::string  &eventType,
                                const std::string  &soundEffect,
                                bool                isObstructed);

      private:
        static bool
            PickObject(const RE::NiPoint3 &from, const RE::NiPoint3 &to);
    };
}