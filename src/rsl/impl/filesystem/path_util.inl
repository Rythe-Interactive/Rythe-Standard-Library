#pragma once

namespace rsl::filesystem
{
    constexpr char separator() noexcept
    {
        if constexpr (RYTHE_PLATFORM_INDEX == RYTHE_WINDOWS_INDEX)
        {
            return '\\';
        }
        else
        {
            return '/';
        }
    }

    constexpr char anti_separator() noexcept
    {
        if constexpr (RYTHE_PLATFORM_INDEX == RYTHE_WINDOWS_INDEX)
        {
            return '/';
        }
        else
        {
            return '\\';
        }
    }

    constexpr dynamic_string domain(const string_view path)
    {
        const size_type idx = linear_search(path, ':');
        if (idx == npos)
        {
            return dynamic_string{};
        }

        return dynamic_string::from_variadic_items(path.subview(0, idx + 1), separator(), separator());
    }

    constexpr string_view parent(const string_view path) noexcept
    {
        return path.subview(0ull, reverse_linear_search(path, separator_char{}, path.size() - 2ull));
    }
    constexpr string_view filename(const string_view path) noexcept
    {
        return path.subview(reverse_linear_search(path, separator_char{}));
    }

    constexpr string_view extension(const string_view path, const bool fullExtension) noexcept
    {
        const string_view fullname = filename(path);
        if (fullExtension)
        {
            return fullname.subview(linear_search(fullname, '.'));
        }

        return fullname.subview(reverse_linear_search(fullname, '.'));
    }

    constexpr string_view stem(const string_view path) noexcept
    {
        const string_view fullname = filename(path);
        return fullname.subview(0ull, linear_search(fullname, '.'));
    }

    constexpr dynamic_string subdir(const string_view path, const string_view sub)
    {
        const string_view sanitizedPath = trim_right(path, separator_char{});
        dynamic_string result;
        result.reserve(sanitizedPath.size() + sub.size() + 1ull);
        result.append(sanitizedPath);
        result.append(separator());
        result.append(sub);
        return result;
    }

    constexpr dynamic_string sanitize(const string_view path, const bool failOnFsLeave)
    {
        dynamic_array<string_view> recreation;

        dynamic_array<string_view> tokens = split_string_at(path, separator_char{});

        dynamic_string filesystem;
        for (const string_view& token : tokens)
        {
            //skip filesystem decl
            if (linear_search(token, ':') != npos)
            {
                filesystem = dynamic_string::from_variadic_items(token, separator(), separator());
                continue;
            }

            if (token.empty())
            {
                continue;
            }

            //handle upwards
            if (trim_right(token) == "..")
            {
                if (!recreation.empty() && recreation.back() != "..")
                {
                    recreation.pop_back();
                }
                else if (failOnFsLeave)
                {
                    //someone is trying something fishy! terminate parsing
                    return {};
                }
                else
                {
                    recreation.emplace_back("..");
                }
            }

            //handle normal tokens and .
            else if (trim_right(token) != ".")
            {
                recreation.push_back(token);
            }
        }

        return filesystem + join_strings_with(recreation, separator());
    }
}
