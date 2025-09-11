#include "filesystem_provider.hpp"

#include "file_solution.hpp"

namespace rsl::filesystem
{
    filesystem_provider& filesystem_provider::register_domain(const string_view domain) noexcept
    {
        m_domains.push_back(dynamic_string::from_view(domain));
        return *this;
    }

    array_view<const dynamic_string> filesystem_provider::get_domains() const noexcept
    {
        return m_domains;
    }

    void filesystem_provider::set_solution_provider(file_solution* solution, filesystem_provider* provider)
    {
        solution->m_provider = provider;
    }
}
