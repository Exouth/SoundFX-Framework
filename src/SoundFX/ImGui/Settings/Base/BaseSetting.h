#pragma once

#define ICON_FA_UNDO "\xef\x8b\xaa"  // Unicode: f2ea / fa-rotate-left

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

      protected:
        BaseSetting() = default;
    };
}