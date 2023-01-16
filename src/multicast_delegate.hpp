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
#include "delegate.hpp"
#include <list>
#include <functional>

namespace rythe {

	template<typename RET, typename ...PARAMS>
	class multicast_delegate<RET(PARAMS...)> final : private delegate_base<RET(PARAMS...)> {
	public:

        using ReturnType = RET;
        using ParamTypes = PARAMS;

		multicast_delegate() = default;
		~multicast_delegate() {
			for (auto& element : m_invocationList) delete element;
			m_invocationList.clear();
		}

		bool empty() const { return m_invocationList.empty(); }
		bool operator ==(std::nullptr_t) const { return empty(); }
        bool operator !=(std::nullptr_t) const { return !empty(); }
        operator bool() const { return !empty(); }

		size_t size() const { return m_invocationList.size(); }

		multicast_delegate& operator =(const multicast_delegate&) = delete;
		multicast_delegate(const multicast_delegate&) = delete;

		bool operator ==(const multicast_delegate& another) const {
			if (m_invocationList.size() != another.m_invocationList.size())
                return false;

			auto anotherIt = another.m_invocationList.begin();
			for (auto it = m_invocationList.begin(); it != m_invocationList.end(); ++it)
				if (**it != **anotherIt)
                    return false;

			return true;
		}

		bool operator !=(const multicast_delegate& another) const { return !(*this == another); }

		bool operator ==(const delegate<ReturnType(ParamTypes...)>& another) const {
			if (isNull() && another.isNull()) return true;
			if (another.isNull() || (size() != 1)) return false;
			return (another.invocation == **m_invocationList.begin());
		}

		bool operator !=(const delegate<ReturnType(ParamTypes...)>& another) const { return !(*this == another); }

		multicast_delegate& operator +=(const multicast_delegate& another) {
			for (auto& item : another.m_invocationList) // clone, not copy; flattens hierarchy:
				this->m_invocationList.push_back(new typename delegate_base<ReturnType(ParamTypes...)>::invocation_element(item->object, item->stub));
			return *this;
		}

		template <typename LAMBDA> // template instantiation is not neededm, will be deduced/inferred:
		multicast_delegate& operator +=(const LAMBDA & lambda) {
			delegate<ReturnType(ParamTypes...)> d = delegate<ReturnType(ParamTypes...)>::template create<LAMBDA>(lambda);
			return *this += d;
		}

		multicast_delegate& operator +=(const delegate<ReturnType(ParamTypes...)>& another) {
			if (another.isNull()) return *this;
			this->m_invocationList.push_back(new typename delegate_base<ReturnType(ParamTypes...)>::invocation_element(another.invocation.object, another.invocation.stub));
			return *this;
		}

		// will work even if ReturnType is void, return values are ignored:
		// (for handling return values, see operator(..., handler))
		void operator()(ParamTypes... arg) const {
			for (auto& item : m_invocationList)
				(*(item->stub))(item->object, arg...);
		}

		template<typename HANDLER>
		void operator()(ParamTypes... arg, HANDLER handler) const {
			size_t index = 0;
			for (auto& item : m_invocationList) {
				ReturnType value = (*(item->stub))(item->object, arg...);
				handler(index, &value);
				++index;
			}
		}

		void operator()(ParamTypes... arg, delegate<void(size_t, ReturnType*)> handler) const {
			operator()<decltype(handler)>(arg..., handler);
		}

		void operator()(ParamTypes... arg, std::function<void(size_t, ReturnType*)> handler) const {
			operator()<decltype(handler)>(arg..., handler);
		}

	private:

		std::list<typename delegate_base<ReturnType(ParamTypes...)>::invocation_element *> m_invocationList;

	};

}

