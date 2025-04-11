#include <catch2/catch_test_macros.hpp>

#define RYTHE_VALIDATE

#include <rsl/math>

TEST_CASE("vectors", "[math]")
{
	using namespace rsl;
	using namespace rsl::math;

	constexpr float32 const0 = 123.456f;
	constexpr float32 const1 = 234.567f;
	constexpr float32 const2 = 345.678f;
	constexpr float32 const3 = 456.789f;
	constexpr float32 const4 = 567.891f;
	constexpr float32 const5 = 678.912f;

	SECTION("vector 1")
	{
		constexpr float1 vec0(const0);

		REQUIRE(vec0.x == const0);
		REQUIRE(vec0.xx == float2(const0, const0));
		REQUIRE(vec0.xxx == float3(const0, const0, const0));
		REQUIRE(vec0.xxxx == float4(const0, const0, const0, const0));

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

		constexpr float1 vec1(const1);

		REQUIRE(vec1 != const0);
		REQUIRE(vec0.x != vec1.x);
		REQUIRE(vec0 != vec1);

		{
			constexpr float1 result = vec0 + vec1;
			REQUIRE(result.x == (const0 + const1));
		}
		{
			constexpr float1 result = vec0 - vec1;
			REQUIRE(result.x == (const0 - const1));
		}
		{
			constexpr float1 result = vec0 * vec1;
			REQUIRE(result.x == (const0 * const1));
		}
		{
			constexpr float1 result = vec0 / vec1;
			REQUIRE(result.x == (const0 / const1));
		}
		{
			constexpr float1 result = -vec0;
			REQUIRE(result.x == (-const0));
		}
		{
			constexpr bool1 result = vec0 > vec1;
			REQUIRE(!bool(result));
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
		REQUIRE(float2(const0, const0) == float2(const0));

		constexpr float2 vec0(const0, const3);

		REQUIRE(vec0.x == const0);
		REQUIRE(vec0.xx == float2(const0, const0));
		REQUIRE(vec0.xxx == float3(const0, const0, const0));
		REQUIRE(vec0.xxxx == float4(const0, const0, const0, const0));

		REQUIRE(vec0.y == const3);
		REQUIRE(vec0.yy == float2(const3, const3));
		REQUIRE(vec0.yyy == float3(const3, const3, const3));
		REQUIRE(vec0.yyyy == float4(const3, const3, const3, const3));

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

		REQUIRE(vec0.xyyx == float4(const0, const3, const3, const0));
		REQUIRE(vec0.xyxy == float4(const0, const3, const0, const3));
		REQUIRE(vec0.yxxy == float4(const3, const0, const0, const3));
		REQUIRE(vec0.yxyx == float4(const3, const0, const3, const0));

		REQUIRE(any(vec0 != const0));
		REQUIRE(!all(vec0 != const0));
		REQUIRE(all(vec0 != const2));

		constexpr float2 vec1(const2);

		REQUIRE(vec1 != const0);
		REQUIRE(vec0.x != vec1.x);
		REQUIRE(vec0.y != vec1.y);
		REQUIRE(vec0 != vec1);

		constexpr float2 vec2(const0, const4);

		{

			constexpr float2 result = vec0 + vec1;

			REQUIRE(result.x == (const0 + const2));
			REQUIRE(result.y == (const3 + const2));
		}
		{

			constexpr float2 result = vec0 + const1;

			REQUIRE(result.x == (const0 + const1));
			REQUIRE(result.y == (const3 + const1));
		}
		{
			constexpr float2 result = vec0 - vec1;
			REQUIRE(result.x == (const0 - const2));
			REQUIRE(result.y == (const3 - const2));
		}
		{
			constexpr float2 result = vec0 - const1;
			REQUIRE(result.x == (const0 - const1));
			REQUIRE(result.y == (const3 - const1));
		}
		{
			constexpr float2 result = vec0 * vec1;
			REQUIRE(result.x == (const0 * const2));
			REQUIRE(result.y == (const3 * const2));
		}
		{
			constexpr float2 result = vec0 * const1;
			REQUIRE(result.x == (const0 * const1));
			REQUIRE(result.y == (const3 * const1));
		}
		{
			constexpr float2 result = vec0 / vec1;
			REQUIRE(result.x == (const0 / const2));
			REQUIRE(result.y == (const3 / const2));
		}
		{
			constexpr float2 result = vec0 / const1;
			REQUIRE(result.x == (const0 / const1));
			REQUIRE(result.y == (const3 / const1));
		}
		{
			constexpr float2 result = -vec0;
			REQUIRE(result.x == (-const0));
			REQUIRE(result.y == (-const3));
		}
		{
			constexpr bool2 result = vec0 > vec1;
			REQUIRE(!bool(result));
			REQUIRE(!result.x);
			REQUIRE(result.y);
		}
		{
			constexpr bool2 result = vec0 > const1;
			REQUIRE(!bool(result));
			REQUIRE(!result.x);
			REQUIRE(result.y);
		}
		{
			constexpr bool2 result = vec0 < vec1;
			REQUIRE(!bool(result));
			REQUIRE(result.x);
			REQUIRE(!result.y);
		}
		{
			constexpr bool2 result = vec0 < const1;
			REQUIRE(!bool(result));
			REQUIRE(result.x);
			REQUIRE(!result.y);
		}
		{
			constexpr bool2 result = vec0 >= vec2;
			REQUIRE(!bool(result));
			REQUIRE(result.x);
			REQUIRE(!result.y);
		}
		{
			constexpr bool2 result = vec0 >= const3;
			REQUIRE(!bool(result));
			REQUIRE(!result.x);
			REQUIRE(result.y);
		}
		{
			constexpr bool2 result = vec0 <= vec2;
			REQUIRE(bool(result));
			REQUIRE(result.x);
			REQUIRE(result.y);
		}
		{
			constexpr bool2 result = vec0 <= const0;
			REQUIRE(!bool(result));
			REQUIRE(result.x);
			REQUIRE(!result.y);
		}
		{
			constexpr bool2 result = !bool2(true, false);
			REQUIRE(any(result));
			REQUIRE(!all(result));
			REQUIRE(!result.x);
			REQUIRE(result.y);
		}
	}

	SECTION("vector 3")
	{
		REQUIRE(float3(const0, const0, const0) == float3(const0));

		constexpr float3 vec0(const0, const3, const1);

		REQUIRE(vec0.x == const0);
		REQUIRE(vec0.xx == float2(const0, const0));
		REQUIRE(vec0.xxx == float3(const0, const0, const0));
		REQUIRE(vec0.xxxx == float4(const0, const0, const0, const0));

		REQUIRE(vec0.y == const3);
		REQUIRE(vec0.yy == float2(const3, const3));
		REQUIRE(vec0.yyy == float3(const3, const3, const3));
		REQUIRE(vec0.yyyy == float4(const3, const3, const3, const3));

		REQUIRE(vec0.z == const1);
		REQUIRE(vec0.zz == float2(const1, const1));
		REQUIRE(vec0.zzz == float3(const1, const1, const1));
		REQUIRE(vec0.zzzz == float4(const1, const1, const1, const1));

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

		REQUIRE(vec0.z == vec0.w);
		REQUIRE(vec0.z == vec0.b);
		REQUIRE(vec0.z == vec0.p);
		REQUIRE(vec0.zz == vec0.ww);
		REQUIRE(vec0.zz == vec0.bb);
		REQUIRE(vec0.zz == vec0.pp);
		REQUIRE(vec0.zzz == vec0.www);
		REQUIRE(vec0.zzz == vec0.bbb);
		REQUIRE(vec0.zzz == vec0.ppp);
		REQUIRE(vec0.zzzz == vec0.wwww);
		REQUIRE(vec0.zzzz == vec0.bbbb);
		REQUIRE(vec0.zzzz == vec0.pppp);

		REQUIRE(vec0.xyyx == float4(const0, const3, const3, const0));
		REQUIRE(vec0.xyxy == float4(const0, const3, const0, const3));
		REQUIRE(vec0.yxxy == float4(const3, const0, const0, const3));
		REQUIRE(vec0.yxyx == float4(const3, const0, const3, const0));

		REQUIRE(vec0.xzzx == float4(const0, const1, const1, const0));
		REQUIRE(vec0.xzxz == float4(const0, const1, const0, const1));
		REQUIRE(vec0.zxxz == float4(const1, const0, const0, const1));
		REQUIRE(vec0.zxzx == float4(const1, const0, const1, const0));

		REQUIRE(vec0.zyyz == float4(const1, const3, const3, const1));
		REQUIRE(vec0.zyzy == float4(const1, const3, const1, const3));
		REQUIRE(vec0.yzzy == float4(const3, const1, const1, const3));
		REQUIRE(vec0.yzyz == float4(const3, const1, const3, const1));

		REQUIRE(vec0.zyxz == float4(const1, const3, const0, const1));
		REQUIRE(vec0.zxzy == float4(const1, const0, const1, const3));
		REQUIRE(vec0.yzzx == float4(const3, const1, const1, const0));
		REQUIRE(vec0.yzxz == float4(const3, const1, const0, const1));

		REQUIRE(any(vec0 != const0));
		REQUIRE(!all(vec0 != const0));
		REQUIRE(all(vec0 != const2));

		constexpr float3 vec1(const2);

		REQUIRE(vec1 != const0);
		REQUIRE(vec0.x != vec1.x);
		REQUIRE(vec0.y != vec1.y);
		REQUIRE(vec0.z != vec1.z);
		REQUIRE(vec0 != vec1);

		constexpr float3 vec2(const0, const4, const1);

		{

			constexpr float3 result = vec0 + vec1;

			REQUIRE(result.x == (const0 + const2));
			REQUIRE(result.y == (const3 + const2));
			REQUIRE(result.z == (const1 + const2));
		}
		{

			constexpr float3 result = vec0 + const1;

			REQUIRE(result.x == (const0 + const1));
			REQUIRE(result.y == (const3 + const1));
			REQUIRE(result.z == (const1 + const1));
		}
		{
			constexpr float3 result = vec0 - vec1;
			REQUIRE(result.x == (const0 - const2));
			REQUIRE(result.y == (const3 - const2));
			REQUIRE(result.z == (const1 - const2));
		}
		{
			constexpr float3 result = vec0 - const1;
			REQUIRE(result.x == (const0 - const1));
			REQUIRE(result.y == (const3 - const1));
			REQUIRE(result.z == (const1 - const1));
		}
		{
			constexpr float3 result = vec0 * vec1;
			REQUIRE(result.x == (const0 * const2));
			REQUIRE(result.y == (const3 * const2));
			REQUIRE(result.z == (const1 * const2));
		}
		{
			constexpr float3 result = vec0 * const1;
			REQUIRE(result.x == (const0 * const1));
			REQUIRE(result.y == (const3 * const1));
			REQUIRE(result.z == (const1 * const1));
		}
		{
			constexpr float3 result = vec0 / vec1;
			REQUIRE(result.x == (const0 / const2));
			REQUIRE(result.y == (const3 / const2));
			REQUIRE(result.z == (const1 / const2));
		}
		{
			constexpr float3 result = vec0 / const1;
			REQUIRE(result.x == (const0 / const1));
			REQUIRE(result.y == (const3 / const1));
			REQUIRE(result.z == (const1 / const1));
		}
		{
			constexpr float3 result = -vec0;
			REQUIRE(result.x == (-const0));
			REQUIRE(result.y == (-const3));
			REQUIRE(result.z == (-const1));
		}
		{
			constexpr bool3 result = vec0 > vec1;
			REQUIRE(!bool(result));
			REQUIRE(!result.x);
			REQUIRE(result.y);
			REQUIRE(!result.z);
		}
		{
			constexpr bool3 result = vec0 > const1;
			REQUIRE(!bool(result));
			REQUIRE(!result.x);
			REQUIRE(result.y);
			REQUIRE(!result.z);
		}
		{
			constexpr bool3 result = vec0 < vec1;
			REQUIRE(!bool(result));
			REQUIRE(result.x);
			REQUIRE(!result.y);
			REQUIRE(result.z);
		}
		{
			constexpr bool3 result = vec0 < const1;
			REQUIRE(!bool(result));
			REQUIRE(result.x);
			REQUIRE(!result.y);
			REQUIRE(!result.z);
		}
		{
			constexpr bool3 result = vec0 >= vec2;
			REQUIRE(!bool(result));
			REQUIRE(result.x);
			REQUIRE(!result.y);
			REQUIRE(result.z);
		}
		{
			constexpr bool3 result = vec0 >= const3;
			REQUIRE(!bool(result));
			REQUIRE(!result.x);
			REQUIRE(result.y);
			REQUIRE(!result.z);
		}
		{
			constexpr bool3 result = vec0 <= vec2;
			REQUIRE(bool(result));
			REQUIRE(result.x);
			REQUIRE(result.y);
			REQUIRE(result.z);
		}
		{
			constexpr bool3 result = vec0 <= const0;
			REQUIRE(!bool(result));
			REQUIRE(result.x);
			REQUIRE(!result.y);
			REQUIRE(!result.z);
		}
		{
			constexpr bool3 result = !bool3(true, false, true);
			REQUIRE(any(result));
			REQUIRE(!all(result));
			REQUIRE(!result.x);
			REQUIRE(result.y);
			REQUIRE(!result.z);
		}
	}

	SECTION("vector 4")
	{
		REQUIRE(float4(const0, const0, const0, const0) == float4(const0));

		constexpr float4 vec0(const0, const3, const1, const5);

		REQUIRE(vec0.x == const0);
		REQUIRE(vec0.xx == float2(const0, const0));
		REQUIRE(vec0.xxx == float3(const0, const0, const0));
		REQUIRE(vec0.xxxx == float4(const0, const0, const0, const0));

		REQUIRE(vec0.y == const3);
		REQUIRE(vec0.yy == float2(const3, const3));
		REQUIRE(vec0.yyy == float3(const3, const3, const3));
		REQUIRE(vec0.yyyy == float4(const3, const3, const3, const3));

		REQUIRE(vec0.z == const1);
		REQUIRE(vec0.zz == float2(const1, const1));
		REQUIRE(vec0.zzz == float3(const1, const1, const1));
		REQUIRE(vec0.zzzz == float4(const1, const1, const1, const1));

		REQUIRE(vec0.x == vec0.r);
		REQUIRE(vec0.x == vec0.s);
		REQUIRE(vec0.xx == vec0.rr);
		REQUIRE(vec0.xx == vec0.ss);
		REQUIRE(vec0.xxx == vec0.rrr);
		REQUIRE(vec0.xxx == vec0.sss);
		REQUIRE(vec0.xxxx == vec0.rrrr);
		REQUIRE(vec0.xxxx == vec0.ssss);

		REQUIRE(vec0.y == vec0.g);
		REQUIRE(vec0.y == vec0.t);
		REQUIRE(vec0.yy == vec0.gg);
		REQUIRE(vec0.yy == vec0.tt);
		REQUIRE(vec0.yyy == vec0.ggg);
		REQUIRE(vec0.yyy == vec0.ttt);
		REQUIRE(vec0.yyyy == vec0.gggg);
		REQUIRE(vec0.yyyy == vec0.tttt);

		REQUIRE(vec0.z == vec0.b);
		REQUIRE(vec0.z == vec0.p);
		REQUIRE(vec0.zz == vec0.bb);
		REQUIRE(vec0.zz == vec0.pp);
		REQUIRE(vec0.zzz == vec0.bbb);
		REQUIRE(vec0.zzz == vec0.ppp);
		REQUIRE(vec0.zzzz == vec0.bbbb);
		REQUIRE(vec0.zzzz == vec0.pppp);

		REQUIRE(vec0.w == vec0.a);
		REQUIRE(vec0.w == vec0.q);
		REQUIRE(vec0.ww == vec0.aa);
		REQUIRE(vec0.ww == vec0.qq);
		REQUIRE(vec0.www == vec0.aaa);
		REQUIRE(vec0.www == vec0.qqq);
		REQUIRE(vec0.wwww == vec0.aaaa);
		REQUIRE(vec0.wwww == vec0.qqqq);

		REQUIRE(vec0.xyyx == float4(const0, const3, const3, const0));
		REQUIRE(vec0.xyxy == float4(const0, const3, const0, const3));
		REQUIRE(vec0.yxxy == float4(const3, const0, const0, const3));
		REQUIRE(vec0.yxyx == float4(const3, const0, const3, const0));

		REQUIRE(vec0.xzzx == float4(const0, const1, const1, const0));
		REQUIRE(vec0.xzxz == float4(const0, const1, const0, const1));
		REQUIRE(vec0.zxxz == float4(const1, const0, const0, const1));
		REQUIRE(vec0.zxzx == float4(const1, const0, const1, const0));

		REQUIRE(vec0.zyyz == float4(const1, const3, const3, const1));
		REQUIRE(vec0.zyzy == float4(const1, const3, const1, const3));
		REQUIRE(vec0.yzzy == float4(const3, const1, const1, const3));
		REQUIRE(vec0.yzyz == float4(const3, const1, const3, const1));

		REQUIRE(vec0.xwwx == float4(const0, const5, const5, const0));
		REQUIRE(vec0.xwxw == float4(const0, const5, const0, const5));
		REQUIRE(vec0.wxxw == float4(const5, const0, const0, const5));
		REQUIRE(vec0.wxwx == float4(const5, const0, const5, const0));

		REQUIRE(vec0.wzzw == float4(const5, const1, const1, const5));
		REQUIRE(vec0.wzwz == float4(const5, const1, const5, const1));
		REQUIRE(vec0.zwwz == float4(const1, const5, const5, const1));
		REQUIRE(vec0.zwzw == float4(const1, const5, const1, const5));

		REQUIRE(vec0.zyxz == float4(const1, const3, const0, const1));
		REQUIRE(vec0.zxzy == float4(const1, const0, const1, const3));
		REQUIRE(vec0.yzzx == float4(const3, const1, const1, const0));
		REQUIRE(vec0.yzxz == float4(const3, const1, const0, const1));

		REQUIRE(vec0.wyxz == float4(const5, const3, const0, const1));
		REQUIRE(vec0.zxwy == float4(const1, const0, const5, const3));
		REQUIRE(vec0.ywzx == float4(const3, const5, const1, const0));
		REQUIRE(vec0.yzxw == float4(const3, const1, const0, const5));

		REQUIRE(any(vec0 != const0));
		REQUIRE(!all(vec0 != const0));
		REQUIRE(all(vec0 != const2));

		constexpr float4 vec1(const2);

		REQUIRE(vec1 != const0);
		REQUIRE(vec0.x != vec1.x);
		REQUIRE(vec0.y != vec1.y);
		REQUIRE(vec0.z != vec1.z);
		REQUIRE(vec0.w != vec1.w);
		REQUIRE(vec0 != vec1);

		constexpr float4 vec2(const0, const4, const1, const5);

		{

			constexpr float4 result = vec0 + vec1;

			REQUIRE(result.x == (const0 + const2));
			REQUIRE(result.y == (const3 + const2));
			REQUIRE(result.z == (const1 + const2));
			REQUIRE(result.w == (const5 + const2));
		}
		{

			constexpr float4 result = vec0 + const1;

			REQUIRE(result.x == (const0 + const1));
			REQUIRE(result.y == (const3 + const1));
			REQUIRE(result.z == (const1 + const1));
			REQUIRE(result.w == (const5 + const1));
		}
		{
			constexpr float4 result = vec0 - vec1;
			REQUIRE(result.x == (const0 - const2));
			REQUIRE(result.y == (const3 - const2));
			REQUIRE(result.z == (const1 - const2));
			REQUIRE(result.w == (const5 - const2));
		}
		{
			constexpr float4 result = vec0 - const1;
			REQUIRE(result.x == (const0 - const1));
			REQUIRE(result.y == (const3 - const1));
			REQUIRE(result.z == (const1 - const1));
			REQUIRE(result.w == (const5 - const1));
		}
		{
			constexpr float4 result = vec0 * vec1;
			REQUIRE(result.x == (const0 * const2));
			REQUIRE(result.y == (const3 * const2));
			REQUIRE(result.z == (const1 * const2));
			REQUIRE(result.w == (const5 * const2));
		}
		{
			constexpr float4 result = vec0 * const1;
			REQUIRE(result.x == (const0 * const1));
			REQUIRE(result.y == (const3 * const1));
			REQUIRE(result.z == (const1 * const1));
			REQUIRE(result.w == (const5 * const1));
		}
		{
			constexpr float4 result = vec0 / vec1;
			REQUIRE(result.x == (const0 / const2));
			REQUIRE(result.y == (const3 / const2));
			REQUIRE(result.z == (const1 / const2));
			REQUIRE(result.w == (const5 / const2));
		}
		{
			constexpr float4 result = vec0 / const1;
			REQUIRE(result.x == (const0 / const1));
			REQUIRE(result.y == (const3 / const1));
			REQUIRE(result.z == (const1 / const1));
			REQUIRE(result.w == (const5 / const1));
		}
		{
			constexpr float4 result = -vec0;
			REQUIRE(result.x == (-const0));
			REQUIRE(result.y == (-const3));
			REQUIRE(result.z == (-const1));
			REQUIRE(result.w == (-const5));
		}
		{
			constexpr bool4 result = vec0 > vec1;
			REQUIRE(!bool(result));
			REQUIRE(!result.x);
			REQUIRE(result.y);
			REQUIRE(!result.z);
			REQUIRE(result.w);
		}
		{
			constexpr bool4 result = vec0 > const1;
			REQUIRE(!bool(result));
			REQUIRE(!result.x);
			REQUIRE(result.y);
			REQUIRE(!result.z);
			REQUIRE(result.w);
		}
		{
			constexpr bool4 result = vec0 < vec1;
			REQUIRE(!bool(result));
			REQUIRE(result.x);
			REQUIRE(!result.y);
			REQUIRE(result.z);
			REQUIRE(!result.w);
		}
		{
			constexpr bool4 result = vec0 < const1;
			REQUIRE(!bool(result));
			REQUIRE(result.x);
			REQUIRE(!result.y);
			REQUIRE(!result.z);
			REQUIRE(!result.w);
		}
		{
			constexpr bool4 result = vec0 >= vec2;
			REQUIRE(!bool(result));
			REQUIRE(result.x);
			REQUIRE(!result.y);
			REQUIRE(result.z);
			REQUIRE(result.w);
		}
		{
			constexpr bool4 result = vec0 >= const3;
			REQUIRE(!bool(result));
			REQUIRE(!result.x);
			REQUIRE(result.y);
			REQUIRE(!result.z);
			REQUIRE(result.w);
		}
		{
			constexpr bool4 result = vec0 <= vec2;
			REQUIRE(bool(result));
			REQUIRE(result.x);
			REQUIRE(result.y);
			REQUIRE(result.z);
			REQUIRE(result.w);
			REQUIRE(!any(!result));
		}
		{
			constexpr bool4 result = vec0 <= const0;
			REQUIRE(!bool(result));
			REQUIRE(result.x);
			REQUIRE(!result.y);
			REQUIRE(!result.z);
			REQUIRE(!result.w);
		}
		{
			constexpr bool4 result = !bool4(true, false, true, true);
			REQUIRE(any(result));
			REQUIRE(!all(result));
			REQUIRE(!result.x);
			REQUIRE(result.y);
			REQUIRE(!result.z);
			REQUIRE(!result.w);
		}
	}

	SECTION("vector length") {}
	SECTION("vector cross") {}
	SECTION("vector distance") {}
	SECTION("vector dot") {}
	SECTION("vector normalize") {}
	SECTION("vector inverse") {}
}

TEST_CASE("matrices", "[math]")
{
	using namespace rsl;
	using namespace rsl::math;

	SECTION("matrix1x1") {}
	SECTION("matrix2x2") {}
	SECTION("matrix3x3") {}
	SECTION("matrix4x4") {}

	SECTION("matrix decompose")
	{
		constexpr float4x4 matrix{};

		float3 scale;
		quat orientation;
		float3 translation;

		decompose(matrix, scale, orientation, translation);

		float4x4 out = compose(scale, orientation, translation);

		bool4x4 r = matrix == out;

		REQUIRE(r);
		REQUIRE(scale == float3::one);
		REQUIRE(orientation == quat::identity);
		REQUIRE(translation == float3::zero);
	}

	SECTION("matrix determinant") {}
	SECTION("matrix adjoint") {}
	SECTION("matrix inverse") {}
}

TEST_CASE("quaternions", "[math]")
{
	SECTION("quaternion") {}
	SECTION("quaternion matrix conversion") {}
	SECTION("quaternion length") {}
	SECTION("quaternion inverse") {}
}

TEST_CASE("generic functions", "[math]")
{
	SECTION("abs") {}
	SECTION("min") {}
	SECTION("max") {}
	SECTION("clamp") {}
	SECTION("saturate") {}
	SECTION("fmod") {}
	SECTION("modf") {}
	SECTION("round") {}
	SECTION("sign") {}
	SECTION("sqrt") {}
	SECTION("pow") {}
	SECTION("inverse sqrt") {}
}

TEST_CASE("interpolation", "[math]")
{
	SECTION("lerp") {}
	SECTION("map") {}
	SECTION("quintic") {}
	SECTION("smoothstep") {}
}

TEST_CASE("random & noise", "[math]")
{
	SECTION("random number engine") {}
	SECTION("rand") {}
	SECTION("linear rand") {}
	SECTION("circular rand") {}
	SECTION("spherical rand") {}
	SECTION("perlin") {}
	SECTION("value") {}
}
