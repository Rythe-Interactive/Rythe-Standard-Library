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

#include "../concepts"
#include "../hash"
#include "../utilities"

namespace rsl {
    
    template<typename T>
    class delegate_base;
    
    template<typename ReturnType, typename... ParamTypes>
    class delegate_base<ReturnType(ParamTypes...)>	{
    protected:
        using stub_type = ReturnType(*)(void*, ParamTypes...);

        struct invocation_element {
            using return_type = ReturnType;
            using param_types = type_sequence<ParamTypes...>;
            constexpr invocation_element() = default;
            constexpr invocation_element(void* object, stub_type stub, id_type id) : m_object(object), m_stub(stub), m_id(id) {}
            constexpr invocation_element(const invocation_element& other) : m_object(other.m_object), m_stub(other.m_stub), m_id(other.m_id) {}

            constexpr bool operator ==(const invocation_element& other) const {
                return other.m_stub == m_stub && other.m_object == m_object;
            }

            constexpr bool operator !=(const invocation_element& other) const {
                return other.m_stub != m_stub || other.m_object != m_object;
            }

            void* m_object = nullptr;
            stub_type m_stub = nullptr;
            id_type m_id = invalid_id;
        };

        template<typename T, ReturnType(T::* method)(ParamTypes...)>
        static ReturnType method_stub(void* obj, ParamTypes... args) {
            T* p = static_cast<T*>(obj);
            return (p->*method)(std::forward<ParamTypes>(args)...);
        }

        template<typename T, ReturnType(T::* method)(ParamTypes...) const>
        static ReturnType const_method_stub(void* obj, ParamTypes... args) {
            const T* p = static_cast<const T*>(obj);
            return (p->*method)(std::forward<ParamTypes>(args)...);
        }

        template<typename T, ReturnType(T::* method)(ParamTypes...)>
        static id_type method_id(const T& obj) {
            return hash_combine(force_cast<size_type>(&obj), force_cast<size_type>(method));
        }

        template<ReturnType(*func)(ParamTypes...)>
        static ReturnType function_stub(void* obj, ParamTypes... args) {
            return (func)(std::forward<ParamTypes>(args)...);
        }

        template<ReturnType(*func)(ParamTypes...)>
        constexpr static id_type function_id() {
            return force_cast<size_type>(func);
        }

        template<functor Functor>
        static ReturnType functor_stub(void* obj, ParamTypes... args) {
            Functor* p = static_cast<Functor*>(obj);
            return (p->operator())(std::forward<ParamTypes>(args)...);
        }

        template<functor Functor>
        static id_type functor_id(Functor&& obj, ParamTypes... args) {
            return method_id<Functor, &Functor::operator()>(std::forward<Functor>(obj));
        }
    };

}
