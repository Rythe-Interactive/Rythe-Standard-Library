#pragma once
#include "delegate.hpp"

namespace rsl
{
	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>::delegate(
		const allocator_storage_type& allocStorage
	)
		: m_alloc(allocStorage),
		  m_invocation()
	{
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <invocable<ReturnType(ParamTypes...)> Functor>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>::delegate(const Functor& instance)
		: m_alloc(),
		  m_invocation(base::template create_element<Functor>(m_alloc, instance))
	{
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <invocable<ReturnType(ParamTypes...)> Functor>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>::delegate(
		const allocator_storage_type& allocStorage, const Functor& instance
	)
		: m_alloc(allocStorage),
		  m_invocation(base::template create_element<Functor>(m_alloc, instance))
	{
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <functor Functor>
		requires invocable<Functor, ReturnType(ParamTypes...)>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>::delegate(const Functor& instance)
		: m_alloc(),
		  m_invocation(base::template create_element<Functor>(m_alloc, instance))
	{
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <functor Functor>
		requires invocable<Functor, ReturnType(ParamTypes...)>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>::delegate(
		const allocator_storage_type& allocStorage, const Functor& instance
	)
		: m_alloc(allocStorage),
		  m_invocation(base::template create_element<Functor>(m_alloc, instance))
	{
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>
	delegate<ReturnType(ParamTypes...), Alloc, Factory>::create(T& instance)
	{
		return delegate(base::template create_element<T, TMethod>(allocator_storage_type{}, instance));
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>
	delegate<ReturnType(ParamTypes...), Alloc, Factory>::create(const allocator_storage_type& alloc, T& instance)
	{
		return delegate(base::template create_element<T, TMethod>(alloc, instance));
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>
	delegate<ReturnType(ParamTypes...), Alloc, Factory>::create(const T& instance)
	{
		return delegate(base::template create_element<T, TMethod>(allocator_storage_type{}, instance));
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>
	delegate<ReturnType(ParamTypes...), Alloc, Factory>::create(const allocator_storage_type& alloc, const T& instance)
	{
		return delegate(base::template create_element<T, TMethod>(alloc, instance));
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <ReturnType (*TMethod)(ParamTypes...)>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>
	delegate<ReturnType(ParamTypes...), Alloc, Factory>::create()
	{
		return delegate(base::template create_element<TMethod>(allocator_storage_type{}));
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <ReturnType (*TMethod)(ParamTypes...)>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>
	delegate<ReturnType(ParamTypes...), Alloc, Factory>::create(const allocator_storage_type& alloc)
	{
		return delegate(base::template create_element<TMethod>(alloc));
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <functor Functor>
		requires invocable<Functor, ReturnType(ParamTypes...)>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>
	delegate<ReturnType(ParamTypes...), Alloc, Factory>::create(const Functor& instance)
	{
		return delegate(base::template create_element<Functor>(allocator_storage_type{}, instance));
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <functor Functor>
		requires invocable<Functor, ReturnType(ParamTypes...)>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>
	delegate<ReturnType(ParamTypes...), Alloc, Factory>::create(
		const allocator_storage_type& alloc, const Functor& instance
	)
	{
		return delegate(base::template create_element<Functor>(alloc, instance));
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr void
	delegate<ReturnType(ParamTypes...), Alloc, Factory>::set_allocator(const allocator_storage_type& allocStorage)
		noexcept(is_nothrow_copy_assignable_v<allocator_storage_type>)
	{
		m_alloc = allocStorage;
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>::allocator_t&
	delegate<ReturnType(ParamTypes...), Alloc, Factory>::get_allocator() noexcept
	{
		return *m_alloc;
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr const delegate<ReturnType(ParamTypes...), Alloc, Factory>::allocator_t&
	delegate<ReturnType(ParamTypes...), Alloc, Factory>::get_allocator() const noexcept
	{
		return *m_alloc;
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr bool delegate<ReturnType(ParamTypes...), Alloc, Factory>::empty() const noexcept
	{
		return m_invocation.stub == nullptr;
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr void delegate<ReturnType(ParamTypes...), Alloc, Factory>::clear() noexcept
	{
		m_invocation = invocation_element();
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr bool delegate<ReturnType(ParamTypes...), Alloc, Factory>::operator==(nullptr_type) const noexcept
	{
		return empty();
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr bool delegate<ReturnType(ParamTypes...), Alloc, Factory>::operator!=(nullptr_type) const noexcept
	{
		return !empty();
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr bool delegate<ReturnType(ParamTypes...), Alloc, Factory>::operator==(
		const multicast_delegate<ReturnType(ParamTypes...)>& other
	) const noexcept
	{
		return other == (*this);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr bool delegate<ReturnType(ParamTypes...), Alloc, Factory>::operator!=(
		const multicast_delegate<ReturnType(ParamTypes...)>& other
	) const noexcept
	{
		return other != (*this);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>&
	delegate<ReturnType(ParamTypes...), Alloc, Factory>::assign(T& instance)
	{
		m_invocation = base::template create_element<T, TMethod>(m_alloc, instance);
		return *this;
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>&
	delegate<ReturnType(ParamTypes...), Alloc, Factory>::assign(const T& instance)
	{
		m_invocation = base::template create_element<T, TMethod>(m_alloc, instance);
		return *this;
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <ReturnType (*TMethod)(ParamTypes...)>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>&
	delegate<ReturnType(ParamTypes...), Alloc, Factory>::assign()
	{
		m_invocation = base::template create_element<TMethod>(m_alloc);
		return *this;
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <invocable<ReturnType(ParamTypes...)> Functor>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>&
	delegate<ReturnType(ParamTypes...), Alloc, Factory>::operator=(const Functor& instance)
	{
		m_invocation = base::template create_element<Functor>(m_alloc, instance);
		return *this;
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr ReturnType delegate<ReturnType(ParamTypes...), Alloc, Factory>::operator()(ParamTypes... args
	) const
	{
		return invoke(args...);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr ReturnType delegate<ReturnType(ParamTypes...), Alloc, Factory>::invoke(ParamTypes... args) const
	{
		return (*m_invocation.stub)(*m_invocation.object, args...);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr delegate<ReturnType(ParamTypes...), Alloc, Factory>::delegate(
		const allocator_storage_type& allocStorage, invocation_element&& e
	)
		: m_alloc(allocStorage),
		  m_invocation(e)
	{
	}
} // namespace rsl
