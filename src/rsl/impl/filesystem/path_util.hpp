#pragma once
#include "../defines.hpp"

#include "../util/string_util.hpp"

namespace rsl::filesystem
{
    [[nodiscard]] [[rythe_always_inline]] constexpr char separator() noexcept;
    [[nodiscard]] [[rythe_always_inline]] constexpr char anti_separator() noexcept;

    struct separator_char
    {
        [[nodiscard]] [[rythe_always_inline]] bool operator==(const separator_char) const noexcept { return true; }
        [[nodiscard]] [[rythe_always_inline]] bool operator!=(const separator_char) const noexcept { return false; }

        [[nodiscard]] [[rythe_always_inline]] bool operator==(const char ch) const noexcept
        {
            return ch == '\\' || ch == '/';
        }

        [[nodiscard]] [[rythe_always_inline]] bool operator!=(const char ch) const noexcept
        {
            return !(*this == ch);
        }

        [[nodiscard]] [[rythe_always_inline]] operator char() const noexcept
        {
            return separator();
        }
    };

    [[nodiscard]] [[rythe_always_inline]] constexpr string_view parent(string_view path) noexcept;
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string subdir(string_view path, string_view sub);
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string sanitize(string_view path, bool failOnFsLeave = false);
    [[nodiscard]] [[rythe_always_inline]] constexpr dynamic_string localize(string_view path);
    [[rythe_always_inline]] constexpr void localize(in_place_signal_type, dynamic_string& path);
}

#include "path_util.inl"
