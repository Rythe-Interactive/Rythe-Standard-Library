#pragma once
#include <type_traits>
#include <typeinfo>

#include "..\util\primitives.hpp"
namespace rsl
{
	template <size_type maxSize>
	class any
	{
	private:
		unsigned char m_data[maxSize];

	public:
		constexpr any() noexcept {}

		any(const any& other)
		{
			memcpy(m_data, other.m_data, sizeof(other.m_data));
		}

		any(any&& other) noexcept
			: m_data(std::move(other.m_data))
		{
		}

		template <class ValueType, std::enable_if_t<std::conjunction_v<std::negation<std::is_same<std::decay_t<ValueType>, any>>, std::negation<std::_Is_specialization<std::decay_t<ValueType>, std::in_place_type_t>>, std::is_copy_constructible<std::decay_t<ValueType>>>, int> = 0>
		any(ValueType&& Value)
		{
			_emplace<std::decay_t<ValueType>>(std::forward<ValueType>(Value));
		}

		template <class ValueType, class... Types, std::enable_if_t<std::conjunction_v<std::is_constructible<std::decay_t<ValueType>, Types...>, std::is_copy_constructible<std::decay_t<ValueType>>>, int> = 0>
		explicit any(std::in_place_type_t<ValueType>, Types&&... Args)
		{
			_emplace<std::decay_t<ValueType>>(std::forward<Types>(Args)...);
		}

		template <class ValueType, class Elem, class... Types, std::enable_if_t<std::conjunction_v<std::is_constructible<std::decay_t<ValueType>, std::initializer_list<Elem>&, Types...>, std::is_copy_constructible<std::decay_t<ValueType>>>, int> = 0>
		explicit any(std::in_place_type_t<ValueType>, std::initializer_list<Elem> Ilist, Types&&... Args)
		{
			_emplace<std::decay_t<ValueType>>(Ilist, std::forward<_Types>(Args)...);
		}

		~any() noexcept
		{
			reset();
		}

		any& operator=(const any& other)
		{
			*this = any{_That};
			return *this;
		}

		any& operator=(any&& other) noexcept
		{
			reset();
			m_data = std::move(other.m_data);
			return *this;
		}

		template <class ValueType, std::enable_if_t<std::conjunction_v<std::negation<std::is_same<std::decay_t<ValueType>, any>>, std::is_copy_constructible<std::decay_t<ValueType>>>, int> = 0>
		any& operator=(ValueType&& Value)
		{
			*this = any{std::forward<ValueType>(Value)};
			return *this;
		}

		template <class ValueType, class... Types, std::enable_if_t<std::conjunction_v<std::is_constructible<std::decay_t<ValueType>, Types...>, std::is_copy_constructible<std::decay_t<ValueType>>>, int> = 0>
		std::decay_t<ValueType>& emplace(Types&&... Args)
		{
			reset();
			return _emplace<std::decay_t<ValueType>>(std::forward<Types>(Args)...);
		}

		template <class ValueType, class Elem, class... Types, std::enable_if_t<std::conjunction_v<std::is_constructible<std::decay_t<ValueType>, std::initializer_list<Elem>&, Types...>, std::is_copy_constructible<std::decay_t<ValueType>>>, int> = 0>
		std::decay_t<ValueType>& emplace(std::initializer_list<Elem> Ilist, Types&&... Args)
		{
			reset();
			return _emplace<std::decay_t<ValueType>>(Ilist, std::forward<Types>(Args)...);
		}
		void reset() noexcept
		{
			// delete m_data;
		}

		void swap(any& other) noexcept
		{
			other = std::exchange(*this, std::move(other));
		}

		[[nodiscard]] bool has_value() const noexcept
		{
			return data != nullptr;
		}

		template <class Decayed>
		[[nodiscard]] const Decayed* cast() const noexcept
		{
			return reinterpret_cast<const Decayed*>(&m_data);
		}

		template <class Decayed>
		[[nodiscard]] Decayed* cast() noexcept
		{
			return const_cast<Decayed*>(static_cast<const any*>(this)->cast<Decayed>());
		}

	private:
		template <class Decayed, class... Types>
		Decayed& _emplace(Types&&... Args)
		{
			auto& obj = reinterpret_cast<Decayed&>(m_data);
			std::_Construct_in_place(obj, std::forward<Types>(Args)...);
			return obj;
		}
	};

