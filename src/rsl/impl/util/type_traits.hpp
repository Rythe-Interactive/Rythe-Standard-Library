#pragma once

#include "type_util.hpp"

namespace rsl
{
	namespace internal
	{
		template <typename T>
		struct get_first_parameter;

		template <template <typename, typename...> typename T, typename First, typename... Rest>
		struct get_first_parameter<T<First, Rest...>>
		{
			using type = First;
		};

		template <typename Newfirst, typename T>
		struct replace_first_parameter;

		template <typename Newfirst, template <typename, typename...> typename T, typename First, typename... Rest>
		struct replace_first_parameter<Newfirst, T<First, Rest...>>
		{
			using type = T<Newfirst, Rest...>;
		};

		template <typename T, typename Other, typename = void>
		struct get_rebind_alias
		{
			using type = typename replace_first_parameter<Other, T>::type;
		};

		template <typename T, typename Other>
		struct get_rebind_alias<T, Other, void_t<typename T::template rebind<Other>>>
		{
			using type = typename T::template rebind<Other>;
		};

		template <typename T, typename Elem>
		struct ptr_traits_base
		{
			using ptr_type = T;
			using element_type = Elem;
			using difference_type = typename internal::get_first_parameter<T>::type;

			template <typename Other>
			using rebind = typename internal::get_rebind_alias<T, Other>::type;

			[[nodiscard]] static constexpr ptr_type pointer_to(conditional_t<is_void_v<Elem>, char, Elem>& val)
				noexcept(noexcept(T::pointer_to(val))) /* strengthened */
			{                                          // Per LWG-3454
				return T::pointer_to(val);
			}
		};

		template <typename, typename = void, typename = void>
		struct ptr_traits_sfinae_layer
		{
		};

		template <typename T, typename _Uty>
		struct ptr_traits_sfinae_layer<T, _Uty, void_t<typename internal::get_first_parameter<T>::type>> :
			ptr_traits_base<T, typename internal::get_first_parameter<T>::type>
		{
		};

		template <typename T>
		struct ptr_traits_sfinae_layer<T, void_t<typename T::element_type>, void> :
			ptr_traits_base<T, typename T::element_type>
		{
		};
	} // namespace internal

	template <typename T>
	struct pointer_traits : internal::ptr_traits_sfinae_layer<T>
	{
	};

	template <typename T>
	struct pointer_traits<T*>
	{
		using ptr_type = T*;
		using element_type = T;
		using difference_type = ptrdiff_t;

		template <typename Other>
		using rebind = Other*;

		[[nodiscard]] static constexpr ptr_type pointer_to(conditional_t<is_void_v<T>, char, T>& val) noexcept
		{
			return addressof(val);
		}
	};

	namespace internal
	{
		template <typename T, typename = void, typename = void>
		struct get_ptr_type
		{
			using type = add_pointer_t<typename T::value_type>;
		};

		template <typename T, typename U>
		struct get_ptr_type<T, U, void_t<typename T::ptr_type>>
		{
			using type = typename T::ptr_type;
		};

		template <typename T>
		struct get_ptr_type<T, void_t<typename T::pointer>>
		{
			using type = typename T::pointer;
		};

		template <typename T, typename = void, typename = void>
		struct get_const_ptr_type
		{
			using type = decorate_type_t<typename T::value_type, const_signal, pointer_signal>;
		};

		template <typename T, typename U>
		struct get_const_ptr_type<T, U, void_t<typename T::const_ptr_type>>
		{
			using type = typename T::const_ptr_type;
		};

		template <typename T>
		struct get_const_ptr_type<T, void_t<typename T::const_pointer>>
		{
			using type = typename T::const_pointer;
		};

		template <typename T, typename = void, typename = void>
		struct get_void_ptr_type
		{
			using type = decorate_type_t<typename T::value_type, const_signal, pointer_signal>;
		};

		template <typename T, typename U>
		struct get_void_ptr_type<T, U, void_t<typename T::void_ptr_type>>
		{
			using type = typename T::void_ptr_type;
		};

		template <typename T>
		struct get_void_ptr_type<T, void_t<typename T::void_pointer>>
		{
			using type = typename T::void_pointer;
		};

