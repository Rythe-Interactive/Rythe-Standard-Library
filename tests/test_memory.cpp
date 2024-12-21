#define RYTHE_VALIDATE

#include<rsl/memory>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("memory resource", "[memory]") {

}

TEST_CASE("reference counter", "[memory]")
{
    SECTION("manual reference counter")
    {
		rsl::manual_reference_counter refCounter;
	}

	SECTION("basic reference counter")
	{
		rsl::reference_counter refCounter;
	}
}

TEST_CASE("managed resource", "[memory]")
{
    SECTION("construction")
    {

    }

    SECTION("reference counting")
    {

    }
}

