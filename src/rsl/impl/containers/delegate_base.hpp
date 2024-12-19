#pragma once

#include <bit>
#include <memory>

#include "../defines.hpp"
#include "../util/concepts.hpp"
#include "../util/hash.hpp"
#include "../util/utilities.hpp"

namespace rsl
{

	template <typename T>
	class delegate_base;

	template <typename ReturnType, typename... ParamTypes>
	class delegate_base<ReturnType(ParamTypes...)>
	{
	protected:
		using stub_type = ReturnType (*)(void*, ParamTypes...);
		using deleter_type = void (*)(void*);
		constexpr static deleter_type defaultDeleter = []([[maybe_unused]] void*) {};

		struct invocation_element
		{
			using return_type = ReturnType;
			using param_types = type_sequence<ParamTypes...>;
			constexpr invocation_element() = default;
			constexpr invocation_element(void* object, stub_type stub, id_type id, deleter_type deleter = nullptr)
				: object(object, deleter ? deleter : defaultDeleter),
				  ownsData(deleter != nullptr),
				  stub(stub),
				  id(id)
			{
			}
			constexpr invocation_element(const invocation_element& other)
				: object(other.object),
				  ownsData(other.ownsData),
				  stub(other.stub),
				  id(other.id)
			{
			}

			constexpr bool operator==(id_type otherId) const noexcept { return id == otherId; }
			constexpr bool operator!=(id_type otherId) const noexcept { return id != otherId; }

			constexpr bool operator==(const invocation_element& other) const noexcept { return id == other.id; }
			constexpr bool operator!=(const invocation_element& other) const noexcept { return id != other.id; }

			std::shared_ptr<void> object = nullptr;
			bool ownsData = false;
			stub_type stub = nullptr;
			id_type id = invalid_id;
		};

		template <typename T, ReturnType (T::*method)(ParamTypes...)>
		static ReturnType method_stub(void* obj, ParamTypes... args)
		{
			T* p = force_cast<T*>(obj);
			return (p->*method)(forward<ParamTypes>(args)...);
		}

		template <typename T, ReturnType (T::*method)(ParamTypes...) const>
		static ReturnType const_method_stub(void* obj, ParamTypes... args)
		{
			const T* p = force_cast<const T*>(obj);
			return (p->*method)(forward<ParamTypes>(args)...);
		}

		template <typename T, ReturnType (T::*method)(ParamTypes...)>
		[[rythe_always_inline]] static id_type method_id(const T& obj)
		{
			return combine_hash(force_cast<size_type>(&obj), force_cast<size_type>(method));
		}

		template <typename T, ReturnType (T::*method)(ParamTypes...) const>
		[[rythe_always_inline]] static id_type method_id(const T& obj)
		{
			return combine_hash(force_cast<size_type>(&obj), force_cast<size_type>(method));
		}

		template <ReturnType (*func)(ParamTypes...)>
		static ReturnType function_stub(void* obj, ParamTypes... args)
		{
			return (func)(forward<ParamTypes>(args)...);
		}

		template <ReturnType (*func)(ParamTypes...)>
		[[rythe_always_inline]] static id_type function_id()
		{
			return force_cast<size_type>(func);
		}

		template <invocable<ReturnType(ParamTypes...)> Func>
			requires(!functor<Func>)
		static ReturnType function_ptr_stub(void* obj, ParamTypes... args)
		{
			return (*bit_cast<Func*>(&obj))(forward<ParamTypes>(args)...);
		}

		template <invocable<ReturnType(ParamTypes...)> Func>
			requires(!functor<Func>)
		[[rythe_always_inline]] static id_type function_ptr_id(Func obj)
		{
			return force_cast<size_type>(obj);
		}

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		static ReturnType functor_stub(void* obj, ParamTypes... args)
		{
			Functor* p = force_cast<Functor*>(obj);
			return (p->operator())(forward<ParamTypes>(args)...);
		}

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		[[rythe_always_inline]] static id_type functor_id(const Functor& obj)
		{
			return combine_hash(force_cast<size_type>(&obj), force_cast<size_type>(&Functor::operator()));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] static invocation_element create_element(T& instance)
		{
			return invocation_element(&instance, method_stub<T, TMethod>, method_id<T, TMethod>(instance));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] static invocation_element create_element(const T& instance)
		{
			return invocation_element(
				force_cast<void*>(&instance), const_method_stub<T, TMethod>, method_id<T, TMethod>(instance)
			);
		}

		template <ReturnType (*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] static invocation_element create_element()
		{
			return invocation_element(nullptr, function_stub<TMethod>, function_id<TMethod>());
		}

		template <invocable<ReturnType(ParamTypes...)> Functor>
		[[rythe_always_inline]] static invocation_element create_element(const Functor& instance)
		{
			if constexpr (!is_functor_v<Functor>)
			{
				return invocation_element(
					*bit_cast<void**>(&instance), function_ptr_stub<Functor>, function_ptr_id<Functor>(instance)
				);
			}
			else if constexpr (is_empty_v<Functor>)
			{
				return invocation_element(
					force_cast<void*>(&instance), functor_stub<Functor>, functor_id<Functor>(instance)
				);
			}
			else
			{
				return invocation_element(
					new Functor(instance), functor_stub<Functor>, functor_id<Functor>(instance),
					[](void* vptr)
				{
					Functor* ptr = static_cast<Functor*>(vptr);
					delete ptr;
				}
				);
			}
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] static id_type create_id(T& instance)
		{
			return method_id<T, TMethod>(instance);
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] static id_type create_id(const T& instance)
		{
			return method_id<T, TMethod>(instance);
		}

		template <ReturnType (*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] static id_type create_id()
		{
			return function_id<TMethod>();
		}

		template <invocable<ReturnType(ParamTypes...)> Functor>
		[[rythe_always_inline]] static id_type create_id(const Functor& instance)
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
	};

} // namespace rsl
