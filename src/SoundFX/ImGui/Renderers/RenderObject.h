#pragma once

namespace SoundFX {
    class RenderObject {
      public:
        static void
            Draw3DCircle(const RE::NiPoint3 &center,
                         float               radius,
                         ImDrawList         *drawList,
                         ImU32               color);
        static void
            Draw3DSphere(const RE::NiPoint3 &center,
                         float               radius,
                         ImDrawList         *drawList,
                         ImU32               color);
        static void
            Draw3DCircleOutline(const RE::NiPoint3 &center,
                                float               radius,
                                ImDrawList         *drawList,
                                ImU32               color,
                                float               thickness);
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

      private:
        static bool
            PickObject(const RE::NiPoint3 &from, const RE::NiPoint3 &to);
    };
}