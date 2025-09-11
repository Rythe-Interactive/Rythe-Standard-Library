#pragma once

#include "../containers/views.hpp"

#include "container_util.hpp"

#ifndef RSL_DEFAULT_HASH_MODE
	#define RSL_DEFAULT_HASH_MODE fast_hash
#endif

namespace rsl
{
	enum struct [[rythe_closed_enum]] hash_mode : u8
	{
		fast_hash,      // fast hashing
		protected_hash, // extra protection against entropy loss
		default_hash = RSL_DEFAULT_HASH_MODE
	};

	template <hash_mode Mode>
	[[rythe_always_inline]] constexpr id_type hash_bytes(byte_view bytes) noexcept;

	[[rythe_always_inline]] constexpr id_type hash_bytes(byte_view bytes) noexcept;

	template <hash_mode Mode>
	[[rythe_always_inline]] constexpr id_type hash_string(rsl::string_view str) noexcept;

	[[rythe_always_inline]] constexpr id_type hash_string(rsl::string_view str) noexcept;

	template <hash_mode Mode, same_as<id_type>... HashTypes>
	[[rythe_always_inline]] constexpr id_type
	combine_hash(id_type seed, const id_type hash, const HashTypes... hashes) noexcept;

	template <same_as<id_type>... HashTypes>
	[[rythe_always_inline]] constexpr id_type
	combine_hash(id_type seed, const id_type hash, const HashTypes... hashes) noexcept;

	template <hash_mode Mode, typename T>
	[[rythe_always_inline]] constexpr id_type hash_value(const T& val) noexcept;

	template <typename T>
	[[rythe_always_inline]] constexpr id_type hash_value(const T& val) noexcept;

	template <typename T, hash_mode Mode>
	struct hash_strategy
	{
		[[rythe_always_inline]] constexpr static id_type hash(const T& val) noexcept
		{
			return hash_bytes<Mode>(byte_view::from_buffer(bit_cast<const byte*>(&val), sizeof(T)));
		}
	};

    template <string_like StringType, hash_mode Mode>
    struct hash_strategy<StringType, Mode>
    {
        [[rythe_always_inline]] constexpr static id_type hash(const StringType& val) noexcept
        {
            return hash_string<Mode>(view_from_stringish(val));
        }
    };

	template <typename T>
	struct hash
	{
		[[nodiscard]] [[rythe_always_inline]] constexpr id_type operator()(const T& val) const noexcept
		{
			return hash_strategy<T, hash_mode::default_hash>::hash(val);
		}
	};

	template <typename T>
	struct fast_hash
	{
		[[nodiscard]] [[rythe_always_inline]] constexpr id_type operator()(const T& val) const noexcept
		{
			return hash_strategy<T, hash_mode::fast_hash>::hash(val);
		}
	};

	template <typename T>
	struct protected_hash
	{
		[[nodiscard]] [[rythe_always_inline]] constexpr id_type operator()(const T& val) const noexcept
		{
			return hash_strategy<T, hash_mode::protected_hash>::hash(val);
		}
	};
} // namespace rsl

#include "hash.inl"
