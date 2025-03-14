#pragma once

namespace SoundFX {
    class DefaultSettings {
      public:
        static bool
            GetShowSoundMarkers() {
            return false;
        }

        static bool
            GetDistanceFilterEnabled() {
            return true;
        }

        static float
            GetMaxRenderDistance() {
            return 4000.0f;
        }

        static float
            GetSoundRadius() {
            return 100.0f;
        }

        static bool
            GetObstructionEffectEnabled() {
            return true;
        }

        static float
            GetObstructionThreshold() {
            return 0.5f;
        }

        static bool
            GetRadiusIndicator() {
            return true;
        }

        static float
            GetRadiusOutlineThickness() {
            return 5.0f;
        }

        static bool
            GetTracers() {
            return true;
        }

        static int
            GetMaxSoundMarkers() {
            return 10;
        }

        static int
            GetNumSegmentsCircle() {
            return 100;
        }

        static int
            GetNumSegmentsSphere() {
            return 35;
        }

        static bool
            GetTextHover() {
            return true;
        }

        static ImVec4
            GetMarkerColor() {
            return {1.0f, 1.0f, 0.0f, 1.0f};
        }

        static ImVec4
            GetRadiusIndicatorColor() {
            return {0.0f, 0.0f, 1.0f, 0.5f};
        }

        static ImVec4
            GetTracerColor() {
            return {1.0f, 1.0f, 0.0f, 1.0f};
        }

        static ImVec4
            GetTextHoverColor() {
            return {1.0f, 1.0f, 1.0f, 1.0f};
        }
    };
}