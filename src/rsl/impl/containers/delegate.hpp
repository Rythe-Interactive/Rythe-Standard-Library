#pragma once

#include "../util/concepts.hpp"
#include "delegate_base.hpp"

namespace rsl
{
	template <
		typename FuncSig, allocator_type Alloc = default_allocator, untyped_factory_type Factory = type_erased_factory>
	class delegate;

	template <
		typename FuncSig, allocator_type Alloc = default_allocator, untyped_factory_type Factory = type_erased_factory>
	class multicast_delegate;

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	class delegate<ReturnType(ParamTypes...), Alloc, Factory> final :
		private delegate_base<ReturnType(ParamTypes...), Alloc, Factory>
	{
		friend class multicast_delegate<ReturnType(ParamTypes...)>;

		using base = delegate_base<ReturnType(ParamTypes...), Alloc, Factory>;
		using typed_alloc_type = typename base::typed_alloc_type;
		using stub_type = typename base::stub_type;

	public:
		using return_type = ReturnType;
		using param_types = type_sequence<ParamTypes...>;
		using invocation_element = typename base::invocation_element;
		using allocator_storage_type = typename base::allocator_storage_type;
		using allocator_t = typename base::allocator_t;
		using factory_storage_type = typename base::factory_storage_type;
		using factory_t = typename base::factory_t;

		[[rythe_always_inline]] constexpr delegate() = default;

		[[rythe_always_inline]] explicit constexpr delegate(const allocator_storage_type& allocStorage);

		template <invocable<ReturnType(ParamTypes...)> Functor>
		[[rythe_always_inline]] constexpr delegate(const Functor& instance);

		template <invocable<ReturnType(ParamTypes...)> Functor>
		[[rythe_always_inline]] constexpr delegate(const allocator_storage_type& allocStorage, const Functor& instance);

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		[[rythe_always_inline]] constexpr delegate(const Functor& instance);

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		[[rythe_always_inline]] constexpr delegate(const allocator_storage_type& allocStorage, const Functor& instance);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[nodiscard]] [[rythe_always_inline]] constexpr static delegate create(T& instance);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[nodiscard]] [[rythe_always_inline]] constexpr static delegate
		create(const allocator_storage_type& alloc, T& instance);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[nodiscard]] [[rythe_always_inline]] constexpr static delegate create(const T& instance);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[nodiscard]] [[rythe_always_inline]] constexpr static delegate
		create(const allocator_storage_type& alloc, const T& instance);

		template <ReturnType (*TMethod)(ParamTypes...)>
		[[nodiscard]] [[rythe_always_inline]] constexpr static delegate create();

		template <ReturnType (*TMethod)(ParamTypes...)>
		[[nodiscard]] [[rythe_always_inline]] constexpr static delegate create(const allocator_storage_type& alloc);

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		[[nodiscard]] [[rythe_always_inline]] constexpr static delegate create(const Functor& instance);

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		[[nodiscard]] [[rythe_always_inline]] constexpr static delegate
		create(const allocator_storage_type& alloc, const Functor& instance);

		[[rythe_always_inline]] constexpr void set_allocator(const allocator_storage_type& allocStorage)
			noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>);

		[[nodiscard]] [[rythe_always_inline]] constexpr allocator_t& get_allocator() noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr const allocator_t& get_allocator() const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr bool empty() const noexcept;
		[[rythe_always_inline]] constexpr void clear() noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(nullptr_type) const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(nullptr_type) const noexcept;

		[[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(const delegate&) const noexcept = default;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(const delegate&) const noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr bool
		operator==(const multicast_delegate<ReturnType(ParamTypes...)>& other) const noexcept;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool
		operator!=(const multicast_delegate<ReturnType(ParamTypes...)>& other) const noexcept;

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr delegate& assign(T& instance);

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] constexpr delegate& assign(const T& instance);

		template <ReturnType (*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr delegate& assign();

		template <invocable<ReturnType(ParamTypes...)> Functor>
		[[rythe_always_inline]] constexpr delegate& operator=(const Functor& instance);
		[[rythe_always_inline]] constexpr delegate& operator=(const delegate&) = default;

		[[rythe_always_inline]] constexpr ReturnType operator()(ParamTypes... args) const;
		[[rythe_always_inline]] constexpr ReturnType invoke(ParamTypes... args) const;

	private:
		[[rythe_always_inline]] constexpr delegate(const allocator_storage_type& allocStorage, invocation_element&& e);

		allocator_storage_type m_alloc;
		invocation_element m_invocation;
	};
} // namespace rsl

#include "delegate.inl"
