#pragma once
#include <utility>

#include "../util/type_util.hpp"

namespace rsl
{
	struct input_iterator_tag
	{
	};

	struct output_iterator_tag
	{
	};

	struct forward_iterator_tag : input_iterator_tag
	{
	};

	struct bidirectional_iterator_tag : forward_iterator_tag
	{
	};

	struct random_access_iterator_tag : bidirectional_iterator_tag
	{
	};

	struct contiguous_iterator_tag : random_access_iterator_tag
	{
	};

    template <class _Ty>
	concept weakly_incrementable = movable<_Ty> && requires(_Ty __i) {
		typename iter_difference_t<_Ty>;
		requires _Signed_integer_like<iter_difference_t<_Ty>>;
		{ ++__i } -> same_as<_Ty&>;
		__i++;
	};

    template <class _It>
	concept input_or_output_iterator = requires(_It __i) {
		{ *__i } -> _Can_reference;
	} && weakly_incrementable<_It>;

    _EXPORT_STD template <class _It>
	concept input_iterator = input_or_output_iterator<_It> && indirectly_readable<_It> && requires {
		typename _Iter_concept<_It>;
	} && derived_from<_Iter_concept<_It>, input_iterator_tag>;

	_EXPORT_STD template <class _It, class _Ty>
	concept output_iterator = input_or_output_iterator<_It> && indirectly_writable<_It, _Ty> &&
							  requires(_It __i, _Ty&& __t) { *__i++ = static_cast<_Ty&&>(__t); };

	_EXPORT_STD template <class _It>
	concept forward_iterator = input_iterator<_It> && derived_from<_Iter_concept<_It>, forward_iterator_tag> &&
							   incrementable<_It> && sentinel_for<_It, _It>;

	_EXPORT_STD template <class _It>
	concept bidirectional_iterator =
		forward_iterator<_It> && derived_from<_Iter_concept<_It>, bidirectional_iterator_tag> && requires(_It __i) {
			{ --__i } -> same_as<_It&>;
			{ __i-- } -> same_as<_It>;
		};

	_EXPORT_STD template <class _It>
	concept random_access_iterator =
		bidirectional_iterator<_It> && derived_from<_Iter_concept<_It>, random_access_iterator_tag> &&
		totally_ordered<_It> && sized_sentinel_for<_It, _It> &&
		requires(_It __i, const _It __j, const iter_difference_t<_It> __n) {
			{ __i += __n } -> same_as<_It&>;
			{ __j + __n } -> same_as<_It>;
			{ __n + __j } -> same_as<_It>;
			{ __i -= __n } -> same_as<_It&>;
			{ __j - __n } -> same_as<_It>;
			{ __j[__n] } -> same_as<iter_reference_t<_It>>;
		};

	_EXPORT_STD template <class _It>
	concept contiguous_iterator =
		random_access_iterator<_It> && derived_from<_Iter_concept<_It>, contiguous_iterator_tag> &&
		is_lvalue_reference_v<iter_reference_t<_It>> &&
		same_as<iter_value_t<_It>, remove_cvref_t<iter_reference_t<_It>>> && requires(const _It& __i) {
			{ _STD to_address(__i) } -> same_as<add_pointer_t<iter_reference_t<_It>>>;
		};

	template <typename Iter>
	struct reverse_iterator
	{
	public:
		using iterator_type = Iter;
		using iterator_concept = conditional_t<
			::std::random_access_iterator<Iter>, random_access_iterator_tag, bidirectional_iterator_tag>;
		using iterator_category = conditional_t<
			derived_from<::std::_Iter_cat_t<Iter>, random_access_iterator_tag>,
			random_access_iterator_tag, _STD _Iter_cat_t<Iter>>;

		using value_type = ::std::_Iter_value_t<Iter>;
		using difference_type = ::std::_Iter_diff_t<Iter>;
		using pointer = typename ::std::iterator_traits<Iter>::pointer;
		using reference = ::std::_Iter_ref_t<Iter>;

