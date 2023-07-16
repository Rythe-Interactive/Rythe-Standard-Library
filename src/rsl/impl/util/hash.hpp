#pragma once
#include <unordered_map>
#include <memory>

#include "primitives.hpp"
#include "string_util.hpp"
#include "type_util.hpp"

namespace rsl
{
    constexpr static size_type hash_combine(size_type seed, size_type hash)
    {
        return seed ^ (hash + (0x9e3779b9 + (seed << 6) + (seed >> 2)));
    }

    template<typename hash_type, typename... hash_types>
    constexpr static size_type hash_combine_multiple(size_type seed, hash_type hash, hash_types... hashes)
    {
        static_assert(std::is_same_v<size_type, hash_type>, "Hashes need to be of type: size_type");

        seed ^= (hash + (0x9e3779b9 + (seed << 6) + (seed >> 2)));
        if constexpr (sizeof...(hash_types) == 1)
        {
            return hash_combine(seed, hashes...);
        }
        else
        {
            return hash_combine_multiple(seed, hashes...);
        }
    }

    template<typename T>
    constexpr static size_type hash(const T& val)
    {
        constexpr ::std::hash<T> hasher{};
        return hasher(val);
    }

    template<typename T>
    constexpr static size_type typeHash()
    {
        return typeid(T).hash_code();
    }

     constexpr id_type combine_hash(id_type seed, id_type value);

    struct name_hash
    {
        id_type value = invalid_id;

        constexpr operator id_type () const noexcept;

        constexpr name_hash() noexcept;

        template<size_type N>
        constexpr name_hash(const char(&name)[N]) noexcept;

        template<size_type N>
        constexpr name_hash(const string_literal<N>& name) noexcept;

        constexpr name_hash(id_type value) noexcept;

        name_hash(const name_hash& src) noexcept;
        name_hash(name_hash&& src) noexcept;

        name_hash& operator=(const name_hash& src) noexcept;
        name_hash& operator=(name_hash&& src) noexcept;
    };

    namespace detail
    {
        static std::unordered_map<id_type, id_type> local_to_global;
        static std::unordered_map<id_type, id_type> global_to_local;
    }

    id_type local_cast(id_type global);
    id_type global_cast(id_type local);

    struct type_hash_base
    {
        friend struct type_reference;

        virtual id_type local() const noexcept RYTHE_PURE;
        virtual std::string_view local_name() const noexcept RYTHE_PURE;
        virtual id_type global() const noexcept RYTHE_PURE;
        virtual std::string_view global_name() const noexcept RYTHE_PURE;

    protected:
        virtual type_hash_base* copy() const RYTHE_PURE;
    };

    template<typename T>
    struct type_hash : public type_hash_base
    {
        std::string_view name;
        id_type value = invalid_id;

        constexpr operator rsl::id_type () const noexcept;

        constexpr type_hash() noexcept : value(localTypeHash<T>()), name(localNameOfType<T>()) {}

        type_hash(const type_hash& src) noexcept : value(src.value), name(src.name) {}
        type_hash(type_hash&& src) noexcept : value(src.value), name(src.name) {}

        type_hash& operator=(const type_hash& src) noexcept
        {
            value = src.value;
            name = src.name;
            return *this;
        }

        type_hash& operator=(type_hash&& src) noexcept
        {
            value = src.value;
            name = src.name;
            return *this;
        }

        virtual id_type local() const noexcept
        {
            return value;
        }

        virtual std::string_view local_name() const noexcept
        {
            return name;
        }

        virtual id_type global() const noexcept
        {
            if (!detail::local_to_global.count(value))
            {
                auto global = typeHash<T>();
                detail::local_to_global.emplace(value, global);
                detail::global_to_local.emplace(global, value);
            }
            return typeHash<T>();
        }

        virtual std::string_view global_name() const noexcept
        {
            if (!detail::local_to_global.count(value))
            {
                auto global = typeHash<T>();
                detail::local_to_global.emplace(value, global);
                detail::global_to_local.emplace(global, value);
            }
            return nameOfType<T>();
        }

    protected:
        virtual type_hash_base* copy() const
        {
            if (!detail::local_to_global.count(value))
            {
                auto global = typeHash<T>();
                detail::local_to_global.emplace(value, global);
                detail::global_to_local.emplace(global, value);
            }
            return new type_hash();
        }
    };

    RYTHE_CLANG_SUPPRESS_WARNING_PUSH
    RYTHE_GCC_SUPPRESS_WARNING_PUSH
    RYTHE_CLANG_SUPPRESS_WARNING("-Wdelete-abstract-non-virtual-dtor")
    RYTHE_GCC_SUPPRESS_WARNING("-Wdelete-abstract-non-virtual-dtor")

    struct type_reference
    {
    private:
        std::unique_ptr<type_hash_base> value;

    public:
        type_reference() = default;
        type_reference(std::nullptr_t);
        type_reference(const type_hash_base& src);

        type_reference(const type_reference& src);
        type_reference(type_reference&& src);

        type_reference& operator=(const type_reference& src);
        type_reference& operator=(type_reference&& src);

        id_type local() const;
        id_type global() const;
        std::string_view local_name() const;
        std::string_view global_name() const;

        operator id_type() const;
    };

    RYTHE_GCC_SUPPRESS_WARNING_POP
    RYTHE_CLANG_SUPPRESS_WARNING_POP

    namespace detail
    {
        static std::unordered_map<id_type, type_reference> hash_to_reference;
    }

    type_reference type_ref_cast(id_type hash);

    template<typename T>
    constexpr type_hash<T> make_hash() noexcept
    {
        return type_hash<T>{};
    }

    template<size_type N>
    constexpr name_hash make_hash(const string_literal<N>& src) noexcept
    {
        return name_hash{ src };
    }

    namespace literals
    {
        constexpr name_hash operator""_hs(cstring src);
    }

}

#include "hash.inl"

#if !defined(DOXY_EXCLUDE)
namespace std
{
    template<> struct hash<rsl::type_reference>
    {
        std::size_t operator()(rsl::type_reference const& handle) const noexcept
        {
            return handle.local();
        }
    };
}
#endif
