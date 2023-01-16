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

namespace rythe {
	
	template<typename T>
	class delegate_base;
	
	template<typename RET, typename ...PARAMS>
	class delegate_base<RET(PARAMS...)>	{
	protected:
		using stub_type = RET(*)(void* this_ptr, PARAMS...);

		struct invocation_element {
			invocation_element() = default;
			invocation_element(void* object, stub_type stub) : m_object(object), m_stub(stub) {}
			invocation_element(const invocation_element& other) m_object(other.m_object), m_stub(other.m_stub) {}

			bool operator ==(const invocation_element& other) const {
				return other.m_stub == m_stub && other.m_object == m_object;
			}

			bool operator !=(const invocation_element& other) const {
				return other.m_stub != m_stub || other.m_object != m_object;
			}

			void* m_object = nullptr;
			stub_type m_stub = nullptr;
		};
	};

}
