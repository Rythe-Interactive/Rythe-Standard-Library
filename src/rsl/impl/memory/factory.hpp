#pragma once

#include "../util/concepts.hpp"
#include "../util/type_traits.hpp"

namespace rsl
{
	template <typename T>
	concept factory_type = requires(T factory, void* mem, typename T::ptr_type ptr, size_type n) {
		{ factory.construct(mem, n) } -> convertible_to<typename T::ptr_type>;
		{ factory.move(mem, ptr, n) } -> convertible_to<typename T::ptr_type>;
		{ factory.destroy(ptr, n) } noexcept;
	};

	template <typename T>
	concept noexcept_factory_type = requires(T factory, void* mem, typename T::ptr_type ptr, size_type n) {
		{ factory.construct(mem, n) } noexcept -> convertible_to<typename T::ptr_type>;
		{ factory.move(mem, ptr, n) } noexcept -> convertible_to<typename T::ptr_type>;
		{ factory.destroy(ptr, n) } noexcept;
	};

	template <typename T>
	concept untyped_factory_type = factory_type<T> && requires(T factory) {
		{ factory.typeSize() } noexcept -> convertible_to<size_type>;
	};


	template <typename T>
	concept typed_factory_type = factory_type<T> && not_same_as<typename T::ptr_type, void*>;

	template <factory_type Factory>
	struct factory_traits
	{
		template <typename... Args>
		constexpr static bool noexcept_constructable =
			requires(Factory factory, void* mem, size_type n, Args&&... args) {
				{ factory.construct(mem, n, forward<Args>(args)...) } noexcept;
			};
		constexpr static bool noexcept_moveable =
			requires(Factory factory, void* mem, typename Factory::ptr_type ptr, size_type n) {
				{ factory.move(mem, ptr, n) } noexcept;
			};
	};

	template <typename T>
	class default_factory
	{
	public:
		using value_type = T;
		using ptr_type = T*;

		template <typename Other>
		using retarget = default_factory<Other>;

		constexpr default_factory() noexcept = default;

		template <typename Other>
		constexpr default_factory(default_factory<Other>) noexcept
		{
		}

		template <typename... Args>
		T* construct(void* ptr, size_type count, Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>);
		T* move(void* dst, T* src, size_type count) noexcept(is_nothrow_move_constructible_v<T>);
		void destroy(T* ptr, size_type count) noexcept;
	};

	class polymorphic_factory
	{
	public:
		using ptr_type = void*;

		virtual void* construct(void* ptr, size_type count) const = 0;
		virtual void* move(void* dst, void* src, size_type count) const = 0;
		virtual void destroy(void* ptr, size_type count) const noexcept = 0;
		virtual size_type typeSize() const noexcept = 0;
		virtual bool trivialCopy() const noexcept = 0;
		virtual id_type typeId() const noexcept = 0;
	};

	template <typename T>
	class typed_polymorphic_factory final : public polymorphic_factory
	{
	public:
		using ptr_type = void*;

		void* construct(void* ptr, size_type count) const override;
		void* move(void* dst, void* src, size_type count) const override;
		void destroy(void* ptr, size_type count) const noexcept override;
		size_type typeSize() const noexcept override;
		bool trivialCopy() const noexcept override;
		id_type typeId() const noexcept override;
	};

	class type_erased_factory
	{
	public:
		using ptr_type = void*;

		using construct_func = void* (*)(void* ptr, size_type count);
		using move_func = void* (*)(void* dst, void* src, size_type count);
		using destroy_func = void (*)(void* ptr, size_type count);

		type_erased_factory() noexcept = default;
		template <typename T>
		[[rythe_always_inline]] type_erased_factory(construct_type_signal_type<T>) noexcept;

		void* construct(void* ptr, size_type count) const;
		void* move(void* dst, void* src, size_type count) const;
		void destroy(void* ptr, size_type count) const noexcept;
		[[nodiscard]] size_type typeSize() const noexcept;
		[[nodiscard]] bool trivialCopy() const noexcept;
		[[nodiscard]] id_type typeId() const noexcept;

	private:
		construct_func m_constructFunc = nullptr;
		move_func m_moveFunc = nullptr;
		destroy_func m_destroyFunc = nullptr;
		size_type m_typeSize = 0;
		bool m_triviallyCopyable = false;
		id_type m_typeId = invalid_id;
	};

	template <untyped_factory_type Factory>
	[[nodiscard]] [[rythe_always_inline]] bool can_trivially_copy(Factory& factory) noexcept;

} // namespace rsl

#include "factory.inl"
