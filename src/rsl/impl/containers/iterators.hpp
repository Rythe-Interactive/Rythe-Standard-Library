#pragma once
#include "../util/concepts.hpp"
#include "../util/type_util.hpp"
#include "pair.hpp"

namespace rsl
{
	namespace internal
	{
		template <typename T>
		concept has_member_difference_type = requires { typename T::difference_type; };

		template <typename T>
		concept can_difference = requires(const T& lhs, const T& rhs) {
			{ lhs - rhs } -> integral_type;
		};
	} // namespace internal

	template <typename>
	struct incrementable_traits
	{
	};

	template <typename T>
		requires is_object_v<T>
	struct incrementable_traits<T*>
	{
		using difference_type = diff_type;
	};

	template <typename T>
	struct incrementable_traits<const T> : incrementable_traits<T>
	{
	};

	template <internal::has_member_difference_type T>
	struct incrementable_traits<T>
	{
		using difference_type = typename T::difference_type;
	};

	template <typename T>
		requires(!internal::has_member_difference_type<T> && internal::can_difference<T>)
	struct incrementable_traits<T>
	{
		using difference_type = make_signed_t<decltype(declval<T>() - declval<T>())>;
	};

	template <typename T>
	using iter_difference_t = typename incrementable_traits<T>::difference_type;

	namespace internal
	{
		template <typename T>
		concept signed_integer_like = is_nonbool_integral_v<remove_cv_t<T>> && static_cast<T>(-1) < static_cast<T>(0);

		template <typename T>
		using with_reference = T&;

		template <typename T>
		concept can_reference = requires { typename with_reference<T>; };

		template <typename T>
		concept dereferenceable = requires(T& val) {
			{ *val } -> can_reference;
		};

		template <typename T>
		concept pointable = requires(T& val) {
			{ val.operator->() } -> dereferenceable;
		} || (is_pointer_v<T>);

		template <typename>
		struct cond_value_type
		{
		};

		template <typename T>
			requires is_object_v<T>
		struct cond_value_type<T>
		{
			using value_type = remove_cv_t<T>;
		};
	} // namespace internal

	template <typename>
	struct indirectly_readable_traits
	{
	};

	template <typename T>
		requires is_object_v<T>
	struct indirectly_readable_traits<T*>
	{
		using value_type = remove_cv_t<T>;
	};

	template <typename T>
		requires is_array_v<T>
	struct indirectly_readable_traits<T>
	{
		using value_type = remove_cv_t<remove_extent_t<T>>;
	};

	template <typename T>
	struct indirectly_readable_traits<const T> : indirectly_readable_traits<T>
	{
	};

	template <typename T>
		requires requires { typename T::value_type; } && is_object_v<typename T::value_type>
	struct indirectly_readable_traits<T>
	{
		using value_type = remove_cv_t<typename T::value_type>;
	};

	template <typename T>
		requires requires { typename T::element_type; } && is_object_v<typename T::element_type>
	struct indirectly_readable_traits<T>
	{
		using value_type = remove_cv_t<typename T::element_type>;
	};

	template <typename T>
		requires requires {
			typename T::value_type;
			typename T::element_type;
		}
	struct indirectly_readable_traits<T>
	{
	};

	template <typename T>
		requires requires {
			typename T::value_type;
			typename T::element_type;
		} && same_as<remove_cv_t<typename T::value_type>, remove_cv_t<typename T::element_type>> &&
				 is_object_v<typename T::value_type>
	struct indirectly_readable_traits<T>
	{
		using value_type = remove_cv_t<typename T::value_type>;
	};

	template <typename T>
	using iter_value_t = typename indirectly_readable_traits<remove_cvr_t<T>>::value_type;

	template <internal::dereferenceable T>
	using iter_reference_t = decltype(*declval<T&>());

	template <internal::pointable T>
	using iter_pointer_t = decltype(&*declval<T&>());

	namespace internal
	{
		template <typename T>
		concept indirectly_readable_impl = requires(const T iter) {
			typename iter_value_t<T>;
			typename iter_reference_t<T>;
			{ *iter } -> same_as<iter_reference_t<T>>;
		} && common_reference_with<iter_reference_t<T>&&, iter_value_t<T>&>;
	} // namespace internal

	template <typename T>
	concept indirectly_readable = internal::indirectly_readable_impl<remove_cvr_t<T>>;

	template <typename It, typename T>
	concept indirectly_writable = requires(It&& iter, T&& val) {
		*iter = static_cast<T&&>(val);
		*static_cast<It&&>(iter) = static_cast<T&&>(val);
		const_cast<const iter_reference_t<It>&&>(*iter) = static_cast<T&&>(val);
		const_cast<const iter_reference_t<It>&&>(*static_cast<It&&>(iter)) = static_cast<T&&>(val);
	};

