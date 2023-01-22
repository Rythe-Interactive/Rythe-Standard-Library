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
#include <cstddef>
#include <functional>

#include "../concepts"

#include "delegate_base.hpp"

namespace rsl {

    template<typename T> class delegate;
    template<typename T> class multicast_delegate;

    template<typename ReturnType, typename... ParamTypes>
    class delegate<ReturnType(ParamTypes...)> final : private delegate_base<ReturnType(ParamTypes...)> {
        using base = delegate_base<ReturnType(ParamTypes...)>;
    public:
        using return_type = ReturnType;
        using param_types = type_sequence<ParamTypes...>;
        using invocation_element = typename base::invocation_element;

    private:
        using stub_type = typename base::stub_type;

        friend class multicast_delegate<ReturnType(ParamTypes...)>;
        invocation_element m_invocation;

        constexpr delegate(invocation_element&& e) : m_invocation(e) {}

    public:
        delegate() = default;
        delegate(const delegate& other) : m_invocation(other.m_invocation) {}

        template<typename T, ReturnType(T::* TMethod)(ParamTypes...)>
        static constexpr delegate create(T& instance) { return delegate(base::template createElement<T, TMethod>(instance)); }

        template<typename T, ReturnType(T::* TMethod)(ParamTypes...) const>
        static constexpr delegate create(const T& instance) { return delegate(base::template createElement<T, TMethod>(instance)); }

        template <ReturnType(*TMethod)(ParamTypes...)>
        static constexpr delegate create() { return delegate(base::template createElement<TMethod>()); }

        template <functor Functor>
            requires std::invocable<Functor, ParamTypes...>&& std::same_as<std::invoke_result_t<Functor, ParamTypes...>, ReturnType>
        static constexpr delegate create(const Functor& instance) { return delegate(base::template createElement<Functor>(instance)); }

        inline bool empty() const { return m_invocation.stub == nullptr; }
        inline void clear() { m_invocation = invocation_element(); }

        inline bool operator ==(std::nullptr_t) const { return empty(); }
        inline bool operator !=(std::nullptr_t) const { return !empty(); }

        inline bool operator == (const delegate& another) const { return m_invocation == another.m_invocation; }
        inline bool operator != (const delegate& another) const { return m_invocation != another.m_invocation; }

        inline bool operator ==(const multicast_delegate<ReturnType(ParamTypes...)>& another) const { return another == (*this); }
        inline bool operator !=(const multicast_delegate<ReturnType(ParamTypes...)>& another) const { return another != (*this); }

        inline delegate& operator =(const delegate& another) {
            m_invocation = another.m_invocation;
            return *this;
        }

        template <invocable Functor>
        requires std::invocable<Functor, ParamTypes...>&& std::same_as<std::invoke_result_t<Functor, ParamTypes...>, ReturnType>
        constexpr delegate& operator =(const Functor& instance) {
            m_invocation = base::template createElement<Functor>(instance);
            return *this;
        }

        inline ReturnType operator()(ParamTypes... args) const {
            return invoke(args...);
        }

        inline ReturnType invoke(ParamTypes... args) const {
            return (*m_invocation.m_stub)(m_invocation.m_object.get(), args...);
        }
    };
}
