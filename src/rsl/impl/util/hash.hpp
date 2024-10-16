#pragma once
#include <span>
#include <unordered_map>

#include "../defines.hpp"
#include "primitives.hpp"
#include "string_util.hpp"
#include "type_util.hpp"

namespace rsl
{
    template <typename T>
	constexpr size_type fnv1a_hash_bytes(size_type base, std::span<byte>);

	template <typename hash_type, typename... hash_types>
	constexpr static id_type combine_hash(id_type seed, hash_type hash, hash_types... hashes);

	template <typename T>
	constexpr static id_type hash(const T& val);

	struct name_hash
	{
		id_type value = invalid_id;

		constexpr operator id_type() const noexcept;

		constexpr name_hash() noexcept;

		template <size_type N>
		constexpr name_hash(const char (&name)[N]) noexcept;

		template <size_type N>
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
	} // namespace detail

	id_type local_cast(id_type global);
	id_type global_cast(id_type local);

	struct type_hash_base
	{
		friend struct type_reference;
		virtual ~type_hash_base() = default;
		virtual id_type local() const noexcept = 0;
		virtual std::string_view local_name() const noexcept = 0;
		virtual id_type global() const noexcept = 0;
		virtual std::string_view global_name() const noexcept = 0;

	protected:
		virtual type_hash_base* copy() const = 0;
	};

	template <typename T>
	struct type_hash : public type_hash_base
	{
		id_type value = invalid_id;
		std::string_view name;

		constexpr operator id_type() const noexcept;

		constexpr type_hash() noexcept
			: value(localTypeHash<T>()),
			  name(localNameOfType<T>())
		{
		}

		type_hash(const type_hash& src) noexcept
			: value(src.value),
			  name(src.name)
		{
		}
		type_hash(type_hash&& src) noexcept
			: value(src.value),
			  name(src.name)
		{
		}

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

	RYTHE_GCC_SUPPRESS_WARNING_POP;
	RYTHE_CLANG_SUPPRESS_WARNING_POP;

	namespace detail
	{
		static std::unordered_map<id_type, type_reference> hash_to_reference;
	}

	type_reference type_ref_cast(id_type hash);

	template <typename T>
	constexpr type_hash<T> make_hash() noexcept
	{
		return type_hash<T>{};
	}

	template <size_type N>
	constexpr name_hash make_hash(const string_literal<N>& src) noexcept
	{
		return name_hash{src};
	}

	namespace literals
	{
		constexpr name_hash operator""_hs(cstring src);
	}


	template <typename T>
	constexpr static size_type typeHash()
	{
		return make_hash<T>();
	}

} // namespace rsl

#include "hash.inl"

#if !defined(DOXY_EXCLUDE)
namespace std
{
	template <>
	struct hash<rsl::type_reference>
	{
		std::size_t operator()(rsl::type_reference const& handle) const noexcept
		{
			return handle.local();
		}
	};
} // namespace std
#endif
