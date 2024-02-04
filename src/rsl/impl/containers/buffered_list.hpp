#pragma once

#include <span>
#include <type_traits>

#include "../util/assert.hpp"
#include "../util/primitives.hpp"

namespace rsl
{
	template <typename ValueType, size_type maxSize>
	struct buffered_list
	{
		using value_type = ValueType;
		using reference = value_type&;
		using const_reference = const value_type&;
		using iterator = value_type*;
		using const_iterator = const value_type*;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using view_type = std::span<value_type>;
		using const_view_type = std::span<const value_type>;

	private:
		union value_container
		{
			value_type value;
			byte dummy;
		};

		value_container m_buffer[maxSize + 1];
		size_type m_size;

		constexpr static size_type validate_size(size_type newSize) noexcept { return newSize < maxSize ? newSize : maxSize; }

		constexpr static bool copy_assign_noexcept = std::is_nothrow_copy_assignable_v<value_type>;
		constexpr static bool copy_construct_noexcept = std::is_nothrow_copy_constructible_v<value_type>;
		constexpr static bool move_assign_noexcept = std::is_nothrow_move_assignable_v<value_type>;
		constexpr static bool move_construct_noexcept = std::is_nothrow_move_constructible_v<value_type>;

		template <typename... Args>
		constexpr static bool construct_noexcept = std::is_nothrow_constructible_v<value_type, Args...>;

		constexpr void copy_assign_from_unsafe_impl(size_type begin, size_type end, const value_type* src) noexcept(copy_assign_noexcept)
		{
			for (auto* to = m_buffer + begin; to != m_buffer + end; to++, src++)
				to->value = *src;
		}

		constexpr void copy_construct_from_unsafe_impl(size_type begin, size_type end, const value_type* src) noexcept(copy_construct_noexcept)
		{
			for (auto* to = m_buffer + begin; to != m_buffer + end; to++, src++)
				new (&to->value) value_type(*src);
		}

		constexpr void move_assign_from_unsafe_impl(size_type begin, size_type end, const value_type* src) noexcept(move_assign_noexcept)
		{
			for (auto* to = m_buffer + begin; to != m_buffer + end; to++, src++)
				to->value = std::move(*src);
		}

		constexpr void move_construct_from_unsafe_impl(size_type begin, size_type end, const value_type* src) noexcept(move_construct_noexcept)
		{
			for (auto* to = m_buffer + begin; to != m_buffer + end; to++, src++)
				new (&to->value) value_type(std::move(*src));
		}

		template <typename... Args>
		constexpr void emplace_unsafe_impl(size_type begin, size_type end, Args&&... args) noexcept(construct_noexcept<Args...>)
		{
			for (auto* to = m_buffer + begin; to != m_buffer + end; to++)
				new (&to->value) value_type(std::forward<Args>(args)...);
		}

		constexpr void reset_unsafe_impl(size_type begin, size_type end) noexcept
		{
			for (size_type i = begin; i < end; i++)
				m_buffer[i].value.~value_type();
		}

		constexpr void copy_assign_impl(const value_type* src, size_type srcSize) noexcept(copy_assign_noexcept && copy_construct_noexcept)
		{
			if (m_size >= srcSize)
			{
				reset_unsafe_impl(srcSize, m_size);
				m_size = srcSize;
				copy_assign_from_unsafe_impl(0, m_size, src);
			}
			else
			{
				size_type effectiveSize = validate_size(srcSize);
				copy_assign_from_unsafe_impl(0, m_size, src);
				copy_construct_from_unsafe_impl(m_size, effectiveSize, src);
				m_size = effectiveSize;
			}
		}

	public:
		constexpr buffered_list() noexcept = default;

		constexpr buffered_list(const buffered_list& src) noexcept(copy_construct_noexcept)
			: m_size(validate_size(src.m_size))
		{
			copy_construct_from_unsafe_impl(0, m_size, src.m_buffer);
		}

		constexpr buffered_list(buffered_list&& src) noexcept(move_construct_noexcept)
			: m_size(validate_size(src.m_size))
		{
			move_construct_from_unsafe_impl(0, m_size, src.m_buffer);
		}

		template <size_type N>
		constexpr buffered_list(const value_type (&arr)[N]) noexcept(copy_construct_noexcept)
			: m_size(validate_size(N))
		{
			copy_construct_from_unsafe_impl(0, m_size, arr);
		}

		template <size_type N>
		constexpr buffered_list(value_type (&&arr)[N]) noexcept(move_construct_noexcept)
			: m_size(validate_size(N))
		{
			move_construct_from_unsafe_impl(0, m_size, arr);
		}

		constexpr buffered_list(view_type src) noexcept(copy_construct_noexcept)
			: m_size(validate_size(src.size()))
		{
			copy_construct_from_unsafe_impl(0, m_size, src.data());
		}

