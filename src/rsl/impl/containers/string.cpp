#include "string.hpp"

namespace rsl
{
	namespace
	{
		size_type read_utf8(const utf8* readPos, uint32& output)
		{
			size_type width = 1u;
			uint32 character = static_cast<uint32>(*readPos);
			readPos++;

			if ((character & 0xe0u) == 0xc0u)
			{
				width = 2u;
				character &= 0x1fu;
			}
			else if ((character & 0xf0u) == 0xe0u)
			{
				width = 3u;
				character &= 0x0fu;
			}
			else if ((character & 0xf8u) == 0xf0u)
			{
				width = 4u;
				character &= 0x07u;
			}
			else if (character >= 128u)
			{
				return 1u;
			}

			for (size_t count = width - 1u; count > 0u; count--)
			{
				const utf8 currentChar = *readPos;
				if ((currentChar & 0xc0u) != 0x80u)
				{
					return width - count;
				}
				character = character << 6u | currentChar & 0x3fu;
				readPos++;
			}

			output = character;
			return width;
		}
	}

	wstring to_utf16(string::view_type str)
	{
		wstring result;
		result.reserve(str.size());

		utf8* data = str.data();
		utf8* end = data + str.size();

		while (data != end)
		{
			uint32 character = 0u;
			size_type bytesRead = read_utf8(data, character);
			data += bytesRead;

			if (character >= 0x10000u)
			{
				character -= 0x10000u;
				result.push_back(static_cast<utf16>(character >> 10u & 0x3ffu | 0xd800u));
				result.push_back(static_cast<utf16>(character & 0x3ffu | 0xdc00u));
			}
			else
			{
				result.push_back(static_cast<utf16>(character));
			}
		}

		return result;
	}
}
