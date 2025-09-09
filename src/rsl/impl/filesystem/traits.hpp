#pragma once

namespace rsl
{
    struct file_traits
    {
        bool isFile{};
        bool isDirectory{};
        bool isValidPath{};

        bool canBeWritten{};
        bool canBeRead{};
        bool canBeCreated{};

        bool exists{};
    };

    constexpr static file_traits invalid_file_traits{ false, false, false, false, false, false, false };

    inline bool operator ==(const file_traits& rhs, const file_traits& lhs) noexcept
    {
        if (&rhs == &lhs)
        {
            return true;
        }

        return rhs.isFile == lhs.isFile &&
                rhs.isDirectory == lhs.isDirectory &&
                rhs.isValidPath == lhs.isValidPath &&
                rhs.canBeWritten == lhs.canBeWritten &&
                rhs.canBeRead == lhs.canBeRead &&
                rhs.canBeCreated == lhs.canBeCreated &&
                rhs.exists == lhs.exists;
    }

    inline bool operator !=(const file_traits& rhs, const file_traits& lhs) noexcept
    {
        return !(rhs == lhs);
    }

    struct filesystem_traits
    {
        bool isReadonly{};
        bool isValid{};
    };

    inline void inherit_traits(const filesystem_traits& filesystemTraits, file_traits& fileTraits) noexcept
    {
        if (!filesystemTraits.isValid)
        {
            fileTraits = invalid_file_traits;
            return;
        }
        if (filesystemTraits.isReadonly)
        {
            fileTraits.canBeWritten = false;
            fileTraits.canBeCreated = false;
        }
    }

    inline void inherit_traits(const filesystem_traits& outer, filesystem_traits& inner) noexcept
    {
        if (!outer.isValid)
        {
            inner.isValid = false;
        }
        if (outer.isReadonly)
        {
            inner.isReadonly = true;
        }
    }


    constexpr static filesystem_traits invalid_filesystem_traits{ false, false };

    inline void sanitize_traits(file_traits& traits) noexcept
    {
        if (traits.isFile)
        {
            traits.isDirectory = false;
        }

        if (traits.isDirectory)
        {
            traits.canBeWritten = false;
            traits.canBeRead = false;
        }

        if (traits.exists)
        {
            traits.canBeCreated = false;
            traits.isValidPath = true;
            if (traits.isFile)
            {
                traits.canBeRead = true;
            }
        }
    }
}
