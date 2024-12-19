#pragma once

#include "../util/concepts.hpp"

namespace rsl
{
	template <typename T>
	concept factory_type = requires(T factory, void* mem, typename T::value_type* ptr, size_type n) {
		{ factory.construct(mem, n) } -> convertible_to<typename T::value_type*>;
		{ factory.move(mem, ptr, n) } -> convertible_to<typename T::value_type*>;
		{ factory.destroy(ptr, n) } noexcept;
	};

	template <typename T>
	concept noexcept_factory_type = requires(T factory, void* mem, typename T::value_type* ptr, size_type n) {
		{ factory.construct(mem, n) } noexcept -> convertible_to<typename T::value_type*>;
		{ factory.move(mem, ptr, n) } noexcept -> convertible_to<typename T::value_type*>;
		{ factory.destroy(ptr, n) } noexcept;
	};

	template <factory_type Factory>
	struct factory_traits
	{
		template <typename... Args>
		constexpr static bool noexcept_constructable =
			requires(Factory factory, void* mem, size_type n, Args&&... args) {
				{ factory.construct(mem, n, forward<Args>(args)...) } noexcept;
			};
		constexpr static bool noexcept_moveable =
			requires(Factory factory, void* mem, typename Factory::value_type* ptr, size_type n) {
				{ factory.move(mem, ptr, n) } noexcept;
			};
	};

	template <typename T>
	class default_factory
	{
	public:
		using value_type = T;

		template <typename Other>
		using retarget = default_factory<Other>;

		template <typename... Args>
		T* construct(void* ptr, size_type count, Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>);
		T* move(void* dst, T* src, size_type count) noexcept(is_nothrow_move_constructible_v<T>);
		void destroy(T* ptr, size_type count) noexcept;
	};
} // namespace rsl

#include "factory.inl"
