#pragma once
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "../util/common.hpp"
#include "../util/primitives.hpp"
#include "iterators.hpp"

/**
 * @file hashed_sparse_set.hpp
 */

namespace rsl
{
	/**@class hashed_sparse_set
	 * @brief Quick lookup contiguous map. The map is based on the concept of a sparse set and thus inherits it's lookup
	 * complexity and contiguous nature.
	 * @tparam value_type The type to be used as the value.
	 * @tparam dense_type Container to be used to store the values.
	 * @tparam sparse_type Container to be used to store the keys.
	 * @note With default container parameters iterators may be invalidated upon resize. See reference of std::vector.
	 * @note Removing item might invalidate the iterator of the last item in the dense container.
	 */
	template <
		typename value_type, typename hash_type = std::hash<value_type>,
		template <typename...> typename dense_type = std::vector,
		template <typename...> typename sparse_type = std::unordered_map>
	class hashed_sparse_set
	{
	public:
		using self_type = hashed_sparse_set<value_type, hash_type, dense_type, sparse_type>;
		using self_reference = self_type&;
		using self_const_reference = const self_type&;

		using value_reference = value_type&;
		using value_const_reference = const value_type&;
		using value_pointer = value_type*;

		using sparse_container = sparse_type<value_type, size_type, hash_type>;
		using dense_container = dense_type<value_type>;

		using iterator = typename dense_container::iterator;
		using const_iterator = typename dense_container::const_iterator;

		using reverse_iterator = typename dense_container::reverse_iterator;
		using const_reverse_iterator = typename dense_container::const_reverse_iterator;
		using reverse_itr_range = pair_range<reverse_iterator>;
		using const_reverse_itr_range = pair_range<const_reverse_iterator>;

	private:
		dense_container m_dense;
		sparse_container m_sparse;

		size_type m_size = 0;
		size_type m_capacity = 0;

	public:
		[[nodiscard]] dense_container& dense() noexcept { return m_dense; }
		[[nodiscard]] const dense_container& dense() const noexcept { return m_dense; }

		[[nodiscard]] iterator begin() noexcept { return m_dense.begin(); }
		[[nodiscard]] const_iterator begin() const noexcept { return m_dense.cbegin(); }
		[[nodiscard]] const_iterator cbegin() const noexcept { return m_dense.cbegin(); }

		[[nodiscard]] iterator end() noexcept { return m_dense.begin() + m_size; }
		[[nodiscard]] const_iterator end() const noexcept { return m_dense.cbegin() + m_size; }
		[[nodiscard]] const_iterator cend() const noexcept { return m_dense.cbegin() + m_size; }

		[[nodiscard]] reverse_iterator rbegin() noexcept { return m_dense.rbegin() + (m_dense.size() - m_size); }
		[[nodiscard]] const_reverse_iterator rbegin() const noexcept
		{
			return m_dense.crbegin() + (m_dense.size() - m_size);
		}
		[[nodiscard]] const_reverse_iterator crbegin() const noexcept
		{
			return m_dense.crbegin() + (m_dense.size() - m_size);
		}

		[[nodiscard]] reverse_iterator rend() noexcept { return m_dense.rend(); }
		[[nodiscard]] const_reverse_iterator rend() const noexcept { return m_dense.crend(); }
		[[nodiscard]] const_reverse_iterator crend() const noexcept { return m_dense.crend(); }

		[[nodiscard]] reverse_itr_range reverse_range() noexcept { return pair_range{rbegin(), rend()}; }
		[[nodiscard]] const_reverse_itr_range reverse_range() const noexcept { return pair_range{crbegin(), crend()}; }

		/**@brief Returns the amount of items in the sparse_map.
		 * @returns size_type Current amount of items contained in sparse_map.
		 */
		[[nodiscard]] size_type size() const noexcept { return m_size; }

		/**@brief Returns the capacity of items the sparse_map could at least store without invalidating the iterators.
		 * @returns size_type Current capacity of the dense container.
		 */
		[[nodiscard]] size_type capacity() const noexcept { return m_capacity; }

		/**@brief Returns the maximum number of items the hashed_sparse_set could at most store without crashing.
		 * @note This value typically reflects the theoretical limit on the size of the container, at most
		 * std::numeric_limits<difference_type>::max(). At runtime, the size of the container may be limited to a value
		 * smaller than max_size() by the amount of RAM available.
		 * @returns size_type
		 */
		[[nodiscard]] size_type max_size() const noexcept { return m_dense.max_size(); }

		/**@brief Returns whether the sparse_map is empty.
		 * @returns bool True if the sparse_map is empty, otherwise false.
		 */
		[[nodiscard]] bool empty() const noexcept { return m_size == 0; }

