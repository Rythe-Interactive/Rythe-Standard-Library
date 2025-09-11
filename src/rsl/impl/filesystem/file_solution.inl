#pragma once

namespace rsl::filesystem
{
    inline const filesystem_provider* file_solution::get_provider() const noexcept
    {
        return m_provider;
    }

    inline filesystem_provider* file_solution::get_provider() noexcept
    {
        return m_provider;
    }

    inline void file_solution::release()
    {
        if (!m_provider)
        {
            return;
        }

        m_provider->release_solution(this);
        m_provider = nullptr;
    }

    inline filesystem_traits file_solution::filesystem_info() const
    {
        if (!m_provider)
        {
            return invalid_filesystem_traits;
        }

        return m_provider->filesystem_info();
    }
}
