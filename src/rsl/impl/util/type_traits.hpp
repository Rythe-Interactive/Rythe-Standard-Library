#pragma once

#include "../containers/constexpr_string.hpp"
#include "hash.hpp"
#include "type_util.hpp"

namespace rsl
{
	namespace internal
	{
		template <typename T>
		consteval auto compiler_dependent_type_name() noexcept
		{
			string_view functionName = __RYTHE_FULL_FUNC__;

			constexpr_string<constexpr_strlen(__RYTHE_FULL_FUNC__) + 1> ret{};
#if defined(RYTHE_MSVC)
			auto first = functionName.find_first_of('<') + 1;
			auto end = functionName.find_last_of('>');
			if (auto t = functionName.find_first_of(' ', first) + 1; t < end)
			{
				first = t;
			}

			ret.copy_from(functionName.substr(first, end - first));
#elif defined(RYTHE_GCC)
			auto first = functionName.find_first_not_of(' ', functionName.find_first_of('=') + 1);
			ret.copy_from(functionName.substr(first, functionName.find_last_of(']') - first));
#elif defined(RYTHE_CLANG)
			auto first = functionName.find_first_not_of(' ', functionName.find_first_of('=') + 1);
			ret.copy_from(functionName.substr(first, functionName.find_last_of(']') - first));
			constexpr_string refFilter = " &";
			constexpr_string refReplace = "&";
			ret = ret.replace(refFilter, refReplace);
			constexpr_string ptrFilter = " *";
			constexpr_string ptrReplace = "*";
			ret = ret.replace(ptrFilter, ptrReplace);
#else
			ret.copy_from(functionName);
#endif

			return ret;
		}

		template <template <typename...> typename T>
		consteval auto compiler_dependent_templated_type_name() noexcept
		{
			string_view functionName = __RYTHE_FULL_FUNC__;

			constexpr_string<constexpr_strlen(__RYTHE_FULL_FUNC__) + 1> ret{};
#if defined(RYTHE_MSVC)
			auto first = functionName.find_first_of('<') + 1;
			auto end = functionName.find_last_of('>');
			if (auto t = functionName.find_first_of(' ', first) + 1; t < end)
			{
				first = t;
			}

			ret.copy_from(functionName.substr(first, end - first));
#elif defined(RYTHE_GCC)
			auto first = functionName.find_first_not_of(' ', functionName.find_first_of('=') + 1);
			ret.copy_from(functionName.substr(first, functionName.find_last_of(']') - first));
			constexpr_string cxxfilter = "__cxx11::";
			ret = ret.filter(cxxfilter);
#elif defined(RYTHE_CLANG)
			auto first = functionName.find_first_not_of(' ', functionName.find_first_of('=') + 1);
			ret.copy_from(functionName.substr(first, functionName.find_last_of(']') - first));
#else
			ret.copy_from(functionName);
#endif

			return ret;
		}

		template <typename T>
		struct compose_type_name
		{
			consteval static auto get_value() noexcept
			{
				constexpr auto ret = compiler_dependent_type_name<T>();
				return ret.template refit<ret.size() + 1>();
			}
		};

		template <template <typename...> typename T, typename... Args>
		struct compose_type_name<T<Args...>>
		{
			template <typename A, typename... As, size_type N>
			consteval static auto add_types(constexpr_string<N> original)
			{
				auto ret = original + compose_type_name<A>::get_value();

				if constexpr (sizeof...(As) != 0)
				{
					return add_types<As...>(ret + constexpr_string(", "));
				}
				else
				{
					return ret;
				}
			}

			consteval static auto construct_value() noexcept
			{
				auto a = compiler_dependent_templated_type_name<T>();
				auto b = a + constexpr_string("<");
				auto c = add_types<Args...>(b) + constexpr_string(">");

				return c;
			}

			consteval static auto get_value() noexcept
			{
				constexpr auto ret = construct_value();
				return ret.template refit<ret.size() + 1>();
			}
		};

	} // namespace internal

	template <typename T, size_type N>
	consteval constexpr_string<N> type_name() noexcept;

	template <>
	consteval constexpr_string<5> type_name<void>() noexcept
	{
		return "void";
	}

	template <typename T>
	consteval auto type_name() noexcept
	{
		return internal::compose_type_name<remove_cvr_t<T>>::get_value();
	}

	template <typename T>
	consteval id_type type_id() noexcept
	{
		return hash_string(std::string_view(type_name<T>()));
	}

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
			using difference_type = typename get_first_parameter<T>::type;

			template <typename Other>
			using rebind = typename get_rebind_alias<T, Other>::type;

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
		struct ptr_traits_sfinae_layer<T, _Uty, void_t<typename get_first_parameter<T>::type>> :
			ptr_traits_base<T, typename get_first_parameter<T>::type>
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
