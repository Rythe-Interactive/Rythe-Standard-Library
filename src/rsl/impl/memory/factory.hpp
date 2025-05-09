#pragma once

#include "../util/assert.hpp"
#include "../util/concepts.hpp"
#include "../util/type_traits.hpp"

namespace rsl
{
	template <typename T>
	concept factory_type = requires(T factory, void* mem, typename T::ptr_type ptr, size_type n)
	{
		{ factory.construct(mem, n) } -> convertible_to<typename T::ptr_type>;
		{ factory.move(mem, ptr, n) } -> convertible_to<typename T::ptr_type>;
		{ factory.destroy(ptr, n) } noexcept;
		{ factory.type_size() } noexcept -> convertible_to<size_type>;
		{ factory.trivial_copy() } noexcept -> convertible_to<bool>;
		{ factory.type_id() } noexcept -> convertible_to<id_type>;
		{ factory.is_valid() } noexcept -> convertible_to<bool>;
	};

	template <typename T>
	concept untyped_factory_type = factory_type<T> && same_as<typename T::ptr_type, void*>;

	template <typename T>
	concept typed_factory_type = factory_type<T> && requires(T factory) {
		{ factory.construct_single_inline() } -> convertible_to<typename T::value_type>;
	};

	template <typename T>
	concept noexcept_factory_type = factory_type<T> && requires(T factory, void* mem, typename T::ptr_type ptr, size_type n)
	{
		{ factory.construct(mem, n) } noexcept -> convertible_to<typename T::ptr_type>;
		{ factory.move(mem, ptr, n) } noexcept -> convertible_to<typename T::ptr_type>;
	} && (invert<typed_factory_type<T>> || requires(T factory) {
		{ factory.construct_single_inline() } noexcept -> convertible_to<typename T::value_type>;
	});

	template <typename T>
	concept statically_optional_typed_factory_type = requires
	{
		{ T::valid_factory } ->convertible_to<bool>;
	} && ( typed_factory_type<T> || (T::valid_factory == false));

	template <factory_type Factory>
	struct factory_traits
	{
		template <typename... Args>
		constexpr static bool noexcept_constructable =
			requires(Factory factory, void* mem, size_type n, Args&&... args)
			{
				{ factory.construct(mem, n, forward<Args>(args)...) } noexcept;
			};
		constexpr static bool noexcept_moveable =
			requires(Factory factory, void* mem, typename Factory::ptr_type ptr, size_type n)
			{
				{ factory.move(mem, ptr, n) } noexcept;
			};
	};

	template <typename T>
	class default_factory;

	template <constructible_at_all T>
	class default_factory<T>
	{
	public:
		using value_type = T;
		using ptr_type = T*;

		constexpr static bool valid_factory = true;
		[[rythe_always_inline]] constexpr bool is_valid() const noexcept { return valid_factory; } //NOLINT

		template <typename Other>
		using retarget = default_factory<Other>;

		constexpr default_factory() noexcept = default;

		template <typename Other>
		constexpr default_factory(default_factory<Other>) noexcept {} // NOLINT(*-explicit-constructor)

		template <typename... Args>
		T construct_single_inline(Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>);
		template <typename... Args>
		T* construct(void* ptr, size_type count, Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>);
		T* move(void* dst, T* src, size_type count) noexcept(is_nothrow_move_constructible_v<T>);
		void destroy(T* ptr, size_type count) noexcept;

		constexpr static size_type type_size() noexcept { return sizeof(T); }
		constexpr static bool trivial_copy() noexcept { return is_trivially_copyable_v<T>; }
		constexpr static id_type type_id() noexcept { return rsl::type_id<T>(); }
	};

	template<>
	class default_factory<void>
	{
	public:
		using value_type = void;
		using ptr_type = void*;

		constexpr static bool valid_factory = false;
		[[rythe_always_inline]] constexpr bool is_valid() const noexcept { return valid_factory; } //NOLINT

		template <typename Other>
		using retarget = default_factory<Other>;

		constexpr default_factory() noexcept = default;

		template <typename Other>
		constexpr default_factory(default_factory<Other>) noexcept { } // NOLINT(*-explicit-constructor)

		template <typename... Args>
		static void construct_single_inline(Args&&...) { rsl_assert_unreachable(); }
		template <typename... Args>
		static void* construct(void*, size_type, Args&&...) { rsl_assert_unreachable(); return nullptr; }
		static void* move(void*, void*, size_type) { rsl_assert_unreachable(); return nullptr; }
		static void destroy(void*, size_type) noexcept { rsl_assert_unreachable(); }

		constexpr static size_type type_size() noexcept { rsl_assert_unreachable(); return 0; }
		constexpr static bool trivial_copy() noexcept { rsl_assert_unreachable(); return false; }
		constexpr static id_type type_id() noexcept { rsl_assert_unreachable(); return 0; }
	};

	template <typename T>
		requires (!is_constructible_any_v<T>)
	class default_factory<T> : public default_factory<void> {};

	class polymorphic_factory
	{
	public:
		using ptr_type = void*;
		virtual ~polymorphic_factory() = default;

		virtual bool is_valid() const noexcept { return true; } // NOLINT

		virtual void* construct(void* ptr, size_type count) const = 0;
		virtual void* move(void* dst, void* src, size_type count) const = 0;
		virtual void destroy(void* ptr, size_type count) const noexcept = 0;
		[[nodiscard]] virtual size_type type_size() const noexcept = 0;
		[[nodiscard]] virtual bool trivial_copy() const noexcept = 0;
		[[nodiscard]] virtual id_type type_id() const noexcept = 0;
	};

	// Crucially does not qualify as a typed factory
	template <typename T>
	class typed_polymorphic_factory final : public polymorphic_factory
	{
	public:
		using ptr_type = void*;

		void* construct(void* ptr, size_type count) const override;
		void* move(void* dst, void* src, size_type count) const override;
		void destroy(void* ptr, size_type count) const noexcept override;
		[[nodiscard]] size_type type_size() const noexcept override;
		[[nodiscard]] bool trivial_copy() const noexcept override;
		[[nodiscard]] id_type type_id() const noexcept override;
	};

	class type_erased_factory
	{
	public:
		using ptr_type = void*;

		using construct_func = void* (*)(void* ptr, size_type count);
		using move_func = void* (*)(void* dst, void* src, size_type count);
		using destroy_func = void (*)(void* ptr, size_type count);

		[[rythe_always_inline]] bool is_valid() const noexcept;

		type_erased_factory() noexcept = default;
		template <typename T>
		[[rythe_always_inline]] type_erased_factory(construct_type_signal_type<T>) noexcept; // NOLINT(*-explicit-constructor)

		void* construct(void* ptr, size_type count) const;
		void* move(void* dst, void* src, size_type count) const;
		void destroy(void* ptr, size_type count) const noexcept;
		[[nodiscard]] size_type type_size() const noexcept;
		[[nodiscard]] bool trivial_copy() const noexcept;
		[[nodiscard]] id_type type_id() const noexcept;

	private:
		construct_func m_constructFunc = nullptr;
		move_func m_moveFunc = nullptr;
		destroy_func m_destroyFunc = nullptr;
		size_type m_typeSize = 0;
		bool m_triviallyCopyable = false;
		id_type m_typeId = invalid_id;
	};

	template <untyped_factory_type Factory>
	[[nodiscard]] [[rythe_always_inline]] bool can_trivially_copy(Factory& factory) noexcept; // NOLINT(*-redundant-declaration)

} // namespace rsl

#include "factory.inl"
