#include <catch2/catch_test_macros.hpp>

#define RYTHE_VALIDATE

#include <rsl/containers>

TEST_CASE("string","[containers]")
{
	using namespace rsl;
	SECTION("construction")
	{
		{//operator=
			rsl::dynamic_string s;
			s = "hello world";
			REQUIRE(!s.empty());
			REQUIRE(s.size() == 12);
		}

		{//copy constructor
			rsl::dynamic_string s = "hello world";
			rsl::dynamic_string s2(s);
			REQUIRE(!s.empty());
			REQUIRE(!s2.empty());
			REQUIRE(s.size() == s2.size());
			REQUIRE(s == s2);
		}

		{//capacity constructor
			rsl::dynamic_string s(10);
			REQUIRE(s.empty());
			REQUIRE(s.capacity() == 10);
		}

		{//char array constructor
			const char str[5] = {'h','e','l','l','o'};
			rsl::dynamic_string s(str);
			REQUIRE(!s.empty());
			REQUIRE(s.capacity() == 5);
		}
	}

	SECTION("search")
	{
		view str = "abcdeffedcba"_sv;
		{
			size_type offset = rsl::find_first_of(str, 'd', 0);
			REQUIRE(offset == 3);
			REQUIRE(str[offset] == 'd');
		}
		{
			size_type offset = rsl::find_first_of(str, 'c', 0);
			REQUIRE(offset == 2);
			REQUIRE(str[offset] == 'c');
		}
		{
			size_type offset = rsl::find_first_of(str, "efb"_sv, 0);
			REQUIRE(offset == 1);
			REQUIRE(str[offset] == 'b');
		}

		{
			size_type offset = rsl::find_first_not_of(str, 'd', 0);
			REQUIRE(offset == 0);
			REQUIRE(str[offset] == 'a');
		}
		{
			size_type offset = rsl::find_first_not_of(str, 'a', 0);
			REQUIRE(offset == 1);
			REQUIRE(str[offset] == 'b');
		}
		{
			size_type offset = rsl::find_first_not_of(str, "abcde"_sv, 0);
			REQUIRE(offset == 5);
			REQUIRE(str[offset] == 'f');
		}

		{
			size_type offset = rsl::find_last_of(str, 'd', 0);
			REQUIRE(offset == 8);
			REQUIRE(str[offset] == 'd');
		}
		{
			size_type offset = rsl::find_last_of(str, 'c', 0);
			REQUIRE(offset == 9);
			REQUIRE(str[offset] == 'c');
		}
		{
			size_type offset = rsl::find_last_of(str, "efb"_sv, 0);
			REQUIRE(offset == 10);
			REQUIRE(str[offset] == 'b');
		}

		{
			size_type offset = rsl::find_last_not_of(str, 'd', 0);
			REQUIRE(offset == 11);
			REQUIRE(str[offset] == 'a');
		}
		{
			size_type offset = rsl::find_last_not_of(str, 'a', 0);
			REQUIRE(offset == 10);
			REQUIRE(str[offset] == 'b');
		}
		{
			size_type offset = rsl::find_last_not_of(str, "abcde"_sv, 0);
			REQUIRE(offset == 6);
			REQUIRE(str[offset] == 'f');
		}
	}
}