		template <typename>
		friend struct reverse_iterator;

		constexpr reverse_iterator() = default;

		constexpr explicit reverse_iterator(Iter _Right) noexcept(::std::is_nothrow_move_constructible_v<Iter>)
			: current(::rsl::move(_Right))
		{
		}

		template <typename _Other>
			requires(!is_same_v<_Other, Iter>) && convertible_to<const _Other&, Iter>
		constexpr reverse_iterator(const reverse_iterator<_Other>& _Right)
			noexcept(::std::is_nothrow_constructible_v<Iter, const _Other&>)
			: current(_Right.current)
		{
		}

		template <typename _Other>
			requires(!is_same_v<_Other, Iter>) && convertible_to<const _Other&, Iter> &&
					assignable_from<Iter&, const _Other&>
		constexpr reverse_iterator& operator=(const reverse_iterator<_Other>& _Right)
			noexcept(::std::is_nothrow_assignable_v<Iter&, const _Other&>)
		{
			current = _Right.current;
			return *this;
		}

		[[nodiscard]] constexpr Iter base() const noexcept(::std::is_nothrow_copy_constructible_v<Iter>)
		{
			return current;
		}

		[[nodiscard]] constexpr reference operator*() const
			noexcept(::std::is_nothrow_copy_constructible_v<Iter> && noexcept(*--(::rsl::declval<Iter&>())))
		{
			Iter _Tmp = current;
			return *--_Tmp;
		}

		[[nodiscard]] constexpr pointer operator->() const
			noexcept(::std::is_nothrow_copy_constructible_v<Iter> && noexcept(--(::rsl::declval<Iter&>())) && _STD _Has_nothrow_operator_arrow<Iter&, pointer>)
			requires(is_pointer_v<Iter> || requires(const Iter __i) { __i.operator->(); })

		{
			Iter _Tmp = current;
			--_Tmp;
			if constexpr (is_pointer_v<Iter>)
			{
				return _Tmp;
			}
			else
			{
				return _Tmp.operator->();
			}
		}

		constexpr reverse_iterator& operator++() noexcept(noexcept(--current))
		{
			--current;
			return *this;
		}

		constexpr reverse_iterator operator++(int)
			noexcept(::std::is_nothrow_copy_constructible_v<Iter> && noexcept(--current))
		{
			reverse_iterator _Tmp = *this;
			--current;
			return _Tmp;
		}

		constexpr reverse_iterator& operator--() noexcept(noexcept(++current))
		{
			++current;
			return *this;
		}

		constexpr reverse_iterator operator--(int)
			noexcept(::std::is_nothrow_copy_constructible_v<Iter> && noexcept(++current))
		{
			reverse_iterator _Tmp = *this;
			++current;
			return _Tmp;
		}

		[[nodiscard]] constexpr reverse_iterator operator+(const difference_type _Off) const
			noexcept(noexcept(reverse_iterator(current - _Off)))
		{
			return reverse_iterator(current - _Off);
		}

		constexpr reverse_iterator& operator+=(const difference_type _Off) noexcept(noexcept(current -= _Off))
		{
			current -= _Off;
			return *this;
		}

		[[nodiscard]] constexpr reverse_iterator operator-(const difference_type _Off) const
			noexcept(noexcept(reverse_iterator(current + _Off)))
		{
			return reverse_iterator(current + _Off);
		}

		constexpr reverse_iterator& operator-=(const difference_type _Off) noexcept(noexcept(current += _Off))
		{
			current += _Off;
			return *this;
		}

		[[nodiscard]] constexpr reference operator[](const difference_type _Off) const
			noexcept(noexcept(::rsl::fake_copy_init<reference>(current[_Off])))
		{
			return current[static_cast<difference_type>(-_Off - 1)];
		}

		[[nodiscard]] friend constexpr _STD iter_rvalue_reference_t<Iter> iter_move(const reverse_iterator& _It)
			noexcept(
			::std::is_nothrow_copy_constructible_v<Iter> && noexcept(_RANGES iter_move(--::rsl::declval<Iter&>()))
		)
		{
			auto _Tmp = _It.current;
			--_Tmp;
			return _RANGES iter_move(_Tmp);
		}

