/*

    Copyright (C) 2017 by Sergey A Kryukov: derived work
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
#include "delegate_base.hpp"

namespace rythe {

    template <typename T> class delegate;
    template <typename T> class multicast_delegate;

    template<typename RET, typename ...PARAMS>
    class delegate<RET(PARAMS...)> final : private delegate_base<RET(PARAMS...)> {
        friend class multicast_delegate<RET(PARAMS...)>;
        invocation_element m_invocation;
        
    public:
        delegate() = default;
        delegate(const delegate& other) : m_invocation(other.m_invocation) {}

        bool empty() const { return m_invocation.stub == nullptr; }
        bool operator ==(std::nullptr_t) const { return empty(); }
        bool operator !=(std::nullptr_t) const { return !empty(); }

        template <typename LAMBDA>
        delegate(const LAMBDA& lambda) {
            assign((void*)(&lambda), lambda_stub<LAMBDA>);
        }

        delegate& operator =(const delegate& another) {
            another.m_invocation.Clone(m_invocation);
            return *this;
        } //operator =

        template <typename LAMBDA> // template instantiation is not needed, will be deduced (inferred):
        delegate& operator =(const LAMBDA& instance) {
            assign((void*)(&instance), lambda_stub<LAMBDA>);
            return *this;
        } //operator =

        bool operator == (const delegate& another) const { return m_invocation == another.m_invocation; }
        bool operator != (const delegate& another) const { return m_invocation != another.m_invocation; }

        bool operator ==(const multicast_delegate<RET(PARAMS...)>& another) const { return another == (*this); }
        bool operator !=(const multicast_delegate<RET(PARAMS...)>& another) const { return another != (*this); }

        template <class T, RET(T::*TMethod)(PARAMS...)>
        static delegate create(T* instance) {
            return delegate(instance, method_stub<T, TMethod>);
        } //create

        template <class T, RET(T::*TMethod)(PARAMS...) const>
        static delegate create(T const* instance) {
            return delegate(const_cast<T*>(instance), const_method_stub<T, TMethod>);
        } //create

        template <RET(*TMethod)(PARAMS...)>
        static delegate create() {
            return delegate(nullptr, function_stub<TMethod>);
        } //create

        template <typename LAMBDA>
        static delegate create(const LAMBDA & instance) {
            return delegate((void*)(&instance), lambda_stub<LAMBDA>);
        } //create

        RET operator()(PARAMS... arg) const {
            return (*m_invocation.stub)(m_invocation.object, arg...);
        } //operator()

    private:
        using invocation_element = delegate_base<RET(PARAMS...)>::invocation_element;
        delegate(void* a_object, typename delegate_base<RET(PARAMS...)>::stub_type a_stub) : m_invocation(a_object, a_stub) {}

        void assign(void* a_object, typename delegate_base<RET(PARAMS...)>::stub_type a_stub) {
            m_invocation = invocation_element(a_object, a_stub);
        }

        template <class T, RET(T::*TMethod)(PARAMS...)>
        static RET method_stub(void* this_ptr, PARAMS... params) {
            T* p = static_cast<T*>(this_ptr);
            return (p->*TMethod)(params...);
        } //method_stub

        template <class T, RET(T::*TMethod)(PARAMS...) const>
        static RET const_method_stub(void* this_ptr, PARAMS... params) {
            T* const p = static_cast<T*>(this_ptr);
            return (p->*TMethod)(params...);
        } //const_method_stub

        template <RET(*TMethod)(PARAMS...)>
        static RET function_stub(void* this_ptr, PARAMS... params) {
            return (TMethod)(params...);
        } //function_stub

        template <typename LAMBDA>
        static RET lambda_stub(void* this_ptr, PARAMS... arg) {
            LAMBDA* p = static_cast<LAMBDA*>(this_ptr);
            return (p->operator())(arg...);
        } //lambda_stub
    };

}