		constexpr buffered_list& operator=(const buffered_list& src) noexcept(copy_assign_noexcept && copy_construct_noexcept)
		{
			copy_assign_impl(src.m_buffer, src.m_size);
			return *this;
		}

		template <size_type N>
		constexpr buffered_list& operator=(const value_type (&arr)[N]) noexcept(copy_assign_noexcept && copy_construct_noexcept)
		{
			copy_assign_impl(arr, N);
			return *this;
		}

		constexpr buffered_list& operator=(view_type src) noexcept(copy_assign_noexcept && copy_construct_noexcept)
		{
			copy_assign_impl(src.data(), src.size());
			return *this;
		}

		template <typename... Args>
		constexpr void resize(size_type newSize, Args&&... args) noexcept
		{
			size_type oldSize = m_size;
			m_size = validate_size(newSize);

			if (m_size >= oldSize)
				emplace_unsafe_impl(oldSize, m_size, std::forward<Args>(args)...);
			else
				reset_unsafe_impl(m_size, oldSize);
		}

		constexpr void push_back(const value_type& value) noexcept(move_construct_noexcept && !RYTHE_VALIDATE_ON)
		{
			size_type oldSize = m_size++;
			rsl_assert_msg(m_size <= maxSize, "out of range");
			copy_construct_from_unsafe_impl(oldSize, m_size, &value);
		}

		constexpr void push_back(value_type&& value) noexcept(move_construct_noexcept && !RYTHE_VALIDATE_ON)
		{
			size_type oldSize = m_size++;
			rsl_assert_msg(m_size <= maxSize, "out of range");
			move_construct_from_unsafe_impl(oldSize, m_size, &value);
		}

		template <typename... Args>
		constexpr value_type& emplace_back(Args&&... args) noexcept(construct_noexcept<Args...> && !RYTHE_VALIDATE_ON)
		{
			size_type oldSize = m_size++;
			rsl_assert_msg(m_size <= maxSize, "out of range");
			emplace_unsafe_impl(oldSize, m_size, std::forward<Args>(args)...);
			return m_buffer[m_size - 1].value;
		}

		constexpr reference at(size_type i) noexcept(!RYTHE_VALIDATE_ON)
		{
			rsl_assert_msg(i < m_size, "out of range");
			return m_buffer[i].value;
		}

		constexpr const_reference at(size_type i) const noexcept(!RYTHE_VALIDATE_ON)
		{
			rsl_assert_msg(i < m_size, "out of range");
			return m_buffer[i].value;
		}

		constexpr reference raw_at(size_type i) noexcept
		{
			return m_buffer[i].value;
		}

		constexpr const_reference raw_at(size_type i) const noexcept
		{
			return m_buffer[i].value;
		}

		constexpr reference operator[](size_type i) { return at(i); }
		constexpr const_reference operator[](size_type i) const { return at(i); }

		constexpr bool empty() const noexcept { return m_size == 0; }
		constexpr size_type size() const noexcept { return m_size; }
		constexpr size_type max_size() const noexcept { return maxSize; }

		constexpr view_type view() noexcept { return view_type(&m_buffer->value, m_size); }
		constexpr const_view_type view() const noexcept { return const_view_type(&m_buffer->value, m_size); }
		constexpr view_type raw_view() noexcept { return view_type(&m_buffer->value, maxSize); }
		constexpr const_view_type raw_view() const noexcept { return const_view_type(&m_buffer->value, maxSize); }

		constexpr operator const value_type*() const noexcept { return &m_buffer->value; }
		constexpr operator view_type() noexcept { return view(); }
		constexpr operator const_view_type() const noexcept { return view(); }

		constexpr iterator begin() noexcept { return &m_buffer->value; }
		constexpr const_iterator begin() const noexcept { return &m_buffer->value; }
		constexpr const_iterator cbegin() const noexcept { return &m_buffer->value; }

		constexpr iterator end() noexcept { return &m_buffer[m_size].value; }
		constexpr const_iterator end() const noexcept { return &m_buffer[m_size].value; }
		constexpr const_iterator cend() const noexcept { return &m_buffer[m_size].value; }

		constexpr reverse_iterator rend() noexcept { return reverse_iterator(&m_buffer[m_size].value); }
		constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(&m_buffer[m_size].value); }
		constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(&m_buffer[m_size].value); }

		constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(&m_buffer->value); }
		constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(&m_buffer->value); }
		constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(&m_buffer->value); }
	};

	template <typename ValueType, size_type maxSize>
	buffered_list(const ValueType (&)[maxSize]) -> buffered_list<ValueType, maxSize>;

	template <typename ValueType>
	using b_list64 = buffered_list<ValueType, 64>;
	template <typename ValueType>
	using b_list128 = buffered_list<ValueType, 128>;
	template <typename ValueType>
	using b_list256 = buffered_list<ValueType, 256>;
	template <typename ValueType>
	using b_list512 = buffered_list<ValueType, 512>;
} // namespace rsl
