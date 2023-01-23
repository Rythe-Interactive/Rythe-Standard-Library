/*

	Based on work by Sergey A Kryukov: derived work
	http://www.SAKryukov.org
	http://www.codeproject.com/Members/SAKryukov

	Based on original work by Sergey Ryazanov:
	"The Impossibly Fast C++ Delegates", 18 Jul 2005
	https://www.codeproject.com/articles/11015/the-impossibly-fast-c-delegates

	MIT license:
	http://en.wikipedia.org/wiki/MIT_License

	Original publication: https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed

*/

#pragma once
#include <list>
#include <functional>

#include "delegate.hpp"

namespace rsl {

	template<typename ReturnType, typename ...ParamTypes>
	class multicast_delegate<ReturnType(ParamTypes...)> final : private delegate_base<ReturnType(ParamTypes...)> {
		using base = delegate_base<ReturnType(ParamTypes...)>;
	public:
		using return_type = ReturnType;
		using param_types = type_sequence<ParamTypes...>;
		using invocation_element = typename base::invocation_element;

	private:
		std::vector<invocation_element> m_invocationList;
		using stub_type = typename base::stub_type;

		constexpr multicast_delegate(multicast_delegate&& e) : m_invocationList(e) {}

	public:
		constexpr multicast_delegate() = default;
		constexpr multicast_delegate(const multicast_delegate& other) : m_invocationList(other.m_invocationList) {}

		constexpr bool empty() const { return m_invocationList.size() < 1; }
		constexpr void clear() { m_invocationList.clear(); }

		constexpr bool operator ==(std::nullptr_t) const { return empty(); }
		constexpr bool operator !=(std::nullptr_t) const { return !empty(); }

		constexpr bool operator ==(const multicast_delegate<ReturnType(ParamTypes...)>& another) const { return another == (*this); }
		constexpr bool operator !=(const multicast_delegate<ReturnType(ParamTypes...)>& another) const { return another != (*this); }

		constexpr multicast_delegate& push_back(const delegate<ReturnType(ParamTypes...)>& del)
		{
			m_invocationList.push_back(del.m_invocation);
			return *this;
		}

		template<typename T, ReturnType(T::* TMethod)(ParamTypes...)>
		constexpr multicast_delegate& push_back(T& instance)
		{
		    m_invocationList.push_back(base::template createElement<T, TMethod>(instance));
		    return *this;
		}

		template<typename T, ReturnType(T::* TMethod)(ParamTypes...) const>
		constexpr multicast_delegate& push_back(const T& instance)
		{
		    m_invocationList.push_back(base::template createElement<T, TMethod>(instance));
		    return *this;
		}

		template <ReturnType(*TMethod)(ParamTypes...)>
		constexpr multicast_delegate& push_back()
		{
			m_invocationList.push_back(base::template createElement<TMethod>());
		    return *this;
		}

		constexpr multicast_delegate& operator +=(delegate<ReturnType(ParamTypes...)>&& another) {
			m_invocationList.push_back(another.m_invocation);
			return *this;
		}

		constexpr multicast_delegate& operator +=(const delegate<ReturnType(ParamTypes...)>& another) {
			m_invocationList.push_back(another.m_invocation);
			return *this;
		}

		constexpr multicast_delegate& remove(const delegate<ReturnType(ParamTypes...)>& del)
		{
			for (auto& item : m_invocationList)
			{
				if (item.m_id == del.m_invocation.m_id)
				{
					m_invocationList.erase(item);
					break;
				}
			}
			return *this;
		}


		constexpr void operator -=(delegate<ReturnType(ParamTypes...)>&& another) {
			for (auto& item : m_invocationList)
			{
				if (item.m_id == another.m_invocation.m_id)
				{
					m_invocationList.erase(item);
					break;
				}
			}
		}

		constexpr void operator -=(const delegate<ReturnType(ParamTypes...)>& another) {
			for (auto& item : m_invocationList)
			{
				if (item.m_id == another.m_invocation.m_id)
				{
					m_invocationList.erase(item);
					break;
				}
			}
		}

		constexpr multicast_delegate& operator =(multicast_delegate<ReturnType(ParamTypes...)>&& another) {
			m_invocationList = another.m_invocationList;
			return *this;
		}

		constexpr multicast_delegate& operator =(const multicast_delegate<ReturnType(ParamTypes...)>& another) {
			m_invocationList = another.m_invocationList;
			return *this;
		}

		template <invocable Functor>
			requires std::invocable<Functor, ParamTypes...>&& std::same_as<std::invoke_result_t<Functor, ParamTypes...>, ReturnType>
		constexpr multicast_delegate& operator =(const Functor& instance) {
			m_invocationList.push_back(base::template createElement<Functor>(instance));
			return *this;
		}

		constexpr void operator()(ParamTypes... args) const {
			invoke(args...);
		}

		constexpr void invoke(ParamTypes... args) const
		{
			for (auto& m_item : m_invocationList)
			{
				(*m_item.m_stub)(m_item.m_object.get(), args...);
			}
		}
	};

}

