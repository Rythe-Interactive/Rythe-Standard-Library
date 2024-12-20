#pragma once
#include "factory.hpp"

namespace rsl
{
	namespace internal
	{
		template <typename T, typename = void>
		struct memset_zero : false_type
		{
		};

		template <typename T>
		struct memset_zero<T, void_t<decltype(T::memset_zero)>> : bool_constant<T::memset_zero>
		{
		};
	} // namespace internal

	template <typename T>
	template <typename... Args>
	inline T* default_factory<T>::construct(void* ptr, size_type count, Args&&... args)
		noexcept(is_nothrow_constructible_v<T, Args...>)
	{
		if constexpr ((is_trivially_default_constructible_v<T>) && sizeof...(Args) == 0)
		{
			if constexpr (internal::memset_zero<T>::value)
			{
				std::memset(ptr, 0, count * sizeof(T));
			}

			return static_cast<T*>(ptr);
		}
		else
		{
			T* first = new (ptr) T(std::forward<Args>(args)...);

			for (size_type i = 1; i < count; i++)
			{
				new (first + i) T(std::forward<Args>(args)...);
			}

			return first;
		}
	}

	template <typename T>
	inline T* default_factory<T>::move(void* dst, T* src, size_type count) noexcept(is_nothrow_move_constructible_v<T>)
	{
		if constexpr (is_trivially_copy_constructible_v<T>)
		{
			std::memcpy(dst, src, count * sizeof(T));
			return static_cast<T*>(dst);
		}
		else
		{
			T* first = new (dst) T(std::move(src[0]));

			for (size_type i = 1; i < count; i++)
			{
				new (first + i) T(std::move(src[i]));
			}

			return first;
		}
	}

	template <typename T>
	inline void default_factory<T>::destroy(T* ptr, size_type count) noexcept
	{
		if constexpr (!is_trivially_destructible_v<T>)
		{
			for (size_type i = 0; i < count; i++)
			{
				(ptr + i)->~T();
			}
		}
	}
} // namespace rsl
