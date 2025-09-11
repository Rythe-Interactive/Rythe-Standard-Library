#pragma once
#include "../containers/array.hpp"
#include "../memory/index_allocator.hpp"
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
        VIRTUAL_RULE_OF_5(filesystem_provider);

        template <string_like Domain, string_like... Domains>
        explicit filesystem_provider(Domain&& domain, Domains&&... domains);

        filesystem_provider& register_domain(string_view domain) noexcept;
        [[nodiscard]] array_view<const dynamic_string> get_domains() const noexcept;
        [[nodiscard]] virtual filesystem_traits filesystem_info() const = 0;
        [[nodiscard]] virtual result<dynamic_array<view>> ls() const = 0;

        [[nodiscard]] virtual result<file_solution*> create_solution(string_view path) = 0;
        virtual void release_solution(file_solution* solution) = 0;

    protected:
        static void set_solution_provider(file_solution* solution, filesystem_provider* provider);

        pair<index_type, bool> create_solution_reference(const dynamic_string& path);
        void destroy_solution_reference(const dynamic_string& path);
        index_type find_existing_solution(const dynamic_string& path);
        const manual_reference_counter& get_reference_count_status(index_type index) const;

        dynamic_array<dynamic_string> m_domains;
        index_allocator m_solutionIndexAllocator;
        dynamic_map<dynamic_string, index_type> m_solutionMap;
        dynamic_array<manual_reference_counter> m_solutionReferences;
    };
}

#include "filesystem_provider.inl"