	template <typename ValueType>
	inline void swap(rsl::any<sizeof(ValueType)>& left, rsl::any<sizeof(ValueType)>& right) noexcept
	{
		left.swap(right);
	}

	template <class ValueType, class... Types, std::enable_if_t<std::is_constructible_v<any<sizeof(ValueType)>, std::in_place_type_t<ValueType>, Types...>, int> = 0>
	[[nodiscard]] any<sizeof(ValueType)> make_any(Types&&... Args)
	{
		return any<sizeof(ValueType)>{std::in_place_type<ValueType>, std::forward<Types>(Args)...};
	}

	template <class ValueType, class Elem, class... Types, std::enable_if_t<std::is_constructible_v<any<sizeof(ValueType)>, std::in_place_type_t<ValueType>, std::initializer_list<Elem>&, Types...>, int> = 0>
	[[nodiscard]] any<sizeof(ValueType)> make_any(std::initializer_list<Elem> Ilist, Types&&... Args)
	{
		return any<sizeof(ValueType)>{std::in_place_type<ValueType>, Ilist, std::forward<Types>(Args)...};
	}

	template <class ValueType>
	[[nodiscard]] const ValueType* any_cast(const any<sizeof(ValueType)>* const Any) noexcept
	{
		// retrieve a pointer to the ValueType contained in _Any, or null
		static_assert(!std::is_void_v<ValueType>, "std::any cannot contain void.");

		if constexpr (std::is_function_v<ValueType> || std::is_array_v<ValueType>)
		{
			return nullptr;
		}
		else
		{
			if (!Any)
			{
				return nullptr;
			}

			return Any->cast<std::remove_cvref_t<ValueType>>();
		}
	}
	template <class ValueType>
	[[nodiscard]] ValueType* any_cast(any<sizeof(ValueType)>* const Any) noexcept
	{
		// retrieve a pointer to the _ValueType contained in _Any, or null
		static_assert(!std::is_void_v<ValueType>, "std::any cannot contain void.");

		if constexpr (std::is_function_v<ValueType> || std::is_array_v<ValueType>)
		{
			return nullptr;
		}
		else
		{
			if (!Any)
			{
				return nullptr;
			}

			return Any->cast<std::remove_cvref_t<ValueType>>();
		}
	}

	template <class Ty>
	[[nodiscard]] std::remove_cv_t<Ty> any_cast(const any<sizeof(Ty)>& Any)
	{
		static_assert(std::is_constructible_v<std::remove_cv_t<Ty>, const std::remove_cvref_t<Ty>&>, "any_cast<T>(const any&) requires remove_cv_t<T> to be constructible from "
																									 "const remove_cv_t<remove_reference_t<T>>&");

		const auto ptr = rsl::any_cast<std::emove_cvref_t<Ty>>(&Any);
		if (!ptr)
			throw_bad_any_cast();

		return static_cast<std::remove_cv_t<Ty>>(*Ptr);
	}

	template <class Ty>
	[[nodiscard]] std::remove_cv_t<Ty> any_cast(any<sizeof(Ty)>& Any)
	{
		static_assert(std::is_constructible_v<std::remove_cv_t<Ty>, std::remove_cvref_t<Ty>&>, "any_cast<T>(any&) requires remove_cv_t<T> to be constructible from remove_cv_t<remove_reference_t<T>>&");

		const auto ptr = rsl::any_cast<std::remove_cvref_t<Ty>>(&Any);
		if (!ptr)
			throw_bad_any_cast();

		return static_cast<std::remove_cv_t<Ty>>(*ptr);
	}
	template <class Ty>
	[[nodiscard]] std::remove_cv_t<Ty> any_cast(any<sizeof(Ty)>&& Any)
	{
		static_assert(std::is_constructible_v<std::remove_cv_t<Ty>, std::remove_cvref_t<Ty>>, "any_cast<T>(any&&) requires remove_cv_t<T> to be constructible from remove_cv_t<remove_reference_t<T>>");

		const auto ptr = rsl::any_cast<std::remove_cvref_t<Ty>>(&Any);
		if (!ptr)
			throw_bad_any_cast();

		return static_cast<std::remove_cv_t<Ty>>(std::move(*ptr));
	}
} // namespace rsl
