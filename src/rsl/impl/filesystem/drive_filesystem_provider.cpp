#include "drive_filesystem_provider.hpp"

#include "path_util.hpp"

namespace rsl::filesystem
{

    file_traits drive_file_solution::file_info() const
    {
        if (!m_provider)
        {
            return invalid_file_traits;
        }

        file_traits traits;
        traits.isDirectory = m_absolutePath.back() == separator();
        traits.isFile = !traits.isDirectory;
        traits.isValidPath = true;
        traits.canBeWritten = traits.isFile;
        traits.canBeRead = traits.isFile;
        traits.canBeCreated = true;
        traits.exists = true;

        inherit_traits(m_provider->filesystem_info(), traits);

        return traits;
    }

    filesystem_traits drive_filesystem_provider::filesystem_info() const
    {

    }

    result<dynamic_array<view>> drive_filesystem_provider::ls() const
    {

    }

    result<file_solution*> drive_filesystem_provider::create_solution(const string_view path)
    {
        const dynamic_string solutionPath = dynamic_string::from_view(path);
        auto [index, newValue] = create_solution_reference(solutionPath);
        if (newValue)
        {
            if (index >= m_solutions.size())
            {
                m_solutions.resize(index + 1);
            }

            set_solution_provider(&m_solutions[index], this);
        }

        return &m_solutions[index];
    }

    void drive_filesystem_provider::release_solution(file_solution* solution)
    {

    }
}
