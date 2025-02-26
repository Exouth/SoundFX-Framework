#pragma once

namespace SoundFX {
    class InputListener {
      public:
        InputListener(const InputListener &) = delete;
        InputListener &
            operator=(const InputListener &) = delete;

        InputListener(InputListener &&) = delete;
        InputListener &
            operator=(InputListener &&) = delete;

        static InputListener *
            GetSingleton();
        static void
            ProcessEvent(RE::InputEvent **event);

      private:
        InputListener()  = default;
        ~InputListener() = default;
    };
}