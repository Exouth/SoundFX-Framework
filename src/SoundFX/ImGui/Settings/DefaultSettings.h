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
    };
}