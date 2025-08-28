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
			REQUIRE(s.size() == 11);
		}

		{//copy constructor
			rsl::dynamic_string s = dynamic_string::from_array("hello world");
			rsl::dynamic_string s2(s);
			REQUIRE(!s.empty());
			REQUIRE(!s2.empty());
			REQUIRE(s.size() == s2.size());
			REQUIRE(s == s2);
		}

		{//capacity constructor
			rsl::dynamic_string s = dynamic_string::create_reserved(10);
			REQUIRE(s.empty());
			REQUIRE(s.capacity() == 10);
		}

		{//char array constructor
			const char str[5] = {'h','e','l','l','o'};
			rsl::dynamic_string s = dynamic_string::from_array(str);
			REQUIRE(!s.empty());
			REQUIRE(s.capacity() == 5);
		}
	}

	SECTION("search")
	{
		string_view str = "abcdeffedcbaabcdeffedcba"_sv;

	    {
		    size_type offset = rsl::linear_search_sequence(str, "effed"_sv, 0);
		    REQUIRE(offset == 4);
		    REQUIRE(str[offset] == 'e');
	    }
	    {
		    size_type offset = rsl::reverse_linear_search_sequence(str, "effed"_sv, 0);
		    REQUIRE(offset == 16);
		    REQUIRE(str[offset] == 'e');
	    }
	    {
		    size_type offset = rsl::linear_search_sequence(str, "effed"_sv, 10);
		    REQUIRE(offset == 16);
		    REQUIRE(str[offset] == 'e');
	    }
		{
			size_type offset = rsl::linear_search(str, 'd', 0);
			REQUIRE(offset == 3);
			REQUIRE(str[offset] == 'd');
		}
	    {
		    size_type offset = rsl::linear_search(str, 'd', 10);
		    REQUIRE(offset == 15);
		    REQUIRE(str[offset] == 'd');
	    }
		{
			size_type offset = rsl::linear_search(str, 'c', 0);
			REQUIRE(offset == 2);
			REQUIRE(str[offset] == 'c');
		}
		{
			size_type offset = rsl::linear_search(str, 'c', 5);
			REQUIRE(offset == 9);
			REQUIRE(str[offset] == 'c');
		}
		{
			size_type offset = rsl::linear_search_collection(str, "efb"_sv, 0);
			REQUIRE(offset == 1);
			REQUIRE(str[offset] == 'b');
		}

		{
			size_type offset = rsl::linear_search_not_eq(str, 'd', 0);
			REQUIRE(offset == 0);
			REQUIRE(str[offset] == 'a');
		}
		{
			size_type offset = rsl::linear_search_not_eq(str, 'f', 5);
			REQUIRE(offset == 7);
			REQUIRE(str[offset] == 'e');
		}
		{
			size_type offset = rsl::linear_search_not_eq(str, 'a', 0);
			REQUIRE(offset == 1);
			REQUIRE(str[offset] == 'b');
		}
		{
			size_type offset = rsl::linear_search_outside_collection(str, "abcde"_sv, 0);
			REQUIRE(offset == 5);
			REQUIRE(str[offset] == 'f');
		}

		{
			size_type offset = rsl::reverse_linear_search(str, 'd', 0);
			REQUIRE(offset == 20);
			REQUIRE(str[offset] == 'd');
		}
		{
			size_type offset = rsl::reverse_linear_search(str, 'c', 0);
			REQUIRE(offset == 21);
			REQUIRE(str[offset] == 'c');
		}
		{
			size_type offset = rsl::reverse_linear_search_collection(str, "efb"_sv, 0);
			REQUIRE(offset == 22);
			REQUIRE(str[offset] == 'b');
		}

		{
			size_type offset = rsl::reverse_linear_search_not_eq(str, 'd', 0);
			REQUIRE(offset == 23);
			REQUIRE(str[offset] == 'a');
		}
		{
			size_type offset = rsl::reverse_linear_search_not_eq(str, 'a', 0);
			REQUIRE(offset == 22);
			REQUIRE(str[offset] == 'b');
		}
		{
			size_type offset = rsl::reverse_linear_search_outside_collection(str, "abcde"_sv, 0);
			REQUIRE(offset == 18);
			REQUIRE(str[offset] == 'f');
		}
	}

    SECTION("hashed string")
	{
	    {//operator=
	        rsl::hashed_string s;
	        s = "hello world"_hs;
	        REQUIRE(!s.empty());
	        REQUIRE(s.size() == 11);
	    }

	    {//copy constructor
	        rsl::hashed_string s = hashed_string::from_array("hello world");
	        rsl::hashed_string s2(s);
	        REQUIRE(!s.empty());
	        REQUIRE(!s2.empty());
	        REQUIRE(s.size() == s2.size());
	        REQUIRE(s == s2);
	    }

	    {//capacity constructor
	        rsl::hashed_string s = hashed_string::create_reserved(10);
	        REQUIRE(s.empty());
	        REQUIRE(s.capacity() == 10);
	    }

	    {//char array constructor
	        const char str[5] = {'h','e','l','l','o'};
	        rsl::hashed_string s = hashed_string::from_array(str);
	        REQUIRE(!s.empty());
	        REQUIRE(s.capacity() == 5);
	    }
	}
}
