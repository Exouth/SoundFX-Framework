#pragma once

namespace SoundFX {
    class InputListener {
      public:
        static InputListener *
            GetSingleton();
        void
            ProcessEvent(RE::InputEvent **event);

      private:
        InputListener()  = default;
        ~InputListener() = default;

        InputListener(const InputListener &) = delete;
        InputListener &
            operator=(const InputListener &) = delete;
    };
}