		template <typename T, typename = void, typename = void>
		struct get_const_void_ptr_type
		{
			using type = typename pointer_traits<typename get_ptr_type<T>::type>::template rebind<const void>;
		};

		template <typename T, typename U>
		struct get_const_void_ptr_type<T, U, void_t<typename T::const_void_ptr_type>>
		{
			using type = typename T::const_void_ptr_type;
		};

		template <typename T>
		struct get_const_void_ptr_type<T, void_t<typename T::const_void_pointer>>
		{
			using type = typename T::const_void_pointer;
		};

		template <typename T, typename Other, typename = void>
		struct get_rebind_type
		{
			using type = typename replace_first_parameter<Other, T>::type;
		};

		template <typename T, typename Other>
		struct get_rebind_type<T, Other, void_t<typename T::template rebind<Other>::other>>
		{
			using type = typename T::template rebind<Other>::other;
		};

		RYTHE_HAS_FUNC(allocate_hint)
		RYTHE_HAS_FUNC(allocate_at_least)
		RYTHE_HAS_FUNC(construct)
		RYTHE_HAS_FUNC(destroy)
		RYTHE_HAS_FUNC(max_size)
	} // namespace internal

	template <typename T>
	struct allocation_result
	{
		T ptr;
		size_type count;
	};

	template <typename Alloc>
	struct allocator_traits
	{
		using allocator_type = Alloc;
		using value_type = typename Alloc::value_type;

		using ptr_type = typename internal::get_ptr_type<Alloc>::type;
		using const_ptr = typename internal::get_const_ptr_type<Alloc>::type;
		using void_ptr_type = typename internal::get_void_ptr_type<Alloc>::type;
		using const_void_ptr = typename internal::get_const_void_ptr_type<Alloc>::type;

		using is_always_equal = is_always_equal_t<Alloc>;

		template <typename Other>
		using rebind_alloc = typename internal::get_rebind_type<Alloc, Other>::type;

		template <typename Other>
		using rebind_traits = allocator_traits<rebind_alloc<Other>>;

		[[nodiscard]] static constexpr ptr_type allocate(Alloc& alloc, const size_type count)
		{
			return alloc.allocate(count);
		}

		[[nodiscard]] static constexpr ptr_type
		allocate(Alloc& alloc, const size_type count, [[maybe_unused]] const const_void_ptr hint)
		{
			if constexpr (internal::has_allocate_hint_v<Alloc, ptr_type(size_type, const_void_ptr)>)
			{
				return alloc.allocate(count, hint);
			}
			else
			{
				return alloc.allocate(count);
			}
		}

		[[nodiscard]] static constexpr allocation_result<ptr_type>
		allocate_at_least(Alloc& alloc, const size_type count)
		{
			if constexpr (internal::has_allocate_at_least_v<Alloc, allocation_result<ptr_type>(size_type)>)
			{
				return alloc.allocate_at_least(count);
			}
			else
			{
				return {alloc.allocate(count), count};
			}
		}

		static constexpr void deallocate(Alloc& alloc, ptr_type ptr, size_type count) { alloc.deallocate(ptr, count); }

		template <typename T, typename... Args>
		static constexpr void construct(Alloc& alloc, T* ptr, Args&&... args)
		{
			if constexpr (internal::has_construct_v<Alloc, void(T*, Args...)>)
			{
				alloc.construct(ptr, ::rsl::forward<Args>(args)...);
			}
			else
			{
				::rsl::construct_at(ptr, ::rsl::forward<Args>(args)...);
			}
		}

		template <typename T>
		static constexpr void destroy(Alloc& alloc, T* ptr)
		{
			if constexpr (internal::has_destroy_v<Alloc, void(T*)>)
			{
				alloc.destroy(ptr);
			}
			else
			{
				::rsl::destroy_at(ptr);
			}
		}

		[[nodiscard]] static constexpr size_type max_size(const Alloc& alloc) noexcept
		{
			if constexpr (internal::has_max_size_v<Alloc, void()>)
			{
				return alloc.max_size();
			}
			else
			{
				return (::std::numeric_limits<size_type>::max)() / sizeof(value_type);
			}
		}
	};
} // namespace rsl
