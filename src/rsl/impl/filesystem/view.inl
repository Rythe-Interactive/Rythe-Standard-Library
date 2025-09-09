#pragma once
#include <filesystem>

namespace rsl::filesystem
{
    constexpr view::view(const string_view path) noexcept : m_path(dynamic_string::from_view(path)) {}

    inline view::operator bool() const noexcept
    {
        return is_valid();
    }

    inline bool view::is_valid(bool deepCheck) const
    {
        if (m_path.empty())
        {
            return false;
        }

        return true;
    }

    inline dynamic_string view::domain() const
    {
        const size_type idx = linear_search(m_path.view(), ':');
        if (idx == npos)
        {
            return dynamic_string{};
        }

        return dynamic_string::from_variadic_items(m_path.subview(0, idx + 1), separator(), separator());
    }

    inline string_view view::path() const
    {
        return m_path;
    }

    inline string_view view::extension(const bool fullExtension) const
    {
        const string_view fullname = this->filename();
        if (fullExtension)
        {
            return fullname.subview(linear_search(fullname, '.'));
        }

        return fullname.subview(reverse_linear_search(fullname, '.'));
    }

    inline string_view view::filename() const
    {
        return m_path.subview(reverse_linear_search(m_path.view(), separator_char{}));
    }

    inline string_view view::stem() const
    {
        const string_view fullname = this->filename();
        return fullname.subview(0ull, linear_search(fullname, '.'));
    }

    inline view view::parent() const
    {
        const size_type end = reverse_linear_search(m_path.view(), separator_char{});
        const size_type start = reverse_linear_search(m_path.view(), separator_char{}, m_path.size() - (end + 1ull));
        return m_path.subview(start, end - start);
    }

    inline view view::subdir(const string_view identifier) const
    {
        return view(fs::subdir(m_path, identifier));
    }

    inline view view::operator/(const string_view identifier) const
    {
        return subdir(identifier);
    }
}
