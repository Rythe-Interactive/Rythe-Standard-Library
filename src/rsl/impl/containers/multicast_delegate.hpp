#pragma once

#include "delegate.hpp"
#include "dynamic_array.hpp"

namespace rsl
{
	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	class multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory> final :
		private delegate_base<ReturnType(ParamTypes...), Alloc, Factory>
	{
		using base = delegate_base<ReturnType(ParamTypes...), Alloc, Factory>;

	public:
		using return_type = ReturnType;
		using param_types = type_sequence<ParamTypes...>;
		using invocation_element = typename base::invocation_element;

		using value_type = delegate<ReturnType(ParamTypes...), Alloc, Factory>;

		using invocation_container = dynamic_array<value_type, Alloc>;

		using iterator_type = typename invocation_container::iterator_type;
		using const_iterator_type = typename invocation_container::const_iterator_type;
		using reverse_iterator_type = typename invocation_container::reverse_iterator_type;
		using const_reverse_iterator_type = typename invocation_container::const_reverse_iterator_type;
		using view_type = typename invocation_container::view_type;
		using const_view_type = typename invocation_container::const_view_type;
		using allocator_storage_type = allocator_storage<Alloc>;
		using allocator_t = Alloc;
		using factory_storage_type = factory_storage<Factory>;
		using factory_t = Factory;

		[[rythe_always_inline]] constexpr multicast_delegate() noexcept = default;

		[[rythe_always_inline]] constexpr multicast_delegate(const value_type& val) noexcept
			: m_invocationList(1, in_place_signal, val.m_invocation)
		{
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr bool empty() const noexcept { return m_invocationList.empty(); }
		[[rythe_always_inline]] constexpr void clear() noexcept { m_invocationList.clear(); }
		[[nodiscard]] [[rythe_always_inline]] constexpr size_type size() const noexcept
		{
			return m_invocationList.size();
		}
		[[rythe_always_inline]] constexpr void reserve(size_type newCap) noexcept { m_invocationList.reserve(newCap); }
		[[nodiscard]] [[rythe_always_inline]] constexpr size_type capacity() const noexcept
		{
			return m_invocationList.capacity();
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr auto begin() noexcept { return m_invocationList.begin(); }
		[[nodiscard]] [[rythe_always_inline]] constexpr auto cbegin() const noexcept
		{
			return m_invocationList.cbegin();
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr auto begin() const noexcept { return cbegin(); }
		[[nodiscard]] [[rythe_always_inline]] constexpr auto rbegin() noexcept { return m_invocationList.rbegin(); }
		[[nodiscard]] [[rythe_always_inline]] constexpr auto crbegin() const noexcept
		{
			return m_invocationList.crbegin();
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr auto rbegin() const noexcept { return crbegin(); }

		[[nodiscard]] [[rythe_always_inline]] constexpr auto end() noexcept { return m_invocationList.end(); }
		[[nodiscard]] [[rythe_always_inline]] constexpr auto cend() const noexcept { return m_invocationList.cend(); }
		[[nodiscard]] [[rythe_always_inline]] constexpr auto end() const noexcept { return cend(); }
		[[nodiscard]] [[rythe_always_inline]] constexpr auto rend() noexcept { return m_invocationList.rend(); }
		[[nodiscard]] [[rythe_always_inline]] constexpr auto crend() const noexcept { return m_invocationList.crend(); }
		[[nodiscard]] [[rythe_always_inline]] constexpr auto rend() const noexcept { return crend(); }

		[[nodiscard]] [[rythe_always_inline]] constexpr value_type& at(size_type i) noexcept
		{
			return m_invocationList.at(i);
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr const value_type& at(size_type i) const noexcept
		{
			return m_invocationList.at(i);
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr value_type& operator[](size_type i) noexcept { return at(i); }
		[[nodiscard]] [[rythe_always_inline]] constexpr const value_type& operator[](size_type i) const noexcept
		{
			return at(i);
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr value_type& front() noexcept
		{
			return m_invocationList.front();
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr const value_type& front() const noexcept
		{
			return m_invocationList.front();
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr value_type& back() noexcept { return m_invocationList.back(); }
		[[nodiscard]] [[rythe_always_inline]] constexpr const value_type& back() const noexcept
		{
			return m_invocationList.back();
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(nullptr_type) const noexcept { return empty(); }
		[[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(nullptr_type) const noexcept
		{
			return !empty();
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr bool
		operator==(const multicast_delegate&) const noexcept = default;
		[[nodiscard]] [[rythe_always_inline]] constexpr bool
		operator!=(const multicast_delegate&) const noexcept = default;

		[[nodiscard]] [[rythe_always_inline]] constexpr bool operator==(const value_type& other) const noexcept
		{
			return size() == 1 && at(0) == other;
		}
		[[nodiscard]] [[rythe_always_inline]] constexpr bool operator!=(const value_type& other) const noexcept
		{
			return !(*this == other);
		}

		[[rythe_always_inline]] constexpr multicast_delegate& push_back(const value_type& e)
		{
			m_invocationList.push_back(e);
			return *this;
		}

		[[rythe_always_inline]] constexpr multicast_delegate& push_back(value_type&& e)
		{
			m_invocationList.push_back(move(e));
			return *this;
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& push_back(T& instance)
		{
			return push_back(base::template create_element<T, TMethod>(m_alloc, instance));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] constexpr multicast_delegate& push_back(const T& instance)
		{
			return push_back(base::template create_element<T, TMethod>(m_alloc, instance));
		}

		template <ReturnType (*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& push_back()
		{
			return push_back(base::template create_element<TMethod>(m_alloc));
		}

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& push_back(const Functor& instance)
		{
			return push_back(base::template create_element<Functor>(m_alloc, instance));
		}

		[[rythe_always_inline]] constexpr multicast_delegate& operator+=(const value_type& another)
		{
			return push_back(another);
		}

		[[rythe_always_inline]] constexpr multicast_delegate& operator+=(value_type&& another)
		{
			return push_back(move(another));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& operator+=(T& instance)
		{
			return push_back(base::template create_element<T, TMethod>(m_alloc, instance));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] constexpr multicast_delegate& operator+=(const T& instance)
		{
			return push_back(base::template create_element<T, TMethod>(m_alloc, instance));
		}

		template <invocable<ReturnType(ParamTypes...)> Functor>
		[[rythe_always_inline]] constexpr multicast_delegate& operator+=(const Functor& instance)
		{
			return push_back(base::template create_element<Functor>(m_alloc, instance));
		}

		[[rythe_always_inline]] constexpr size_type erase(size_type pos) { return m_invocationList.erase(pos); }

		[[rythe_always_inline]] constexpr size_type erase(size_type first, size_type last)
		{
			return m_invocationList.erase(first, last);
		}

		[[rythe_always_inline]] constexpr void pop_back() { erase(size() - 1); }

		[[rythe_always_inline]] constexpr multicast_delegate& remove(const value_type& del)
		{
			return remove(del.m_invocation.id);
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& remove(T& instance)
		{
			return remove(base::template create_id<T, TMethod>(instance));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] constexpr multicast_delegate& remove(const T& instance)
		{
			return remove(base::template create_id<T, TMethod>(instance));
		}

		template <ReturnType (*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& remove()
		{
			return remove(base::template create_id<TMethod>());
		}

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& remove(const Functor& instance)
		{
			return remove(base::template create_id<Functor>(instance));
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr bool contains(const value_type& del) const noexcept
		{
			return contains(del.m_invocation.id);
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[nodiscard]] [[rythe_always_inline]] constexpr bool contains(T& instance) const noexcept
		{
			return contains(base::template create_id<T, TMethod>(instance));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[nodiscard]] [[rythe_always_inline]] constexpr bool contains(const T& instance) const noexcept
		{
			return contains(base::template create_id<T, TMethod>(instance));
		}

		template <ReturnType (*TMethod)(ParamTypes...)>
		[[nodiscard]] [[rythe_always_inline]] constexpr bool contains() const noexcept
		{
			return contains(base::template create_id<TMethod>());
		}

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		[[nodiscard]] [[rythe_always_inline]] constexpr bool contains(const Functor& instance) const noexcept
		{
			return contains(base::template create_id<Functor>(instance));
		}

		[[rythe_always_inline]] constexpr multicast_delegate& operator-=(const value_type& another)
		{
			return remove(another.m_invocation.id);
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& operator-=(T& instance)
		{
			return remove(base::template create_id<T, TMethod>(instance));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] constexpr multicast_delegate& operator-=(const T& instance)
		{
			return remove(base::template create_id<T, TMethod>(instance));
		}

		template <invocable<ReturnType(ParamTypes...)> Functor>
		[[rythe_always_inline]] constexpr multicast_delegate& operator-=(const Functor& instance)
		{
			return remove(base::template create_id<Functor>(instance));
		}

		[[rythe_always_inline]] constexpr multicast_delegate& operator=(const multicast_delegate&) = default;

		[[rythe_always_inline]] constexpr multicast_delegate& operator=(const value_type& del)
		{
			clear();
			return push_back(del);
		}

		[[rythe_always_inline]] constexpr multicast_delegate& operator=(value_type&& del)
		{
			clear();
			return push_back(move(del));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& operator=(T& instance)
		{
			clear();
			return push_back(base::template create_element<T, TMethod>(m_alloc, instance));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] constexpr multicast_delegate& operator=(const T& instance)
		{
			clear();
			return push_back(base::template create_element<T, TMethod>(m_alloc, instance));
		}

		template <invocable<ReturnType(ParamTypes...)> Functor>
		[[rythe_always_inline]] constexpr multicast_delegate& operator=(const Functor& instance)
		{
			clear();
			return push_back(base::template create_element<Functor>(m_alloc, instance));
		}

		[[rythe_always_inline]] constexpr multicast_delegate& assign(const value_type& del)
		{
			clear();
			return push_back(del);
		}

		[[rythe_always_inline]] constexpr multicast_delegate& assign(value_type&& del)
		{
			clear();
			return push_back(move(del));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...)>
		[[rythe_always_inline]] multicast_delegate& assign(T& instance)
		{
			clear();
			return push_back(base::template create_element<T, TMethod>(m_alloc, instance));
		}

		template <typename T, ReturnType (T::*TMethod)(ParamTypes...) const>
		[[rythe_always_inline]] constexpr multicast_delegate& assign(const T& instance)
		{
			clear();
			return push_back(base::template create_element<T, TMethod>(m_alloc, instance));
		}

		template <functor Functor>
			requires invocable<Functor, ReturnType(ParamTypes...)>
		[[rythe_always_inline]] constexpr multicast_delegate& assign(const Functor& instance)
		{
			clear();
			return push_back(base::template create_element<Functor>(m_alloc, instance));
		}

		template <input_iterator InputIt>
		[[rythe_always_inline]] constexpr multicast_delegate& assign(InputIt first, InputIt last)
		{
			m_invocationList.assign(first, last);
			return *this;
		}

	private:
		template <typename T>
		struct invocation_result
		{
			using type = dynamic_array<T, allocator_t>;
		};

		template <>
		struct invocation_result<void>
		{
			using type = void;
		};

		template <typename T>
		using invocation_result_t = typename invocation_result<T>::type;

	public:
		[[rythe_always_inline]] constexpr auto operator()(ParamTypes... args) const { return invoke(args...); }

		[[rythe_always_inline]] constexpr auto invoke(ParamTypes... args) const -> invocation_result_t<ReturnType>
		{
			if constexpr (same_as<ReturnType, void>)
			{
				for (auto& m_item : m_invocationList)
				{
					m_item.invoke(args...);
				}
			}
			else
			{
				invocation_result_t<ReturnType> result;
				result.reserve(size());
				for (auto& m_item : m_invocationList)
				{
					result.push_back(m_item.invoke(args...));
				}

				return result;
			}
		}

	private:
		[[rythe_always_inline]] constexpr multicast_delegate(invocation_container&& e)
			: m_invocationList(move(e))
		{
		}

		[[rythe_always_inline]] constexpr multicast_delegate& remove(id_type id)
		{
			m_invocationList.erase(id);
			return *this;
		}

		[[nodiscard]] [[rythe_always_inline]] constexpr bool contains(id_type id) const noexcept
		{
			for (auto& element : m_invocationList)
			{
				if (element.id == id)
				{
					return true;
				}
			}

			return false;
		}

		allocator_storage_type m_alloc;
		invocation_container m_invocationList;
	};

	template <typename ReturnType, typename... ParamTypes, allocator_type Alloc, untyped_factory_type Factory>
	multicast_delegate(const delegate<ReturnType(ParamTypes...), Alloc, Factory>&)
		-> multicast_delegate<ReturnType(ParamTypes...), Alloc, Factory>;
} // namespace rsl

#include "multicast_delegate.inl"