	template <typename T>
	concept weakly_incrementable = movable<T> && requires(T i) {
		typename iter_difference_t<T>;
		requires internal::signed_integer_like<iter_difference_t<T>>;
		{ ++i } -> same_as<T&>;
		i++;
	};

	template <typename T>
	concept incrementable = regular<T> && weakly_incrementable<T> && requires(T val) {
		{ val++ } -> same_as<T>;
	};

	template <typename It>
	concept input_or_output_iterator = internal::dereferenceable<It> && internal::pointable<It> && weakly_incrementable<It>;

	template <typename Se, typename It>
	concept sentinel_for =
		semiregular<Se> && input_or_output_iterator<It> && internal::weakly_equality_comparable_with<Se, It>;

	template <typename Se, typename It>
	constexpr bool disable_sized_sentinel_for = false;

	template <typename Se, typename It>
	concept sized_sentinel_for =
		sentinel_for<Se, It> && !disable_sized_sentinel_for<remove_cv_t<Se>, remove_cv_t<It>> &&
		requires(const It& iter, const Se& sent) {
			{ sent - iter } -> same_as<iter_difference_t<It>>;
			{ iter - sent } -> same_as<iter_difference_t<It>>;
		};

	template <typename It>
	concept input_iterator = input_or_output_iterator<It> && indirectly_readable<It>;

	template <typename It, typename T>
	concept output_iterator = input_or_output_iterator<It> && indirectly_writable<It, T> &&
							  requires(It iter, T&& val) { *iter++ = static_cast<T&&>(val); };

	template <typename It>
	concept forward_iterator = input_iterator<It> && incrementable<It> && sentinel_for<It, It>;

	template <typename It>
	concept bidirectional_iterator = forward_iterator<It> && requires(It iter) {
		{ --iter } -> same_as<It&>;
		{ iter-- } -> same_as<It>;
	};

	template <typename It>
	concept random_access_iterator = bidirectional_iterator<It> && totally_ordered<It> && sized_sentinel_for<It, It> &&
									 requires(It iter, const It constIter, const iter_difference_t<It> n) {
										 { iter += n } -> same_as<It&>;
										 { constIter + n } -> same_as<It>;
										 { iter -= n } -> same_as<It&>;
										 { constIter - n } -> same_as<It>;
										 { constIter[n] } -> same_as<iter_reference_t<It>>;
									 };

	template <typename It>
	concept contiguous_iterator =
		random_access_iterator<It> && is_lvalue_reference_v<iter_reference_t<It>> &&
		same_as<iter_value_t<It>, remove_cvr_t<iter_reference_t<It>>> && requires(const It& iter) {
			{ rsl::to_address(iter) } -> same_as<add_pointer_t<iter_reference_t<It>>>;
		};

	namespace internal
	{
		template <typename It>
		struct iterator_diff_impl;

		template <forward_iterator It>
		struct iterator_diff_impl<It>
		{
			[[nodiscard]] [[rythe_always_inline]] constexpr size_type operator()(It first, It last) const noexcept
			{
				size_type diff = 0;
				while (first != last)
				{
					++first;
					++diff;
				}

				return diff;
			}
		};

		template <typename It>
			requires sized_sentinel_for<It, It>
		struct iterator_diff_impl<It>
		{
			[[nodiscard]] [[rythe_always_inline]] constexpr size_type operator()(It first, It last) const noexcept
			{
				return last - first;
			}
		};
	} // namespace internal

	template <typename It>
	[[nodiscard]] [[rythe_always_inline]] constexpr size_type iterator_diff(It first, It last) noexcept
	{
		return internal::iterator_diff_impl<It>{}(first, last);
	}

	template <bidirectional_iterator It>
	class reverse_iterator
	{
	public:
		using underlying_iter = It;
		using difference_type = iter_difference_t<It>;

		using ref_type = iter_reference_t<underlying_iter>;
		using ptr_type = iter_pointer_t<underlying_iter>;

		constexpr reverse_iterator() noexcept = default;
		constexpr reverse_iterator(nullptr_type) noexcept {};

		constexpr explicit reverse_iterator(const underlying_iter& iter) noexcept : m_iter(iter) {}
		constexpr explicit reverse_iterator(underlying_iter&& iter) noexcept : m_iter(rsl::move(iter)) {}

		template <bidirectional_iterator OtherNodeIter>
		constexpr reverse_iterator(const reverse_iterator<OtherNodeIter>& other) noexcept
			requires constructible_from<underlying_iter, OtherNodeIter> && not_same_as<underlying_iter, OtherNodeIter>
			: m_iter(other.m_iter)
		{
		}

