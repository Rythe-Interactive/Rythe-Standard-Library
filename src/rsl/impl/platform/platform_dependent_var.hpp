#pragma once

#include <rsl/type_util>
#include <rsl/utilities>

namespace rsl
{
	template <size_type PLATFORM_INDEX>
	struct platform_tag
	{
		constexpr static size_type index = PLATFORM_INDEX;
	};

	using windows_tag = platform_tag<RYTHE_WINDOWS_INDEX>;
	using linux_tag = platform_tag<RYTHE_LINUX_INDEX>;

	template <typename Tag, typename T>
	struct platform_specific_var
	{
		using value_type = T;
		constexpr static size_type index = Tag::index;
		T value;

		template<typename V>
		constexpr explicit platform_specific_var(V&& v)
			: value(rsl::forward<V>(v))
		{
		}

		constexpr platform_specific_var() = default;
	};

	template <typename T>
	struct windows_var : platform_specific_var<windows_tag, T>
	{
		using platform_specific_var<windows_tag, T>::platform_specific_var;
	};

	template <typename T>
	windows_var(T) -> windows_var<T>;

	template <typename T>
	struct linux_var : platform_specific_var<linux_tag, T>
	{
		using platform_specific_var<linux_tag, T>::platform_specific_var;
	};

	template <typename T>
	linux_var(T) -> linux_var<T>;

	namespace internal
	{
		template <typename T, typename... Others>
		struct is_same_value_type
		{
			constexpr static bool value = (std::is_same_v<typename T::value_type, typename Others::value_type> && ...);
		};
	} // namespace internal

	template <typename T, size_type MAX_PLATFORMS = RYTHE_MAX_PLATFORMS>
	struct platform_dependent_var
	{
		template <typename Var, typename... Vars>
			requires internal::is_same_value_type<Var, Vars...>::value
		constexpr platform_dependent_var(Var&& var, Vars&&... vars)
		{
			assign(var.index, var.value);
			(assign(vars.index, vars.value), ...);
		}

		constexpr platform_dependent_var() = default;

		using value_type = T;
		constexpr value_type get() const { return m_values[RYTHE_PLATFORM_INDEX]; }
		constexpr operator value_type() const { return get(); }


	private:
		constexpr void assign(size_type index, T value) { m_values[index] = value; }

		T m_values[MAX_PLATFORMS];
	};

	template <typename Var, typename... Vars>
	platform_dependent_var(Var&&, Vars&&...) -> platform_dependent_var<typename Var::value_type>;
} // namespace rsl
