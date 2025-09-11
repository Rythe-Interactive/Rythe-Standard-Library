#pragma once
#include "../util/error_handling.hpp"

#include "traits.hpp"

namespace rsl::filesystem
{
    class filesystem_provider;
    class view;

    class file_solution
    {
    public:
        VIRTUAL_RULE_OF_5(file_solution)

        [[nodiscard]] [[rythe_always_inline]] const filesystem_provider* get_provider() const noexcept;
        [[nodiscard]] [[rythe_always_inline]] filesystem_provider* get_provider() noexcept;
         [[rythe_always_inline]] void release();

        [[nodiscard]] virtual file_traits file_info() const = 0;
        [[nodiscard]] [[rythe_always_inline]] filesystem_traits filesystem_info() const;
        [[nodiscard]] virtual result<dynamic_array<view>> ls() const = 0;

        [[nodiscard]] virtual result<byte_view> read() const = 0;
        [[nodiscard]] virtual result<void> write(byte_view data) = 0;
        [[nodiscard]] virtual result<void> append(byte_view data) = 0;
        [[nodiscard]] virtual result<void> flush() const = 0;

    protected:
        friend class filesystem_provider;

        filesystem_provider* m_provider;
    };
}

#include "file_solution.inl"
