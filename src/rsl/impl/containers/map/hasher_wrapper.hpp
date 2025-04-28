#pragma once

namespace rsl
{
	template <typename T, typename = void>
	struct has_is_transparent : false_type
	{
	};

	template <typename T>
	struct has_is_transparent<T, void_t<typename T::is_transparent>> : true_type
	{
	};

	namespace internal
	{
		template <typename KeyType, typename Hasher, bool HasIsTransparent = has_is_transparent<Hasher>::value>
		struct hasher_wrapper;

		template <typename KeyType, typename Hasher>
		struct hasher_wrapper<KeyType, Hasher, false>
		{
			Hasher hasher{};

			[[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
			{
				id_type hash = hasher(val);
				hash *= 0xc4ceb9fe1a85ec53ull;
				hash ^= hash >> 33;
				return hash;
			}
		};

		template <typename KeyType, typename Hasher>
		struct hasher_wrapper<KeyType, Hasher, true>
		{
			using is_transparent = typename Hasher::is_transparent;

			Hasher hasher{};

			[[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
			{
				id_type hash = hasher(val);
				hash *= 0xc4ceb9fe1a85ec53ull;
				hash ^= hash >> 33;
				return hash;
			}
		};

		template <typename KeyType, typename T>
		struct hasher_wrapper<KeyType, ::rsl::hash<T>, false>
		{
			::rsl::hash<T> hasher{};

			[[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
			{
				return hasher(val);
			}
		};

		template <typename KeyType, typename T>
		struct hasher_wrapper<KeyType, fast_hash<T>, false>
		{
			fast_hash<T> hasher{};

			[[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
			{
				return hasher(val);
			}
		};

		template <typename KeyType, typename T>
		struct hasher_wrapper<KeyType, protected_hash<T>, false>
		{
			protected_hash<T> hasher{};

			[[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
			{
				return hasher(val);
			}
		};
	} // namespace internal
} // namespace rsl
