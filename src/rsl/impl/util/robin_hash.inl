#pragma once

#include "../defines.hpp"
#include "primitives.hpp"

// Based on:
// Fast & memory efficient hashtable based on robin hood hashing for C++11/14/17/20
// https://github.com/martinus/robin-hood-hashing

namespace rsl::internal
{
	namespace robin_hash
	{
		constexpr u64 offset_basis = 0xe17a1465ull;
		constexpr u64 multiplier = 0xc6a4a7935bd1e995ull;
		constexpr u64 shift = 47ull;

		constexpr id_type hash_bytes(span<const byte> bytes) noexcept
		{
			constexpr size_type blockSize = 8;
			const size_type blockCount = bytes.size() / blockSize;
			const size_type remainder = bytes.size() - blockCount;
			const byte* const data = bytes.data();
			u64 hash = offset_basis ^ (static_cast<u64>(bytes.size()) * multiplier);

			for (size_type i = 0; i < blockCount; i++)
			{
				u64 k = unaligned_load<u64>(data + i * blockSize);

				k *= multiplier;
				k ^= k >> shift;
				k *= multiplier;

				hash ^= k;
				hash *= multiplier;
			}

			const byte* const remainingData = data + blockCount * blockSize;
			switch (remainder)
			{
				case 7: hash ^= static_cast<u64>(remainingData[6]) << 48u; [[fallthrough]];
				case 6: hash ^= static_cast<u64>(remainingData[5]) << 40u; [[fallthrough]];
				case 5: hash ^= static_cast<u64>(remainingData[4]) << 32u; [[fallthrough]];
				case 4: hash ^= static_cast<u64>(remainingData[3]) << 24u; [[fallthrough]];
				case 3: hash ^= static_cast<u64>(remainingData[2]) << 16u; [[fallthrough]];
				case 2: hash ^= static_cast<u64>(remainingData[1]) << 8u; [[fallthrough]];
				case 1:
					hash ^= static_cast<u64>(remainingData[0]);
					hash *= multiplier;
					[[fallthrough]];
				default: break;
			}

			hash ^= hash >> shift;

			hash *= multiplier;
			hash ^= hash >> shift;
			return static_cast<id_type>(hash);
		}

		[[rythe_always_inline]] constexpr id_type hash_int(uint64 i) noexcept
		{
			i ^= i >> 33u;
			i *= 0xff51afd7ed558ccdull;
			i ^= i >> 33u;

			i *= 0xc4ceb9fe1a85ec53ull;
			i ^= i >> 33u;
			return static_cast<id_type>(i);
		}
	} // namespace robin_hash

	template <typename T, hash_algorithm Algorithm>
	struct hash_strategy;

	template <typename T>
	struct hash_strategy<T*, hash_algorithm::robin>
	{
		[[rythe_always_inline]] constexpr static id_type hash(const T*& val) noexcept
		{
			return robin_hash::hash_int(static_cast<u64>(val));
		}
	};

#define RSL_HASH_INT(T)                                                                                                    \
	template <>                                                                                                        \
	struct hash_strategy<T, hash_algorithm::robin>                                                                     \
	{                                                                                                                  \
		[[rythe_always_inline]] constexpr static id_type hash(const T& val) noexcept                                   \
		{                                                                                                              \
			return robin_hash::hash_int(static_cast<u64>(val));                                                        \
		}                                                                                                              \
	};

	RSL_HASH_INT(uint8);
	RSL_HASH_INT(uint16);
	RSL_HASH_INT(uint32);
	RSL_HASH_INT(uint64);
	RSL_HASH_INT(int8);
	RSL_HASH_INT(int16);
	RSL_HASH_INT(int32);
	RSL_HASH_INT(int64);
	RSL_HASH_INT(bool);
	RSL_HASH_INT(char);
	RSL_HASH_INT(long);
	RSL_HASH_INT(unsigned long);
	RSL_HASH_INT(char16_t);
	RSL_HASH_INT(char32_t);
	RSL_HASH_INT(wchar_t);

#undef HASH_INT
} // namespace rsl::internal
