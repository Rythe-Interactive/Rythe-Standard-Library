#include "../defines.hpp"
#include "../util/primitives.hpp"
#include "../util/type_util.hpp"

namespace rsl
{
	inline constexpr size_type dynamic_extent = static_cast<size_type>(-1);

    template <typename T, size_type Extent>
	class span;

	template <typename>
	inline constexpr bool is_span_v = false;

	template <typename T, size_type Extent>
	inline constexpr bool is_span_v<span<T, Extent>> = true;

    namespace internal
	{
		template <typename T, size_type Extent>
		struct span_base
		{
			using pointer = T*;

			constexpr span_base() noexcept = default;

			constexpr explicit span_base(const pointer data, size_type) noexcept
				: m_data{data}
			{
			}

			pointer m_data{nullptr};
			static constexpr size_type m_size = Extent;
		};

		template <typename T>
		struct span_base<T, dynamic_extent>
		{
			using pointer = T*;

			constexpr span_base() noexcept = default;

			constexpr explicit span_base(const pointer data, const size_type size) noexcept
				: m_data{data},
				  m_size{size}
			{
			}

			pointer m_data{nullptr};
			size_type m_size{0};
		};

		// clang-format off
        template <typename _It, typename _Ty>
		concept span_compatible_iterator =
            std::contiguous_iterator<_It>
         && std::is_convertible_v<remove_reference_t<std::iter_reference_t<_It>> (*)[], _Ty (*)[]>;

		template <typename _Sentinel, typename _It>
		concept span_compatible_sentinel =
            std::sized_sentinel_for<_Sentinel, _It>
         && !std::is_convertible_v<_Sentinel, size_t>;
        
        template <typename>
        inline constexpr bool is_std_array_v = false;

        template <typename T, size_type Size>
        inline constexpr bool is_std_array_v<std::array<T, Size>> = true;

		template <typename _Rng, typename _Ty>
		concept span_compatible_range =
			!is_array_v<remove_cvr_t<_Rng>>
         && !is_span_v<remove_cvr_t<_Rng>>
         && !is_std_array_v<remove_cvr_t<_Rng>>
         && std::ranges::contiguous_range<_Rng>
         && std::ranges::sized_range<_Rng>
         && (std::ranges::borrowed_range<_Rng> || is_const_v<_Ty>)
         && std::is_convertible_v<remove_reference_t<std::ranges::range_reference_t<_Rng>> (*)[], _Ty (*)[]>;
		// clang-format on
	} // namespace internal

	template <typename T, size_type Extent = dynamic_extent>
	class span : private internal::span_base<T, Extent>
	{
	private:
		using _Mybase = internal::span_base<T, Extent>;
		using _Mybase::m_data;
		using _Mybase::m_size;

	public:
		using element_type = T;
		using value_type = remove_cv_t<T>;
		using size_type = size_type;
		using difference_type = ptrdiff_t;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using iterator = remove_cv_t<T>*;
		using reverse_iterator = std::reverse_iterator<iterator>;

		static constexpr size_type extent = Extent;

		// [span.cons] Constructors, copy, and assignment

		constexpr span() noexcept
			requires(Extent == 0 || Extent == dynamic_extent)
		= default;

		template <internal::span_compatible_iterator<element_type> _It>
		constexpr explicit(Extent != dynamic_extent) span(_It _First, size_type _Count) noexcept // strengthened
			: _Mybase(_STD to_address(_Get_unwrapped_n(_First, _Count)), _Count)
		{
#if _CONTAINER_DEBUG_LEVEL > 0
			if constexpr (Extent != dynamic_extent)
			{
				_STL_VERIFY(_Count == Extent, "Cannot construct span with static extent from range [first, first + count) as count != extent");
			}
#endif // _CONTAINER_DEBUG_LEVEL > 0
		}