		template <_STD indirectly_swappable<Iter> Iter2>
		friend constexpr void iter_swap(const reverse_iterator& _Left, const reverse_iterator<Iter2>& _Right) noexcept(
			::std::is_nothrow_copy_constructible_v<Iter> && ::std::is_nothrow_copy_constructible_v<Iter2> &&
			noexcept(_RANGES iter_swap(--::rsl::declval<Iter&>(), --::rsl::declval<Iter2&>()))
		)
		{
			auto _LTmp = _Left.current;
			auto _RTmp = _Right.base();
			--_LTmp;
			--_RTmp;
			_RANGES iter_swap(_LTmp, _RTmp);
		}

		using _Prevent_inheriting_unwrap = reverse_iterator;

		template <typename Iter2, enable_if_t<_STD _Range_verifiable_v<Iter, Iter2>, int> = 0>
		friend constexpr void
		_Verify_range(const reverse_iterator& _First, const reverse_iterator<Iter2>& _Last) noexcept
		{
			_Verify_range(_Last._Get_current(), _First.current); // note reversed parameters
		}

		template <typename Iter2 = Iter, enable_if_t<_STD _Offset_verifiable_v<Iter2>, int> = 0>
		constexpr void _Verify_offset(const difference_type _Off) const noexcept
		{
			_STL_VERIFY(_Off != _STD _Min_possible_v<difference_type>, "integer overflow");
			current._Verify_offset(-_Off);
		}

		template <typename Iter2 = Iter, enable_if_t<_STD _Unwrappable_v<const Iter2&>, int> = 0>
		[[nodiscard]] constexpr reverse_iterator<_STD _Unwrapped_t<const Iter2&>> _Unwrapped() const& noexcept(
			noexcept(static_cast<reverse_iterator<_STD _Unwrapped_t<const Iter2&>>>(current._Unwrapped()))
		)
		{
			return static_cast<reverse_iterator<_STD _Unwrapped_t<const Iter2&>>>(current._Unwrapped());
		}
		template <typename Iter2 = Iter, enable_if_t<_STD _Unwrappable_v<Iter2>, int> = 0>
		[[nodiscard]] constexpr reverse_iterator<_STD _Unwrapped_t<Iter2>> _Unwrapped() && noexcept(
			noexcept(static_cast<reverse_iterator<_STD _Unwrapped_t<Iter2>>>(::rsl::move(current)._Unwrapped()))
		)
		{
			return static_cast<reverse_iterator<_STD _Unwrapped_t<Iter2>>>(::rsl::move(current)._Unwrapped());
		}

		static constexpr bool _Unwrap_when_unverified = _STD _Do_unwrap_when_unverified_v<Iter>;

		template <typename _Src, enable_if_t<_STD _Wrapped_seekable_v<Iter, const _Src&>, int> = 0>
		constexpr void _Seek_to(const reverse_iterator<_Src>& _It) noexcept(noexcept(current._Seek_to(_It.current)))
		{
			current._Seek_to(_It.current);
		}

		[[nodiscard]] constexpr const Iter& _Get_current() const noexcept { return current; }

	protected:
		Iter current{};
	};

	template <typename Iter1, typename Iter2>
	[[nodiscard]] constexpr bool operator==(const reverse_iterator<Iter1>& _Left, const reverse_iterator<Iter2>& _Right)
		noexcept(noexcept(::rsl::fake_copy_init<bool>(_Left._Get_current() == _Right._Get_current())))
		requires requires {
			{ _Left._Get_current() == _Right._Get_current() } -> _STD _Implicitly_convertible_to<bool>;
		}

	{
		return _Left._Get_current() == _Right._Get_current();
	}