		template <bidirectional_iterator OtherNodeIter>
		constexpr reverse_iterator(reverse_iterator<OtherNodeIter>&& other) noexcept
			requires constructible_from<underlying_iter, OtherNodeIter&&> && not_same_as<underlying_iter, OtherNodeIter>
			: m_iter(rsl::move(other.m_iter))
		{
		}

		template <bidirectional_iterator OtherNodeIter>
		constexpr reverse_iterator& operator=(const reverse_iterator<OtherNodeIter>& other) noexcept
			requires assignable_from<underlying_iter, OtherNodeIter> && not_same_as<underlying_iter, OtherNodeIter>
		{
			m_iter = other.m_iter;
			return *this;
		}

		template <bidirectional_iterator OtherNodeIter>
		constexpr reverse_iterator& operator=(reverse_iterator<OtherNodeIter>&& other) noexcept
			requires assignable_from<underlying_iter, OtherNodeIter&&> && not_same_as<underlying_iter, OtherNodeIter>
		{
			m_iter = rsl::move(other.m_iter);
			return *this;
		}

		constexpr reverse_iterator& operator+=(const difference_type offset) noexcept
			requires random_access_iterator<underlying_iter>
		{
			m_iter -= offset;
			return *this;
		}

		constexpr reverse_iterator operator+(const difference_type offset) const noexcept
			requires random_access_iterator<underlying_iter>
		{
			return reverse_iterator(m_iter - offset);
		}

		constexpr reverse_iterator& operator++() noexcept
		{
			--m_iter;
			return *this;
		}

		constexpr reverse_iterator operator++(int) noexcept
		{
			reverse_iterator tmp = *this;
			++(*this);
			return tmp;
		}

		constexpr reverse_iterator& operator-=(const difference_type offset) noexcept
			requires random_access_iterator<underlying_iter>
		{
			m_iter += offset;
			return *this;
		}

		constexpr reverse_iterator operator-(const difference_type offset) const noexcept
			requires random_access_iterator<underlying_iter>
		{
			return reverse_iterator(m_iter + offset);
		}

		constexpr reverse_iterator& operator--() noexcept
		{
			++m_iter;
			return *this;
		}

		constexpr reverse_iterator operator--(int) noexcept
		{
			reverse_iterator tmp = *this;
			--(*this);
			return tmp;
		}

		constexpr ref_type operator[](const difference_type n) const noexcept
			requires random_access_iterator<underlying_iter>
		{
			return m_iter[-n];
		}

		constexpr ref_type operator*() const noexcept { underlying_iter tmp = m_iter; return *--tmp; }
		constexpr ptr_type operator->() const noexcept { return &operator*(); }

		template <bidirectional_iterator OtherNodeIter>
			requires sentinel_for<underlying_iter, OtherNodeIter>
		constexpr bool operator==(const reverse_iterator<OtherNodeIter>& other) const noexcept
		{
			return m_iter == other.m_iter;
		}

		template <bidirectional_iterator OtherNodeIter>
			requires sentinel_for<underlying_iter, OtherNodeIter>
		constexpr bool operator!=(const reverse_iterator<OtherNodeIter>& other) const noexcept
		{
			return m_iter != other.m_iter;
		}

	private:
		underlying_iter m_iter{nullptr};
	};

	template <class T>
	struct pair_range
	{
		using iterator = T;
		pair_range(const pair<T, T> r) noexcept
			: range(r)
		{
		}

		template <typename ItType>
		pair_range(ItType begin, ItType end) noexcept
			: range(move(begin), move(end))
		{
		}

		[[nodiscard]] [[rythe_always_inline]] auto& begin() const { return range.first; }

		[[nodiscard]] [[rythe_always_inline]] auto& end() const { return range.second; }
		pair<T, T> range;
	};

	template <class T>
	pair_range(pair<T, T>) -> pair_range<T>;

	template <class T>
	pair_range(T begin, T end) -> pair_range<remove_reference_t<T>>;

	template <class It>
	bool checked_next(It& iter, It end, size_type diff)
	{
		while (diff-- > 0)
		{
			if (iter == end)
			{
				return false;
			}
			++iter;
		}
		return true;
	}

	template <typename KeysIterator, typename ValuesIterator>
	class key_value_pair_iterator
	{
	public:
		using keys_proxy_type = KeysIterator;
		using values_proxy_type = ValuesIterator;
		using key_type = typename keys_proxy_type::value_type;
		using value_type = typename values_proxy_type::value_type;
		using pair_type = pair<key_type&, value_type&>;
		using const_pair_type = pair<const key_type&, const value_type&>;