		template <internal::span_compatible_iterator<element_type> _It, internal::span_compatible_sentinel<_It> _Sentinel>
		constexpr explicit(Extent != dynamic_extent) span(_It _First, _Sentinel _Last)
			noexcept(noexcept(_Last - _First)) // strengthened
			: _Mybase(_STD to_address(_First), static_cast<size_type>(_Last - _First))
		{
			_Adl_verify_range(_First, _Last);
#if _CONTAINER_DEBUG_LEVEL > 0
			if constexpr (Extent != dynamic_extent)
			{
				_STL_VERIFY(_Last - _First == Extent, "Cannot construct span with static extent from range [first, last) as last - first != extent");
			}
#endif // _CONTAINER_DEBUG_LEVEL > 0
		}

		template <size_type _Size>
			requires(Extent == dynamic_extent || Extent == _Size)
		constexpr span(type_identity_t<element_type> (&_Arr)[_Size]) noexcept
			: _Mybase(_Arr, _Size)
		{
		}

		template <class _OtherTy, size_type _Size>
			requires(Extent == dynamic_extent || Extent == _Size) && is_convertible_v<_OtherTy (*)[], element_type (*)[]>
		constexpr span(array<_OtherTy, _Size>& _Arr) noexcept
			: _Mybase(_Arr.data(), _Size)
		{
		}

		template <class _OtherTy, size_type _Size>
			requires(Extent == dynamic_extent || Extent == _Size) && is_convertible_v<const _OtherTy (*)[], element_type (*)[]>
		constexpr span(const array<_OtherTy, _Size>& _Arr) noexcept
			: _Mybase(_Arr.data(), _Size)
		{
		}

		template <internal::span_compatible_range<element_type> _Rng>
		constexpr explicit(Extent != dynamic_extent) span(_Rng&& _Range)
			: _Mybase(_RANGES data(_Range), static_cast<size_type>(_RANGES size(_Range)))
		{
#if _CONTAINER_DEBUG_LEVEL > 0
			if constexpr (Extent != dynamic_extent)
			{
				_STL_VERIFY(_RANGES size(_Range) == Extent, "Cannot construct span with static extent from range r as std::ranges::size(r) != extent");
			}
#endif // _CONTAINER_DEBUG_LEVEL > 0
		}

		template <class _OtherTy, size_type _OtherExtent>
			requires(Extent == dynamic_extent || _OtherExtent == dynamic_extent || Extent == _OtherExtent) && is_convertible_v<_OtherTy (*)[], element_type (*)[]>
		constexpr explicit(Extent != dynamic_extent && _OtherExtent == dynamic_extent)
			span(const span<_OtherTy, _OtherExtent>& _Other) noexcept
			: _Mybase(_Other.data(), _Other.size())
		{
#if _CONTAINER_DEBUG_LEVEL > 0
			if constexpr (Extent != dynamic_extent)
			{
				_STL_VERIFY(_Other.size() == Extent, "Cannot construct span with static extent from other span as other.size() != extent");
			}
#endif // _CONTAINER_DEBUG_LEVEL > 0
		}

		// [span.sub] Subviews
		template <size_type _Count>
		_NODISCARD constexpr auto first() const noexcept /* strengthened */
		{
			if constexpr (Extent != dynamic_extent)
			{
				static_assert(_Count <= Extent, "Count out of range in span::first()");
			}
#if _CONTAINER_DEBUG_LEVEL > 0
			else
			{
				_STL_VERIFY(_Count <= m_size, "Count out of range in span::first()");
			}
#endif // _CONTAINER_DEBUG_LEVEL > 0
			return span<element_type, _Count>{m_data, _Count};
		}

		_NODISCARD constexpr auto first(const size_type _Count) const noexcept
		/* strengthened */ {
#if _CONTAINER_DEBUG_LEVEL > 0
			_STL_VERIFY(_Count <= m_size, "Count out of range in span::first(count)");
#endif // _CONTAINER_DEBUG_LEVEL > 0
			return span<element_type, dynamic_extent>{m_data, _Count};
		}

