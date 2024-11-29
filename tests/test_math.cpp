#include <catch2/catch_test_macros.hpp>

#define RYTHE_VALIDATE

#include <rsl/math>

TEST_CASE("vectors", "[math]")
{
	using namespace rsl;
	using namespace rsl::math;

	constexpr float32 CONST0 = 123.456f;
	constexpr float32 CONST1 = 234.567f;
	constexpr float32 CONST2 = 345.678f;

	SECTION("vector 1")
	{
		constexpr float1 vec0(CONST0);

		REQUIRE(vec0.x == CONST0);
		REQUIRE(vec0.xx == float2(CONST0, CONST0));
		REQUIRE(vec0.xxx == float3(CONST0, CONST0, CONST0));
		REQUIRE(vec0.xxxx == float4(CONST0, CONST0, CONST0, CONST0));

		REQUIRE(vec0.x == vec0.u);
		REQUIRE(vec0.x == vec0.r);
		REQUIRE(vec0.x == vec0.s);
		REQUIRE(vec0.xx == vec0.uu);
		REQUIRE(vec0.xx == vec0.rr);
		REQUIRE(vec0.xx == vec0.ss);
		REQUIRE(vec0.xxx == vec0.uuu);
		REQUIRE(vec0.xxx == vec0.rrr);
		REQUIRE(vec0.xxx == vec0.sss);
		REQUIRE(vec0.xxxx == vec0.uuuu);
		REQUIRE(vec0.xxxx == vec0.rrrr);
		REQUIRE(vec0.xxxx == vec0.ssss);

		constexpr float1 vec1(CONST1);

		REQUIRE(vec1 != CONST0);
		REQUIRE(vec0.x != vec1.x);
		REQUIRE(vec0 != vec1);

		{
			constexpr float1 result = vec0 + vec1;
			REQUIRE(result.x == (CONST0 + CONST1));
		}
		{
			constexpr float1 result = vec0 - vec1;
			REQUIRE(result.x == (CONST0 - CONST1));
		}
		{
			constexpr float1 result = vec0 * vec1;
			REQUIRE(result.x == (CONST0 * CONST1));
		}
		{
			constexpr float1 result = vec0 / vec1;
			REQUIRE(result.x == (CONST0 / CONST1));
		}
		{
			constexpr float1 result = -vec0;
			REQUIRE(result.x == (-CONST0));
		}
		{
			constexpr bool1 result = vec0 > vec1;
			REQUIRE(!result);
		}
		{
			constexpr bool1 result = vec0 < vec1;
			REQUIRE(result);
		}
		{
			constexpr bool1 result = !bool1(false);
			REQUIRE(result);
		}
	}

	SECTION("vector 2")
	{
		REQUIRE(float2(CONST0, CONST0) == float2(CONST0));

		constexpr float2 vec0(CONST0, CONST1);

		REQUIRE(vec0.x == CONST0);
		REQUIRE(vec0.xx == float2(CONST0, CONST0));
		REQUIRE(vec0.xxx == float3(CONST0, CONST0, CONST0));
		REQUIRE(vec0.xxxx == float4(CONST0, CONST0, CONST0, CONST0));

		REQUIRE(vec0.y == CONST1);
		REQUIRE(vec0.yy == float2(CONST1, CONST1));
		REQUIRE(vec0.yyy == float3(CONST1, CONST1, CONST1));
		REQUIRE(vec0.yyyy == float4(CONST1, CONST1, CONST1, CONST1));

		REQUIRE(vec0.x == vec0.u);
		REQUIRE(vec0.x == vec0.r);
		REQUIRE(vec0.x == vec0.s);
		REQUIRE(vec0.xx == vec0.uu);
		REQUIRE(vec0.xx == vec0.rr);
		REQUIRE(vec0.xx == vec0.ss);
		REQUIRE(vec0.xxx == vec0.uuu);
		REQUIRE(vec0.xxx == vec0.rrr);
		REQUIRE(vec0.xxx == vec0.sss);
		REQUIRE(vec0.xxxx == vec0.uuuu);
		REQUIRE(vec0.xxxx == vec0.rrrr);
		REQUIRE(vec0.xxxx == vec0.ssss);

		REQUIRE(vec0.y == vec0.v);
		REQUIRE(vec0.y == vec0.g);
		REQUIRE(vec0.y == vec0.t);
		REQUIRE(vec0.yy == vec0.vv);
		REQUIRE(vec0.yy == vec0.gg);
		REQUIRE(vec0.yy == vec0.tt);
		REQUIRE(vec0.yyy == vec0.vvv);
		REQUIRE(vec0.yyy == vec0.ggg);
		REQUIRE(vec0.yyy == vec0.ttt);
		REQUIRE(vec0.yyyy == vec0.vvvv);
		REQUIRE(vec0.yyyy == vec0.gggg);
		REQUIRE(vec0.yyyy == vec0.tttt);

		REQUIRE(any(vec0 != CONST0));

		constexpr float2 vec1(CONST2);

		REQUIRE(vec1 != CONST0);
		REQUIRE(vec0.x != vec1.x);
		REQUIRE(vec0.y != vec1.y);
		REQUIRE(vec0 != vec1);

		constexpr float2 vec2 = vec0 + vec1;

		REQUIRE(vec2.x == (CONST0 + CONST2));
		REQUIRE(vec2.y == (CONST1 + CONST2));

		//{
		//	constexpr float1 result = vec0 + vec1;
		//	REQUIRE(result.x == (CONST0 + CONST1));
		//}
		//{
		//	constexpr float1 result = vec0 - vec1;
		//	REQUIRE(result.x == (CONST0 - CONST1));
		//}
		//{
		//	constexpr float1 result = vec0 * vec1;
		//	REQUIRE(result.x == (CONST0 * CONST1));
		//}
		//{
		//	constexpr float1 result = vec0 / vec1;
		//	REQUIRE(result.x == (CONST0 / CONST1));
		//}
		//{
		//	constexpr float1 result = -vec0;
		//	REQUIRE(result.x == (-CONST0));
		//}
		//{
		//	constexpr bool1 result = vec0 > vec1;
		//	REQUIRE(!result);
		//}
		//{
		//	constexpr bool1 result = vec0 < vec1;
		//	REQUIRE(result);
		//}
		//{
		//	constexpr bool1 result = !bool1(false);
		//	REQUIRE(result);
		//}
	}
}
