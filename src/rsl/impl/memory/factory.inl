#pragma once
#include "factory.hpp"

namespace rsl
{
	namespace internal
	{
		template <typename T>
		struct memset_zero : false_type
		{
		};

		template <typename T>
			requires requires{ T::memset_zero; }
		struct memset_zero<T> : bool_constant<T::memset_zero>
		{
		};

		template <typename T, typename... Args>
		[[rythe_always_inline]] T default_construct_single_inline(Args&&... args)
			noexcept(noexcept(default_factory<T>{}.construct_single_inline(forward<Args>(args)...)))
		{
			return default_factory<T>{}.construct_single_inline(forward<Args>(args)...);
		}

		template <typename T, typename... Args>
		[[rythe_always_inline]] void* default_construct(void* ptr, size_type count, Args&&... args)
			noexcept(noexcept(default_factory<T>{}.construct(ptr, count, forward<Args>(args)...)))
		{
			return default_factory<T>{}.construct(ptr, count, forward<Args>(args)...);
		}

		template <typename T>
		[[rythe_always_inline]] void* default_move(void* dst, void* src, size_type count)
			noexcept(noexcept(default_factory<T>{}.move(dst, static_cast<T*>(src), count)))
		{
			return default_factory<T>{}.move(dst, static_cast<T*>(src), count);
		}

		template <typename T>
		[[rythe_always_inline]] void default_destroy(void* ptr, size_type count)
			noexcept(noexcept(default_factory<T>{}.destroy(static_cast<T*>(ptr), count)))
		{
			default_factory<T>{}.destroy(static_cast<T*>(ptr), count);
		}
	} // namespace internal

	template <typename T>
	template <typename ... Args>
	T default_factory<T>::construct_single_inline(Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>)
	{
		return T(rsl::forward<Args>(args)...);
	}

	template <typename T>
	template <typename... Args>
	inline T* default_factory<T>::construct(void* ptr, const size_type count, Args&&... args)
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
			T* first = new (ptr) T(rsl::forward<Args>(args)...);

			for (size_type i = 1; i < count; i++)
			{
				new (first + i) T(rsl::forward<Args>(args)...);
			}

			return first;
		}
	}

	template <typename T>
	inline T* default_factory<T>::move(void* dst, T* src, const size_type count) noexcept(is_nothrow_move_constructible_v<T>)
	{
		if constexpr (is_trivially_copy_constructible_v<T>)
		{
			std::memcpy(dst, src, count * sizeof(T));

			if constexpr (internal::memset_zero<T>::value)
			{
				std::memset(src, 0, count * sizeof(T));
			}

			return static_cast<T*>(dst);
		}
		else
		{
			T* first = new (dst) T(rsl::move(src[0]));

			for (size_type i = 1; i < count; i++)
			{
				new (first + i) T(rsl::move(src[i]));
			}

			return first;
		}
	}

	template <typename T>
	inline void default_factory<T>::destroy(T* ptr, const size_type count) noexcept
	{
		if constexpr (!is_trivially_destructible_v<T>)
		{
			for (size_type i = 0; i < count; i++)
			{
				(ptr + i)->~T();
			}
		}
	}

	template <typename T>
	inline void* typed_polymorphic_factory<T>::construct(void* ptr, const size_type count) const
	{
		return internal::default_construct<T>(ptr, count);
	}

	template <typename T>
	inline void* typed_polymorphic_factory<T>::move(void* dst, void* src, const size_type count) const
	{
		return internal::default_move<T>(dst, src, count);
	}

	template <typename T>
	inline void typed_polymorphic_factory<T>::destroy(void* ptr, const size_type count) const noexcept
	{
		internal::default_destroy<T>(ptr, count);
	}

	template <typename T>
	inline size_type typed_polymorphic_factory<T>::type_size() const noexcept
	{
		return sizeof(T);
	}

	template <typename T>
	inline bool typed_polymorphic_factory<T>::trivial_copy() const noexcept
	{
		return is_trivially_copyable_v<T>;
	}

	template <typename T>
	inline id_type typed_polymorphic_factory<T>::type_id() const noexcept
	{
		return rsl::type_id<T>();
	}

	template <typename T>
	inline type_erased_factory::type_erased_factory(construct_type_signal_type<T>) noexcept
		: m_constructFunc(&internal::default_construct<T>),
		  m_moveFunc(&internal::default_move<T>),
		  m_destroyFunc(&internal::default_destroy<T>),
		  m_typeSize(sizeof(T)),
		  m_triviallyCopyable(is_trivially_copyable_v<T>),
		  m_typeId(rsl::type_id<T>())
	{
	}

	template <untyped_factory_type Factory>
	bool can_trivially_copy(Factory& factory) noexcept
	{
		if constexpr (requires(Factory& f) {
						  { f.trivialCopy() } noexcept -> convertible_to<bool>;
					  })
		{
			return factory.trivialCopy();
		}
		else
		{
			return false;
		}
	}
} // namespace rsl