		template <size_type _Count>
		_NODISCARD constexpr auto last() const noexcept /* strengthened */
		{
			if constexpr (Extent != dynamic_extent)
			{
				static_assert(_Count <= Extent, "Count out of range in span::last()");
			}
#if _CONTAINER_DEBUG_LEVEL > 0
			else
			{
				_STL_VERIFY(_Count <= m_size, "Count out of range in span::last()");
			}
#endif // _CONTAINER_DEBUG_LEVEL > 0
			return span<element_type, _Count>{m_data + (m_size - _Count), _Count};
		}

		_NODISCARD constexpr auto last(const size_type _Count) const noexcept /* strengthened */
		{
#if _CONTAINER_DEBUG_LEVEL > 0
			_STL_VERIFY(_Count <= m_size, "Count out of range in span::last(count)");
#endif // _CONTAINER_DEBUG_LEVEL > 0
			return span<element_type, dynamic_extent>{m_data + (m_size - _Count), _Count};
		}

		template <size_type _Offset, size_type _Count = dynamic_extent>
		_NODISCARD constexpr auto subspan() const noexcept /* strengthened */
		{
			if constexpr (Extent != dynamic_extent)
			{
				static_assert(_Offset <= Extent, "Offset out of range in span::subspan()");
				static_assert(
					_Count == dynamic_extent || _Count <= Extent - _Offset, "Count out of range in span::subspan()"
				);
			}
#if _CONTAINER_DEBUG_LEVEL > 0
			else
			{
				_STL_VERIFY(_Offset <= m_size, "Offset out of range in span::subspan()");

				if constexpr (_Count != dynamic_extent)
				{
					_STL_VERIFY(_Count <= m_size - _Offset, "Count out of range in span::subspan()");
				}
			}
#endif // _CONTAINER_DEBUG_LEVEL > 0
			using _ReturnType = span<element_type, _Count != dynamic_extent ? _Count : (Extent != dynamic_extent ? Extent - _Offset : dynamic_extent)>;
			return _ReturnType{m_data + _Offset, _Count == dynamic_extent ? m_size - _Offset : _Count};
		}

		_NODISCARD constexpr auto subspan(const size_type _Offset, const size_type _Count = dynamic_extent) const noexcept
		/* strengthened */ {
#if _CONTAINER_DEBUG_LEVEL > 0
			_STL_VERIFY(_Offset <= m_size, "Offset out of range in span::subspan(offset, count)");
			_STL_VERIFY(_Count == dynamic_extent || _Count <= m_size - _Offset, "Count out of range in span::subspan(offset, count)");
#endif // _CONTAINER_DEBUG_LEVEL > 0
			using _ReturnType = span<element_type, dynamic_extent>;
			return _ReturnType{m_data + _Offset, _Count == dynamic_extent ? m_size - _Offset : _Count};
		}

		// [span.obs] Observers
		_NODISCARD constexpr size_type size() const noexcept
		{
			return m_size;
		}

#pragma warning(push)
#pragma warning(disable : 4127) // conditional expression is constant
		_NODISCARD constexpr size_type size_bytes() const noexcept
		{
#if _CONTAINER_DEBUG_LEVEL > 0
			_STL_VERIFY(m_size <= dynamic_extent / sizeof(element_type), "size of span in bytes exceeds std::numeric_limits<size_type>::max()");
#endif // _CONTAINER_DEBUG_LEVEL > 0
			return m_size * sizeof(element_type);
		}
#pragma warning(pop)

		_NODISCARD constexpr bool empty() const noexcept
		{
			return m_size == 0;
		}

		// [span.elem] Element access
		_NODISCARD constexpr reference operator[](const size_type _Off) const noexcept /* strengthened */
		{
#if _CONTAINER_DEBUG_LEVEL > 0
			_STL_VERIFY(_Off < m_size, "span index out of range");
#endif // _CONTAINER_DEBUG_LEVEL > 0
			return m_data[_Off];
		}

#pragma warning(push)
#pragma warning(disable : 4127)                               // conditional expression is constant
		_NODISCARD constexpr reference front() const noexcept /* strengthened */
		{
#if _CONTAINER_DEBUG_LEVEL > 0
			_STL_VERIFY(m_size > 0, "front of empty span");
#endif // _CONTAINER_DEBUG_LEVEL > 0
			return m_data[0];
		}

