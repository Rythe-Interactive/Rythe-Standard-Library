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

		[[rythe_always_inline]] explicit delegate(const allocator_storage_type& allocStorage)
			: m_alloc(allocStorage)
		{
		}

		template <invocable<ReturnType(ParamTypes...)> Functor>
		constexpr delegate(const Functor& instance)
			: m_invocation(base::template create_element<Functor>(instance))
		{
		}

		template <invocable<ReturnType(ParamTypes...)> Functor>
		constexpr delegate(const allocator_storage_type& allocStorage, const Functor& instance)
			: m_alloc(allocStorage),
			  m_invocation(base::template create_element<Functor>(instance))
		{
		}

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		constexpr delegate(const Functor& instance)
			: m_invocation(base::template create_element<Functor>(instance))
		{
		}

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		constexpr delegate(const allocator_storage_type& allocStorage, const Functor& instance)
			: m_alloc(allocStorage),
			  m_invocation(base::template create_element<Functor>(instance))
		{
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		constexpr static delegate create(T& instance)
		{
			return delegate(base::template create_element<T, TMethod>(allocator_storage_type{}, instance));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		constexpr static delegate create(const allocator_storage_type& alloc, T& instance)
		{
			return delegate(base::template create_element<T, TMethod>(alloc, instance));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		constexpr static delegate create(const T& instance)
		{
			return delegate(base::template create_element<T, TMethod>(allocator_storage_type{}, instance));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		constexpr static delegate create(const allocator_storage_type& alloc, const T& instance)
		{
			return delegate(base::template create_element<T, TMethod>(alloc, instance));
		}

		template <ReturnType (*TMethod)(ParamTypes...)>
		constexpr static delegate create()
		{
			return delegate(base::template create_element<TMethod>(allocator_storage_type{}));
		}

		template <ReturnType (*TMethod)(ParamTypes...)>
		constexpr static delegate create(const allocator_storage_type& alloc)
		{
			return delegate(base::template create_element<TMethod>(alloc));
		}

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		constexpr static delegate create(const Functor& instance)
		{
			return delegate(base::template create_element<Functor>(allocator_storage_type{}, instance));
		}

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		constexpr static delegate create(const allocator_storage_type& alloc, const Functor& instance)
		{
			return delegate(base::template create_element<Functor>(alloc, instance));
		}

		void set_allocator(const allocator_storage_type& allocStorage)
			noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>)
		{
			m_alloc = allocStorage;
		}

		allocator_t& get_allocator() noexcept { return *m_alloc; }
		const allocator_t& get_allocator() const noexcept { return *m_alloc; }

		constexpr bool empty() const { return m_invocation.stub == nullptr; }
		constexpr void clear() { m_invocation = invocation_element(); }

		constexpr bool operator==(nullptr_type) const { return empty(); }
		constexpr bool operator!=(nullptr_type) const { return !empty(); }

		constexpr bool operator==(const delegate&) const = default;
		constexpr bool operator!=(const delegate&) const = default;

		constexpr bool operator==(const multicast_delegate<ReturnType(ParamTypes...)>& other) const
		{
			return other == (*this);
		}
		constexpr bool operator!=(const multicast_delegate<ReturnType(ParamTypes...)>& other) const
		{
			return other != (*this);
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		constexpr delegate& assign(T& instance)
		{
			m_invocation = base::template create_element<T, TMethod>(m_alloc, instance);
			return *this;
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		constexpr delegate& assign(const T& instance)
		{
			m_invocation = base::template create_element<T, TMethod>(m_alloc, instance);
			return *this;
		}

		template <ReturnType (*TMethod)(ParamTypes...)>
		constexpr delegate& assign()
		{
			m_invocation = base::template create_element<TMethod>(m_alloc);
			return *this;
		}

		constexpr delegate& operator=(const delegate&) = default;

		template <invocable<ReturnType(ParamTypes...)> Functor>
		constexpr delegate& operator=(const Functor& instance)
		{
			m_invocation = base::template create_element<Functor>(m_alloc, instance);
			return *this;
		}

		constexpr ReturnType operator()(ParamTypes... args) const { return invoke(args...); }

		constexpr ReturnType invoke(ParamTypes... args) const
		{
			return (*m_invocation.stub)(*m_invocation.object, args...);
		}

	private:
		constexpr delegate(invocation_element&& e)
			: m_invocation(e)
		{
		}

		allocator_storage_type m_alloc;
		invocation_element m_invocation;
	};
} // namespace rsl

#include "delegate.inl"