		/**@brief Clears sparse_map.
		 * @note Will not update capacity.
		 */
		void clear() noexcept
		{
			m_size = 0;
			m_dense.clear();
			m_sparse.clear();
			m_capacity = 0;
		}

		/**@brief Reserves space in dense container for more items.
		 * @param size Amount of items to reserve space for (would be the new capacity).
		 * @note Will update capacity if resize happened.
		 */
		void reserve(size_type size)
		{
			if (size > m_capacity)
			{
				m_dense.resize(size);
				m_capacity = size;
			}
		}

#pragma region count
		/**@brief Returns the amount of items of a certain value.
		 * @param val Value to look for.
		 * @returns size_type Amount of items of the same value (either 0 or 1).
		 * @note Function is only available for compatibility reasons, it is advised to use contains instead.
		 * @ref legion::core::sparse_map::contains
		 */
		[[nodiscard]] size_type count(value_const_reference val) const { return contains(val); }

		/**@brief Returns the amount of items of a certain value.
		 * @param val Value to look for.
		 * @returns size_type Amount of items of the same value (either 0 or 1).
		 * @note Function is only available for compatibility reasons, it is advised to use contains instead.
		 * @ref legion::core::sparse_map::contains
		 */
		[[nodiscard]] size_type count(value_type&& val) const { return contains(val); }
#pragma endregion

#pragma region contains
		/**@brief Checks whether a certain value is contained in the sparse_map.
		 * @param val Value to check for.
		 * @returns bool True if the value was found, otherwise false.
		 */
		[[nodiscard]] bool contains(value_const_reference val) const
		{
			if (!m_sparse.count(val))
			{
				return false;
			}

			const size_type& sparseVal = m_sparse.at(val);
			return sparseVal >= 0 && sparseVal < m_size && sparseVal < m_dense.size() && m_dense.at(sparseVal) == val;
		}

		/**@brief Checks whether a certain value is contained in the sparse_map.
		 * @param val Value to check for.
		 * @returns bool True if the value was found, otherwise false.
		 */
		[[nodiscard]] bool contains(value_type&& val) const
		{
			if (!m_sparse.count(val))
			{
				return false;
			}

			const size_type& sparseVal = m_sparse.at(val);
			return sparseVal >= 0 && sparseVal < m_size && sparseVal < m_dense.size() && m_dense.at(sparseVal) == val;
		}

		/**@brief Checks if all items in hashed_sparse_set are inside this set as well.
		 * @param other Other hashed_sparse_set to check against.
		 * @returns bool True if all items in other are also in this hashed_sparse_set, otherwise false.
		 */
		[[nodiscard]] bool contains(const hashed_sparse_set<value_type>& other) const
		{
			if (other.m_size == 0)
			{
				return true;
			}

			if (m_size == 0 || m_size < other.m_size)
			{
				return false;
			}

			for (int i = 0; i < other.m_size; i++)
			{
				if (!contains(other.m_dense[i]))
				{
					return false;
				}
			}

			return true;
		}
#pragma endregion

		/**@brief Checks if all items are the same for both hashed_sparse_sets.
		 * @param other Other hashed_sparse_set to check against.
		 * @returns bool True if both sets are the same size and contain the same items, otherwise false.
		 */
		[[nodiscard]] bool equals(self_const_reference other) const
		{
			if (m_size == other.m_size)
			{
				for (int i = 0; i < m_size; i++)
				{
					if (!other.contains(m_dense[i]))
					{
						return false;
					}
				}

				return true;
			}

			return false;
		}

		/**@brief Checks if all items are the same for both hashed_sparse_sets.
		 * @param other Other hashed_sparse_set to check against.
		 * @returns bool True if both sets are the same size and contain the same items, otherwise false.
		 */
		[[nodiscard]] bool operator==(self_const_reference other) const
		{
			if (m_size == other.m_size)
			{
				for (int i = 0; i < m_size; i++)
				{
					if (!other.contains(m_dense[i]))
					{
						return false;
					}
				}

				return true;
			}

			return false;
		}

#pragma region find
		/**@brief Finds the iterator of a value using std::find.
		 * @param val Value to find.
		 * @returns Iterator to the value if found, otherwise end.
		 */
		[[nodiscard]] iterator find(value_const_reference val)
		{
			if (contains(val))
			{
				return begin() + m_sparse.at(val);
			}
			return end();
		}

