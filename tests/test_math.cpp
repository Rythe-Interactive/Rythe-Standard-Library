#include <catch2/catch_test_macros.hpp>

#define RYTHE_VALIDATE

#include <rsl/math>

TEST_CASE("vectors", "[math]")
{
	using namespace rsl::math;

	SECTION("vector 1")
	{
		float1 vec(123.456f);

		REQUIRE(vec.x == 123.456f);
		REQUIRE(vec.xx == float2(123.456f));
		REQUIRE(vec.xxx == float3(123.456f));
		REQUIRE(vec.xxxx == float4(123.456f));

		REQUIRE(vec.x == vec.u);
		REQUIRE(vec.x == vec.r);
		REQUIRE(vec.x == vec.s);
		REQUIRE(vec.xx == vec.uu);
		REQUIRE(vec.xx == vec.rr);
		REQUIRE(vec.xx == vec.ss);
		REQUIRE(vec.xxx == vec.uuu);
		REQUIRE(vec.xxx == vec.rrr);
		REQUIRE(vec.xxx == vec.sss);
		REQUIRE(vec.xxxx == vec.uuuu);
		REQUIRE(vec.xxxx == vec.rrrr);
		REQUIRE(vec.xxxx == vec.ssss);
	}
}
