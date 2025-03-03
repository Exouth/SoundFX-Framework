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
            WorldToScreen(const RE::NiPoint3 &worldPos, ImVec2 &screenPos, float &depth);
    };
}