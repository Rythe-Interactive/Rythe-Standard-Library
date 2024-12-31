#pragma once
#include <vector>

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
		using base = delegate_base<ReturnType(ParamTypes...), Alloc, Factory>;
		using typed_alloc_type = typename base::typed_alloc_type;

	public:
		using return_type = ReturnType;
		using param_types = type_sequence<ParamTypes...>;
		using invocation_element = typename base::invocation_element;
		using allocator_storage_type = typename base::allocator_storage_type;
		using allocator_t = typename base::allocator_t;
		using factory_storage_type = typename base::factory_storage_type;
		using factory_t = typename base::factory_t;

	private:
		using stub_type = typename base::stub_type;

		friend class multicast_delegate<ReturnType(ParamTypes...)>;
		invocation_element m_invocation;

		constexpr delegate(invocation_element&& e)
			: m_invocation(e)
		{
		}

	public:
		constexpr delegate() = default;

		template <invocable<ReturnType(ParamTypes...)> Functor>
		constexpr delegate(const Functor& instance)
			: m_invocation(base::template create_element<Functor>(instance))
		{
		}

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		constexpr delegate(const Functor& instance)
			: m_invocation(base::template create_element<Functor>(instance))
		{
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		constexpr static delegate create(T& instance)
		{
			return delegate(base::template create_element<T, TMethod>(instance));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		constexpr static delegate create(const T& instance)
		{
			return delegate(base::template create_element<T, TMethod>(instance));
		}

		template <ReturnType (*TMethod)(ParamTypes...)>
		constexpr static delegate create()
		{
			return delegate(base::template create_element<TMethod>());
		}

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		constexpr static delegate create(const Functor& instance)
		{
			return delegate(base::template create_element<Functor>(instance));
		}

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
			m_invocation = base::template create_element<T, TMethod>(instance);
			return *this;
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		constexpr delegate& assign(const T& instance)
		{
			m_invocation = base::template create_element<T, TMethod>(instance);
			return *this;
		}

		template <ReturnType (*TMethod)(ParamTypes...)>
		constexpr delegate& assign()
		{
			m_invocation = base::template create_element<TMethod>();
			return *this;
		}

		constexpr delegate& operator=(const delegate&) = default;

		template <invocable<ReturnType(ParamTypes...)> Functor>
		constexpr delegate& operator=(const Functor& instance)
		{
			m_invocation = base::template create_element<Functor>(instance);
			return *this;
		}

		constexpr ReturnType operator()(ParamTypes... args) const { return invoke(args...); }

		constexpr ReturnType invoke(ParamTypes... args) const
		{
			return (*m_invocation.stub)(*m_invocation.object, args...);
		}
	};
} // namespace rsl
