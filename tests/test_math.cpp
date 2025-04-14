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

	SECTION("matrix translate rotate scale")
	{
		{
			constexpr float3 translation0{1, 2, 3};
			constexpr float3 translation1{6, 4, 2};
			constexpr float4x4 matrix0 = translate(float4x4{}, translation0);
			constexpr float4x4 matrix1 = translate(matrix0, translation1);

			REQUIRE(matrix0[0] == float4::right);
			REQUIRE(matrix0[1] == float4::up);
			REQUIRE(matrix0[2] == float4::forward);
			REQUIRE(matrix0[3].xyz == translation0);
			REQUIRE(matrix0[3].w == 1.f);

			REQUIRE(matrix1[0] == float4::right);
			REQUIRE(matrix1[1] == float4::up);
			REQUIRE(matrix1[2] == float4::forward);
			REQUIRE(matrix1[3].xyz == translation0 + translation1);
			REQUIRE(matrix1[3].w == 1.f);
		}
		{
			constexpr float3 scale0{1, 2, 3};
			constexpr float3 scale1{6, 4, 2.5f};
			constexpr float4x4 matrix0 = scale(float4x4{}, scale0);
			constexpr float4x4 matrix1 = scale(matrix0, scale1);

			REQUIRE(matrix0[0] == float4::right * scale0.x);
			REQUIRE(matrix0[1] == float4::up * scale0.y);
			REQUIRE(matrix0[2] == float4::forward * scale0.z);
			REQUIRE(matrix0[3] == float4::positiveW);

			REQUIRE(matrix1[0] == float4::right * scale0.x * scale1.x);
			REQUIRE(matrix1[1] == float4::up * scale0.y * scale1.y);
			REQUIRE(matrix1[2] == float4::forward * scale0.z * scale1.z);
			REQUIRE(matrix1[3] == float4::positiveW);
		}
		{
			const float4x4 matrix0 = rotate(float4x4{}, radians32::deg90, float3::up);
			const float4x4 matrix1 = rotate(matrix0, radians32::deg90, float3::right);

			REQUIRE(matrix0[0] == float4::backward);
			REQUIRE(matrix0[1] == float4::up);
			REQUIRE(matrix0[2] == float4::right);
			REQUIRE(matrix0[3] == float4::positiveW);

			REQUIRE(matrix1[0] == float4::backward);
			REQUIRE(matrix1[1] == float4::right);
			REQUIRE(matrix1[2] == float4::down);
			REQUIRE(matrix1[3] == float4::positiveW);
		}
	}

	SECTION("matrix decompose")
	{
		{
			constexpr float4x4 matrix{};

			float3 scale;
			quat orientation;
			float3 translation;

			decompose(matrix, scale, orientation, translation);

			float4x4 out = compose(scale, orientation, translation);

			REQUIRE(matrix == out);
			REQUIRE(scale == float3::one);
			REQUIRE(orientation == quat::identity);
			REQUIRE(translation == float3::zero);
		}
		{

			constexpr float3 pos(1.0f);
			constexpr float3 scal(1.0f);
			constexpr float3 axis(1.f, 2.f, 3.f);
			constexpr radians32 angle{2.329f};
			const quat expectedRotation = quat::angle_axis(angle, axis);

			float4x4 matrix = translate(rotate(scale(float4x4{}, scal), angle, axis), pos);

			float3 scale(3);
			quat orientation;
			float3 translation(3);

			decompose(matrix, scale, orientation, translation);

			float4x4 out = compose(scale, orientation, translation);

			REQUIRE(orientation == expectedRotation);
			REQUIRE(scale == scal);
			REQUIRE(translation == pos);
			REQUIRE(matrix == out);
		}
	}

	SECTION("matrix determinant") {}
	SECTION("matrix adjoint") {}
	SECTION("matrix inverse") {}

	SECTION("matrix transpose")
	{
		{
			constexpr float4x4 matrix{};
			constexpr float4x4 transposed = transpose(matrix);

			REQUIRE(transposed == matrix);
		}

		{
			float4x4 matrix{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
			float4x4 transposed = transpose(matrix);
			constexpr float4x4 expected{1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};

			REQUIRE(transposed == expected);
		}
	}

	SECTION("matrix multiplication") {}

	SECTION("matrix quaternion conversion")
	{
		{
			constexpr float4x4 matrix{};
			quat quaternion{matrix};

			REQUIRE(quaternion == quat::identity);
		}
		{
			constexpr float3x3 matrix{};
			quat quaternion{matrix};

			REQUIRE(quaternion == quat::identity);
		}
		{
			const float4x4 matrix{
				float4::right * 2.f, float4::up * 3.5f, float4::forward * 6.4f, float4{2.f, 2.5f, 3.2f, 1.f}
			};
			quat quaternion{matrix};

			REQUIRE(quaternion == quat::identity);
		}
		{
			const float3x3 matrix{float3::right * 3.12f, float3::up * 8.45f, float3::forward * 0.23f};
			quat quaternion{matrix};

			REQUIRE(quaternion == quat::identity);
		}
		{
			const float4x4 matrix{float4::right, float4::forward, float4::down, float4::positiveW};
			quat quaternion{matrix};

			REQUIRE(quaternion == quat::rotate_x_90);
		}
		{
			const float3x3 matrix{float3::right, float3::forward, float3::down};
			quat quaternion{matrix};

			REQUIRE(quaternion == quat::rotate_x_90);
		}
		{
			const float4x4 matrix{
				float4::right, normalize(float4::up + float4::forward), normalize(float4::forward + float4::down),
				float4::positiveW
			};
			const quat quaternion{matrix};

			REQUIRE(quaternion == quat::rotate_x_45);
		}
		{
			const float3x3 matrix{
				float3::right * 5.2125f, (float3::up + float3::forward) * 4.4647f,
				(float3::forward + float3::down) * 3.21654f
			};
			const quat quaternion{matrix};

			REQUIRE(quaternion == quat::rotate_x_45);
		}
		{
			const float4x4 matrix{
				float4::right * 0.52165f, (float4::up + float4::forward) * 0.154668f,
				(float4::forward + float4::down) * 11.7567f, float4{4868.f, 1543.125f, 124.155f, 1.f}
			};
			const quat quaternion{matrix};

			REQUIRE(quaternion == quat::rotate_x_45);
		}
		{
			const float3x3 matrix{
				float3::right, normalize(float3::up + float3::forward), normalize(float3::forward + float3::down)
			};
			const quat quaternion{matrix};

			REQUIRE(quaternion == quat::rotate_x_45);
		}
	}
}

