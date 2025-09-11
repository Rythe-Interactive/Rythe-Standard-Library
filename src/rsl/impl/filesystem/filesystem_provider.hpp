#pragma once
#include "../containers/array.hpp"
#include "../util/container_util.hpp"
#include "../util/error_handling.hpp"

#include "traits.hpp"

namespace rsl::filesystem
{
    class view;
    class file_solution;

    class filesystem_provider
    {
    public:
        filesystem_provider() = default;
        virtual ~filesystem_provider() = default;

        template<string_like Domain, string_like... Domains>
        explicit filesystem_provider(Domain&& domain, Domains&&... domains);

        filesystem_provider& register_domain(string_view domain) noexcept;
        [[nodiscard]] array_view<const dynamic_string> get_domains() const noexcept;
        [[nodiscard]] virtual filesystem_traits get_traits() const = 0;
        [[nodiscard]] virtual result<dynamic_array<view>> ls() const = 0;

        [[nodiscard]] virtual result<file_solution*> create_solution(string_view path) = 0;
        virtual void release_solution(file_solution* solution) = 0;

    protected:
        static void set_solution_provider(file_solution* solution, filesystem_provider* provider);

        dynamic_array<dynamic_string> m_domains;
    };
}

#include "filesystem_provider.inl"
