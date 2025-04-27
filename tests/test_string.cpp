#include <catch2/catch_test_macros.hpp>

#define RYTHE_VALIDATE

#include <rsl/containers>

TEST_CASE("string","[containers]")
{
	SECTION("construction")
	{
		{
			rsl::string s = "hello world";
			REQUIRE(!s.empty());
			REQUIRE(s.size() == 12);
		}
		{
			rsl::string s = "hello world";
			rsl::string s2(s);
			REQUIRE(!s.empty());
			REQUIRE(!s2.empty());
			REQUIRE(s.size() == s2.size());
			REQUIRE(s == s2);
		}
		{
			rsl::string s(10);
			REQUIRE(s.empty());
			REQUIRE(s.capacity() == 10);
		}

		{
			const char* str = new const char[5]{'h','e','l','l','o'};
			rsl::string s(*str);
			REQUIRE(!s.empty());
			REQUIRE(s.capacity() == 10);
		}
	}
}
