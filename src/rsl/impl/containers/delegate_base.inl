#pragma once
#include "delegate_base.hpp"

namespace rsl
{
	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::invocation_element::invocation_element(
		const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage, void* object,
		stub_type stub, id_type id, deleter_type deleter
	)
		noexcept(is_nothrow_constructible_v<
				 managed_resource<void*, Alloc, Factory>, const allocator_storage_type&, const factory_storage_type&,
				 deleter_type, void*>)
		: object(allocStorage, factoryStorage, deleter ? deleter : defaultDeleter, object),
		  ownsData(deleter != nullptr),
		  stub(stub),
		  id(id)
	{
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	inline constexpr delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::invocation_element::invocation_element(
		const invocation_element& other
	) noexcept(is_nothrow_copy_constructible_v<managed_resource<void*, Alloc, Factory>>)
		: object(other.object),
		  ownsData(other.ownsData),
		  stub(other.stub),
		  id(other.id)
	{
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*method)(ParamTypes...)>
	inline ReturnType
	delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::method_stub(void* obj, ParamTypes... args)
	{
		T* p = force_cast<T*>(obj);
		return (p->*method)(forward<ParamTypes>(args)...);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*method)(ParamTypes...) const>
	inline ReturnType
	delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::const_method_stub(void* obj, ParamTypes... args)
	{
		const T* p = force_cast<const T*>(obj);
		return (p->*method)(forward<ParamTypes>(args)...);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*method)(ParamTypes...)>
	inline id_type delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::method_id(const T& obj)
	{
		return combine_hash(force_cast<size_type>(&obj), force_cast<size_type>(method));
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*method)(ParamTypes...) const>
	inline id_type delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::method_id(const T& obj)
	{
		return combine_hash(force_cast<size_type>(&obj), force_cast<size_type>(method));
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <ReturnType (*func)(ParamTypes...)>
	inline ReturnType delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::function_stub(void*, ParamTypes... args)
	{
		return (func)(forward<ParamTypes>(args)...);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <ReturnType (*func)(ParamTypes...)>
	inline id_type delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::function_id()
	{
		return force_cast<size_type>(func);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <invocable<ReturnType(ParamTypes...)> Func>
	inline ReturnType
	delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::function_ptr_stub(void* obj, ParamTypes... args)
		requires(!functor<Func>)
	{
		return (*bit_cast<Func*>(&obj))(forward<ParamTypes>(args)...);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <invocable<ReturnType(ParamTypes...)> Func>
	inline id_type delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::function_ptr_id(Func obj)
		requires(!functor<Func>)
	{
		return force_cast<size_type>(obj);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <functor Functor>
	inline ReturnType
	delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::functor_stub(void* obj, ParamTypes... args)
		requires invocable<Functor, ReturnType(ParamTypes...)>
	{
		Functor* p = force_cast<Functor*>(obj);
		return (p->operator())(forward<ParamTypes>(args)...);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <functor Functor>
	inline id_type delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::functor_id(const Functor& obj)
		requires invocable<Functor, ReturnType(ParamTypes...)>
	{
		return combine_hash(force_cast<size_type>(&obj), force_cast<size_type>(&Functor::operator()));
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
	inline delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::invocation_element
	delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::create_element(
		const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage, T& instance
	)
	{
		return invocation_element(
			allocStorage, factoryStorage, &instance, method_stub<T, TMethod>, method_id<T, TMethod>(instance)
		);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
	inline delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::invocation_element
	delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::create_element(
		const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage, const T& instance
	)
	{
		return invocation_element(
			allocStorage, factoryStorage, force_cast<void*>(&instance), const_method_stub<T, TMethod>,
			method_id<T, TMethod>(instance)
		);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <ReturnType (*TMethod)(ParamTypes...)>
	inline delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::invocation_element
	delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::create_element(
		const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage
	)
	{
		return invocation_element(
			allocStorage, factoryStorage, nullptr, function_stub<TMethod>, function_id<TMethod>()
		);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <invocable<ReturnType(ParamTypes...)> Functor>
	inline delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::invocation_element
	delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::create_element(
		const allocator_storage_type& allocStorage, const factory_storage_type& factoryStorage, const Functor& instance
	)
	{
		if constexpr (!is_functor_v<Functor>)
		{
			return invocation_element(
				allocStorage, factoryStorage, *bit_cast<void**>(&instance), function_ptr_stub<Functor>,
				function_ptr_id<Functor>(instance)
			);
		}
		else if constexpr (is_empty_v<Functor>)
		{
			return invocation_element(
				allocStorage, factoryStorage, force_cast<void*>(&instance), functor_stub<Functor>,
				functor_id<Functor>(instance)
			);
		}
		else
		{
			return invocation_element(
				allocStorage, factoryStorage, new Functor(instance), functor_stub<Functor>,
				functor_id<Functor>(instance),
				[](void* vptr)
			{
				Functor* ptr = static_cast<Functor*>(vptr);
				delete ptr;
			}
			);
		}
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
	inline id_type
	delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::create_id(T& instance)
	{
		return method_id<T, TMethod>(instance);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
	inline id_type
	delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::create_id(const T& instance)
	{
		return method_id<T, TMethod>(instance);
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <ReturnType (*TMethod)(ParamTypes...)>
	inline id_type delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::create_id()
	{
		return function_id<TMethod>();
	}

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	template <invocable<ReturnType(ParamTypes...)> Functor>
	inline id_type delegate_base<ReturnType(ParamTypes...), Alloc, Factory>::create_id(const Functor& instance)
	{
		if constexpr (!is_functor_v<Functor>)
		{
			return function_ptr_id<Functor>(instance);
		}
		else if constexpr (is_empty_v<Functor>)
		{
			return functor_id<Functor>(instance);
		}
		else
		{
			return functor_id<Functor>(instance);
		}
	}
} // namespace rsl