TEST_CASE("quaternions", "[math]")
{
	using namespace rsl;
	using namespace rsl::math;

	constexpr static float32 sin22_5 = 0.3826834323650897717284599840304f;
	constexpr static float32 cos22_5 = 0.92387953251128675612818318939679f;
	constexpr static float32 sin45 = 0.70710678118654752440084436210485f;
	constexpr static float32 cos45 = 0.70710678118654752440084436210485f;

	constexpr static quat identity(1.f, 0.f, 0.f, 0.f);
	constexpr static quat rotate_x_45(cos22_5, sin22_5, 0.f, 0.f);
	constexpr static quat rotate_y_45(cos22_5, 0.f, sin22_5, 0.f);
	constexpr static quat rotate_z_45(cos22_5, 0.f, 0.f, sin22_5);
	constexpr static quat rotate_x_90(cos45, sin45, 0.f, 0.f);
	constexpr static quat rotate_y_90(cos45, 0.f, sin45, 0.f);
	constexpr static quat rotate_z_90(cos45, 0.f, 0.f, sin45);

	SECTION("quaternion")
	{
		{
			const quat quaternion = quat::angle_axis(radians32::deg45, float3::right);
			REQUIRE(quaternion == quat::rotate_x_45);
		}
		{
			const quat quaternion = quat::angle_axis(radians32::deg45, float3::up);
			REQUIRE(quaternion == quat::rotate_y_45);
		}
		{
			const quat quaternion = quat::angle_axis(radians32::deg45, float3::forward);
			REQUIRE(quaternion == quat::rotate_z_45);
		}
	}

	SECTION("quaternion matrix conversion")
	{
		{
			constexpr quat quaternion = identity;
			constexpr float4x4 matrix{quaternion};

			REQUIRE(matrix[0] == float4::right);
			REQUIRE(matrix[1] == float4::up);
			REQUIRE(matrix[2] == float4::forward);
			REQUIRE(matrix[3] == float4::positiveW);
		}
		{
			constexpr quat quaternion = identity;
			constexpr float3x3 matrix{quaternion};

			REQUIRE(matrix[0] == float3::right);
			REQUIRE(matrix[1] == float3::up);
			REQUIRE(matrix[2] == float3::forward);
		}
		{
			constexpr quat quaternion = rotate_x_90;
			constexpr float4x4 matrix{quaternion};

			REQUIRE(matrix[0] == float4::right);
			REQUIRE(matrix[1] == float4::forward);
			REQUIRE(matrix[2] == float4::down);
			REQUIRE(matrix[3] == float4::positiveW);
		}
		{
			constexpr quat quaternion = rotate_x_90;
			constexpr float3x3 matrix{quaternion};

			REQUIRE(matrix[0] == float3::right);
			REQUIRE(matrix[1] == float3::forward);
			REQUIRE(matrix[2] == float3::down);
		}
		{
			constexpr quat quaternion = rotate_y_90;
			constexpr float4x4 matrix{quaternion};

			REQUIRE(matrix[0] == float4::backward);
			REQUIRE(matrix[1] == float4::up);
			REQUIRE(matrix[2] == float4::right);
			REQUIRE(matrix[3] == float4::positiveW);
		}
		{
			constexpr quat quaternion = rotate_y_90;
			constexpr float3x3 matrix{quaternion};

			REQUIRE(matrix[0] == float3::backward);
			REQUIRE(matrix[1] == float3::up);
			REQUIRE(matrix[2] == float3::right);
		}
		{
			constexpr quat quaternion = rotate_z_90;
			constexpr float4x4 matrix{quaternion};

			REQUIRE(matrix[0] == float4::up);
			REQUIRE(matrix[1] == float4::left);
			REQUIRE(matrix[2] == float4::forward);
			REQUIRE(matrix[3] == float4::positiveW);
		}
		{
			constexpr quat quaternion = rotate_z_90;
			constexpr float3x3 matrix{quaternion};

			REQUIRE(matrix[0] == float3::up);
			REQUIRE(matrix[1] == float3::left);
			REQUIRE(matrix[2] == float3::forward);
		}

		{
			constexpr quat quaternion = rotate_x_45;
			constexpr float4x4 matrix{quaternion};

			REQUIRE(matrix[0] == float4::right);
			REQUIRE(matrix[1] == normalize(float4::up + float4::forward));
			REQUIRE(matrix[2] == normalize(float4::forward + float4::down));
			REQUIRE(matrix[3] == float4::positiveW);
		}
		{
			constexpr quat quaternion = rotate_x_45;
			constexpr float3x3 matrix{quaternion};

			REQUIRE(matrix[0] == float3::right);
			REQUIRE(matrix[1] == normalize(float3::up + float3::forward));
			REQUIRE(matrix[2] == normalize(float3::forward + float3::down));
		}
		{
			constexpr quat quaternion = rotate_y_45;
			constexpr float4x4 matrix{quaternion};

			REQUIRE(matrix[0] == normalize(float4::right + float4::backward));
			REQUIRE(matrix[1] == float4::up);
			REQUIRE(matrix[2] == normalize(float4::forward + float4::right));
			REQUIRE(matrix[3] == float4::positiveW);
		}
		{
			constexpr quat quaternion = rotate_y_45;
			constexpr float3x3 matrix{quaternion};

			REQUIRE(matrix[0] == normalize(float3::right + float3::backward));
			REQUIRE(matrix[1] == float3::up);
			REQUIRE(matrix[2] == normalize(float3::forward + float3::right));
		}
		{
			constexpr quat quaternion = rotate_z_45;
			constexpr float4x4 matrix{quaternion};

			REQUIRE(matrix[0] == normalize(float4::right + float4::up));
			REQUIRE(matrix[1] == normalize(float4::up + float4::left));
			REQUIRE(matrix[2] == float4::forward);
			REQUIRE(matrix[3] == float4::positiveW);
		}
		{
			constexpr quat quaternion = rotate_z_45;
			constexpr float3x3 matrix{quaternion};

			REQUIRE(matrix[0] == normalize(float3::right + float3::up));
			REQUIRE(matrix[1] == normalize(float3::up + float3::left));
			REQUIRE(matrix[2] == float3::forward);
		}
	}
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