		explicit key_value_pair_iterator(keys_proxy_type keys, values_proxy_type values)
			: m_key(keys),
			  m_value(values)
		{
		}

		pair_type operator*() { return {ref(key()), ref(value())}; }
		const_pair_type operator*() const { return {cref(key()), cref(value())}; }

		pair_type operator->() { return {ref(key()), ref(value())}; }
		const_pair_type operator->() const { return {cref(key()), cref(value())}; }

		auto& key() { return *m_key; }
		const auto& key() const { return *m_key; }
		auto& value() { return *m_value; }
		const auto& value() const { return *m_value; }

		friend auto operator==(const key_value_pair_iterator& rhs, const key_value_pair_iterator& lhs)
		{
			return rhs.m_key == lhs.m_key && rhs.m_value == lhs.m_value;
		}
		friend auto operator!=(const key_value_pair_iterator& rhs, const key_value_pair_iterator& lhs)
		{
			return rhs.m_key != lhs.m_key || rhs.m_value != lhs.m_value;
		}

		auto& operator++() noexcept
		{
			++m_key;
			++m_value;
			return *this;
		}

		auto operator++(int) noexcept { return key_value_pair_iterator(++m_key, ++m_value); }

		auto& operator--() noexcept
		{
			--m_key;
			--m_value;
			return *this;
		}

		auto operator--(int) noexcept { return key_value_pair_iterator(--m_key, --m_value); }

	private:
		keys_proxy_type m_key;
		values_proxy_type m_value;
	};

	template <class PairIteratorContainer>
	class key_only_iterator
	{
	public:
		using self_proxy_type = typename PairIteratorContainer::iterator;
		explicit key_only_iterator(self_proxy_type self)
			: m_self(self)
		{
		}

		auto& operator*() { return key(); }
		const auto& operator*() const { return key(); }

		auto& operator->() { return key(); }
		const auto& operator->() const { return key(); }
		auto& key() { return m_self->first; }
		const auto& key() const { return m_self->first; }
		friend auto operator==(const key_only_iterator& rhs, const key_only_iterator& lhs)
		{
			return rhs.m_self == lhs.m_self;
		}
		friend auto operator!=(const key_only_iterator& rhs, const key_only_iterator& lhs)
		{
			return rhs.m_self != lhs.m_self;
		}

		auto operator++() { return key_only_iterator(m_self++); }
		auto operator++() const { return key_only_iterator(m_self++); }

	private:
		self_proxy_type m_self;
	};

	template <class PairIteratorContainer>
	class value_only_iterator
	{
	public:
		using self_proxy_type = typename PairIteratorContainer::iterator;
		explicit value_only_iterator(self_proxy_type self)
			: m_self(move(self))
		{
		}

		auto& operator*() { return value(); }
		const auto& operator*() const { return value(); }

		auto& operator->() { return value(); }
		const auto& operator->() const { return value(); }
		auto& value() { return m_self->second; }
		const auto& value() const { return m_self->second; }
		auto operator++() { return value_only_iterator(m_self++); }
		auto operator++() const { return value_only_iterator(m_self++); }
		friend auto operator==(const value_only_iterator& rhs, const value_only_iterator& lhs)
		{
			return rhs.m_self == lhs.m_self;
		}
		friend auto operator!=(const value_only_iterator& rhs, const value_only_iterator& lhs)
		{
			return rhs.m_self != lhs.m_self;
		}

	private:
		self_proxy_type m_self;
	};

	template <class PairIteratorContainer>
	class keys_only_view
	{
	public:
		explicit keys_only_view(PairIteratorContainer& cont)
			: m_container(cont)
		{
		}
		[[nodiscard]] auto begin() const { return key_only_iterator<PairIteratorContainer>(m_container.begin()); }
		[[nodiscard]] auto end() const { return key_only_iterator<PairIteratorContainer>(m_container.end()); }

	private:
		PairIteratorContainer& m_container;
	};

	template <class PairIteratorContainer>
	keys_only_view(PairIteratorContainer&) -> keys_only_view<PairIteratorContainer>;

	template <class PairIteratorContainer>
	class values_only_view
	{
	public:
		explicit values_only_view(PairIteratorContainer& cont)
			: m_container(cont)
		{
		}

		[[nodiscard]] auto begin() const { return value_only_iterator<PairIteratorContainer>(m_container.begin()); }
		[[nodiscard]] auto end() const { return value_only_iterator<PairIteratorContainer>(m_container.end()); }

	private:
		PairIteratorContainer& m_container;
	};

	template <class PairIteratorContainer>
	values_only_view(PairIteratorContainer&) -> values_only_view<PairIteratorContainer>;
} // namespace rsl