		_NODISCARD constexpr reference back() const noexcept /* strengthened */
		{
#if _CONTAINER_DEBUG_LEVEL > 0
			_STL_VERIFY(m_size > 0, "back of empty span");
#endif // _CONTAINER_DEBUG_LEVEL > 0
			return m_data[m_size - 1];
		}
#pragma warning(pop)

		_NODISCARD constexpr pointer data() const noexcept
		{
			return m_data;
		}

		// [span.iterators] Iterator support
		_NODISCARD constexpr iterator begin() const noexcept
		{
#if _ITERATOR_DEBUG_LEVEL >= 1
			return {m_data, m_data, m_data + m_size};
#else  // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 / _ITERATOR_DEBUG_LEVEL == 0 vvv
			return {m_data};
#endif // ^^^ _ITERATOR_DEBUG_LEVEL == 0 ^^^
		}

		_NODISCARD constexpr iterator end() const noexcept
		{
			const auto _End = m_data + m_size;
#if _ITERATOR_DEBUG_LEVEL >= 1
			return {_End, m_data, _End};
#else  // ^^^ _ITERATOR_DEBUG_LEVEL >= 1 / _ITERATOR_DEBUG_LEVEL == 0 vvv
			return {_End};
#endif // ^^^ _ITERATOR_DEBUG_LEVEL == 0 ^^^
		}

#if _HAS_CXX23 && defined(__cpp_lib_concepts)
		_NODISCARD constexpr const_iterator cbegin() const noexcept
		{
			return begin();
		}

		_NODISCARD constexpr const_iterator cend() const noexcept
		{
			return end();
		}
#endif // _HAS_CXX23 && defined(__cpp_lib_concepts)

		_NODISCARD constexpr reverse_iterator rbegin() const noexcept
		{
			return reverse_iterator{end()};
		}

		_NODISCARD constexpr reverse_iterator rend() const noexcept
		{
			return reverse_iterator{begin()};
		}

#if _HAS_CXX23 && defined(__cpp_lib_concepts)
		_NODISCARD constexpr const_reverse_iterator crbegin() const noexcept
		{
			return rbegin();
		}

		_NODISCARD constexpr const_reverse_iterator crend() const noexcept
		{
			return rend();
		}
#endif // _HAS_CXX23 && defined(__cpp_lib_concepts)

		_NODISCARD constexpr pointer _Unchecked_begin() const noexcept
		{
			return m_data;
		}

		_NODISCARD constexpr pointer _Unchecked_end() const noexcept
		{
			return m_data + m_size;
		}
	};

	template <class T, size_type Extent>
	span(T (&)[Extent]) -> span<T, Extent>;

	template <class T, size_type _Size>
	span(array<T, _Size>&) -> span<T, _Size>;

	template <class T, size_type _Size>
	span(const array<T, _Size>&) -> span<const T, _Size>;

#ifdef __cpp_lib_concepts

	template <contiguous_iterator _It, class _End>
	span(_It, _End) -> span<remove_reference_t<iter_reference_t<_It>>>;

	template <_RANGES contiguous_range _Rng>
	span(_Rng&&) -> span<remove_reference_t<_RANGES range_reference_t<_Rng>>>;

#else  // ^^^ defined(__cpp_lib_concepts) / !defined(__cpp_lib_concepts) vvv

	template <class _Rng>
	span(_Rng&) -> span<typename _Rng::value_type>;

	template <class _Rng>
	span(const _Rng&) -> span<const typename _Rng::value_type>;

#endif // ^^^ !defined(__cpp_lib_concepts) ^^^
} // namespace rsl