		/**@brief Finds the iterator of a value using std::find.
		 * @param val Value to find.
		 * @returns Iterator to the value if found, otherwise end.
		 */
		[[nodiscard]] const_iterator find(value_const_reference val) const
		{
			if (contains(val))
			{
				return begin() + m_sparse.at(val);
			}
			return end();
		}
#pragma endregion

#pragma region insert
		/**@brief Inserts new item into hashed_sparse_set.
		 * @param val Value to insert.
		 * @returns std::pair<iterator, bool> Iterator at the location of the val and true if succeeded, end and false
		 * if it didn't succeed.
		 */
		std::pair<iterator, bool> insert(value_const_reference val)
		{
			if (!contains(val))
			{
				if (m_size >= m_capacity)
				{
					reserve(m_size + 1);
				}

				auto itr = m_dense.begin() + m_size;
				*itr = val;

				m_sparse.insert_or_assign(val, m_size);
				++m_size;
				return std::make_pair(itr, true);
			}
			return std::make_pair(end(), false);
		}

		/**@brief Inserts new item into hashed_sparse_set.
		 * @param val Value to insert.
		 * @returns std::pair<iterator, bool> Iterator at the location of the val and true if succeeded, end and false
		 * if it didn't succeed.
		 */
		std::pair<iterator, bool> insert(value_type&& val)
		{
			if (!contains(val))
			{
				if (m_size >= m_capacity)
				{
					reserve(m_size + 1);
				}

				auto itr = m_dense.begin() + m_size;
				*itr = std::move(val);

				m_sparse.insert_or_assign(*itr, m_size);
				++m_size;
				return std::make_pair(itr, true);
			}
			return std::make_pair(end(), false);
		}
#pragma endregion

#pragma region emplace

		/**@brief Construct item in place.
		 * @param arguments Arguments to pass to the item constructor.
		 */
		template <typename... Arguments>
		std::pair<iterator, bool> emplace(Arguments&&... arguments)
		{
			return insert(std::forward<value_type>(value_type(arguments...)));
		}
#pragma endregion

#pragma region operator[]
		/**@brief Returns item from dense container.
		 * @param index Index of item in dense container.
		 */
		[[nodiscard]] value_reference operator[](size_type&& index)
		{
			if (index < 0 || index > m_size)
			{
				throw std::out_of_range("hashed_sparse_set subscript out of range");
			}
			return m_dense[index];
		}

		/**@brief Returns item from dense container.
		 * @param index Index of item in dense container.
		 */
		[[nodiscard]] value_reference operator[](const size_type& index)
		{
			if (index < 0 || index > m_size)
			{
				throw std::out_of_range("hashed_sparse_set subscript out of range");
			}
			return m_dense[index];
		}

		/**@brief Returns item from dense container.
		 * @param index Index of item in dense container.
		 */
		[[nodiscard]] value_const_reference operator[](size_type&& index) const
		{
			if (index < 0 || index > m_size)
			{
				throw std::out_of_range("hashed_sparse_set subscript out of range");
			}
			return m_dense[index];
		}

		/**@brief Returns item from dense container.
		 * @param index Index of item in dense container.
		 */
		[[nodiscard]] value_const_reference operator[](const size_type& index) const
		{
			if (index < 0 || index > m_size)
			{
				throw std::out_of_range("hashed_sparse_set subscript out of range");
			}
			return m_dense[index];
		}
#pragma endregion

#pragma region at
		/**@brief Returns item from dense container.
		 * @param index Index of item in dense container.
		 */
		[[nodiscard]] value_reference at(size_type&& index)
		{
			if (index < 0 || index > m_size)
			{
				throw std::out_of_range("hashed_sparse_set subscript out of range");
			}
			return m_dense[index];
		}

		/**@brief Returns item from dense container.
		 * @param index Index of item in dense container.
		 */
		[[nodiscard]] value_reference at(const size_type& index)
		{
			if (index < 0 || index > m_size)
			{
				throw std::out_of_range("hashed_sparse_set subscript out of range");
			}
			return m_dense[index];
		}

		/**@brief Returns item from dense container.
		 * @param index Index of item in dense container.
		 */
		[[nodiscard]] value_const_reference at(size_type&& index) const
		{
			if (index < 0 || index > m_size)
			{
				throw std::out_of_range("hashed_sparse_set subscript out of range");
			}
			return m_dense[index];
		}

		/**@brief Returns item from dense container.
		 * @param index Index of item in dense container.
		 */
		[[nodiscard]] value_const_reference at(const size_type& index) const
		{
			if (index < 0 || index > m_size)
			{
				throw std::out_of_range("hashed_sparse_set subscript out of range");
			}
			return m_dense[index];
		}
#pragma endregion

		/**@brief Erases item from sparse_map.
		 * @param val Value that needs to be erased.
		 */
		size_type erase(value_const_reference val)
		{
			if (contains(val))
			{
				if (m_size - 1 != m_sparse[val])
				{
					m_dense[m_sparse[val]] = std::move(m_dense[m_size - 1]);
					m_sparse[m_dense[m_size - 1]] = std::move(m_sparse[val]);
				}

				--m_size;
				return true;
			}
			return false;
		}
	};
} // namespace rsl
