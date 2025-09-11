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
        file_solution() = default;

        const filesystem_provider* get_provider() const noexcept;
        filesystem_provider* get_provider() noexcept;
        void release();

        [[nodiscard]] file_traits file_info() const;
        [[nodiscard]] filesystem_traits filesystem_info() const;
        [[nodiscard]] result<dynamic_array<view>> ls() const;

        [[nodiscard]] result<byte_view> read() const;
        [[nodiscard]] result<void> write(byte_view data);
        [[nodiscard]] result<void> append(byte_view data);
        [[nodiscard]] result<void> flush() const;

    private:
        friend class filesystem_provider;

        filesystem_provider* m_provider;
    };
}