	template <typename Iter1, typename Iter2>
	[[nodiscard]] constexpr bool operator!=(const reverse_iterator<Iter1>& _Left, const reverse_iterator<Iter2>& _Right)
		noexcept(noexcept(::rsl::fake_copy_init<bool>(_Left._Get_current() != _Right._Get_current())))
		requires requires {
			{ _Left._Get_current() != _Right._Get_current() } -> _STD _Implicitly_convertible_to<bool>;
		}

	{
		return _Left._Get_current() != _Right._Get_current();
	}

	template <typename Iter1, typename Iter2>
	[[nodiscard]] constexpr bool operator<(const reverse_iterator<Iter1>& _Left, const reverse_iterator<Iter2>& _Right)
		noexcept(noexcept(::rsl::fake_copy_init<bool>(_Left._Get_current() > _Right._Get_current())))
		requires requires {
			{ _Left._Get_current() > _Right._Get_current() } -> _STD _Implicitly_convertible_to<bool>;
		}

	{
		return _Left._Get_current() > _Right._Get_current();
	}

	template <typename Iter1, typename Iter2>
	[[nodiscard]] constexpr bool operator>(const reverse_iterator<Iter1>& _Left, const reverse_iterator<Iter2>& _Right)
		noexcept(noexcept(::rsl::fake_copy_init<bool>(_Left._Get_current() < _Right._Get_current())))
		requires requires {
			{ _Left._Get_current() < _Right._Get_current() } -> _STD _Implicitly_convertible_to<bool>;
		}

	{
		return _Left._Get_current() < _Right._Get_current();
	}

	template <typename Iter1, typename Iter2>
	[[nodiscard]] constexpr bool operator<=(const reverse_iterator<Iter1>& _Left, const reverse_iterator<Iter2>& _Right)
		noexcept(noexcept(::rsl::fake_copy_init<bool>(_Left._Get_current() >= _Right._Get_current())))
		requires requires {
			{ _Left._Get_current() >= _Right._Get_current() } -> _STD _Implicitly_convertible_to<bool>;
		}

	{
		return _Left._Get_current() >= _Right._Get_current();
	}

	template <typename Iter1, typename Iter2>
	[[nodiscard]] constexpr bool operator>=(const reverse_iterator<Iter1>& _Left, const reverse_iterator<Iter2>& _Right)
		noexcept(noexcept(::rsl::fake_copy_init<bool>(_Left._Get_current() <= _Right._Get_current())))
		requires requires {
			{ _Left._Get_current() <= _Right._Get_current() } -> _STD _Implicitly_convertible_to<bool>;
		}

	{
		return _Left._Get_current() <= _Right._Get_current();
	}

	template <typename Iter1, _STD three_way_comparable_with<Iter1> Iter2>
	[[nodiscard]] constexpr _STD compare_three_way_result_t<Iter1, Iter2>
	operator<=>(const reverse_iterator<Iter1>& _Left, const reverse_iterator<Iter2>& _Right)
		noexcept(noexcept(_Right._Get_current() <=> _Left._Get_current()))
	{
		return _Right._Get_current() <=> _Left._Get_current();
	}

	template <typename Iter1, typename Iter2>
	[[nodiscard]] constexpr auto operator-(const reverse_iterator<Iter1>& _Left, const reverse_iterator<Iter2>& _Right)
		noexcept(noexcept(_Right._Get_current() - _Left._Get_current())
		) -> decltype(_Right._Get_current() - _Left._Get_current())
	{
		return _Right._Get_current() - _Left._Get_current();
	}

	template <typename Iter>
	[[nodiscard]] constexpr reverse_iterator<Iter>
	operator+(typename reverse_iterator<Iter>::difference_type _Off, const reverse_iterator<Iter>& _Right)
		noexcept(noexcept(_Right + _Off))
	{
		return _Right + _Off;
	}

	template <typename Iter>
	[[nodiscard]] constexpr reverse_iterator<Iter> make_reverse_iterator(Iter _Iter)
		noexcept(::std::is_nothrow_move_constructible_v<Iter>)
	{
		return reverse_iterator<Iter>(::rsl::move(_Iter));
	}
} // namespace rsl
