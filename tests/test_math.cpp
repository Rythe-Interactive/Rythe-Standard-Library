#include <catch2/catch_test_macros.hpp>

#define RYTHE_VALIDATE

#include <rsl/math>

[[nodiscard]] bool quat_absolute_equal(const rsl::math::quat& lhs, const rsl::math::quat& rhs) noexcept
{
	using namespace rsl;
	using namespace rsl::math;
	quat diff = inverse(lhs) * rhs;
	return close_enough(atan2(length(diff.vec), diff.w), 0.f);
}

[[nodiscard]] bool mat_is_orthogonal(const rsl::math::float4x4& mat)
{
	using namespace rsl;
	using namespace rsl::math;

	float3x3 orientation = mat;
	return (orientation * transpose(orientation)) == float3x3::identity;
}

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

		constexpr float1 normalized = normalize(vec0);
		constexpr float32 vecLength = length(vec0);

		CHECK(close_enough(vecLength, const0));
		CHECK(close_enough(normalized[0], 1.f));
		CHECK(close_enough(length(normalized), 1.f));

		CHECK(close_enough(vec0.x, const0));
		CHECK(vec0.xx == float2(const0, const0));
		CHECK(vec0.xxx == float3(const0, const0, const0));
		CHECK(vec0.xxxx == float4(const0, const0, const0, const0));

		CHECK(close_enough(vec0.x, vec0.u));
		CHECK(close_enough(vec0.x, vec0.r));
		CHECK(close_enough(vec0.x, vec0.s));
		CHECK(vec0.xx == vec0.uu);
		CHECK(vec0.xx == vec0.rr);
		CHECK(vec0.xx == vec0.ss);
		CHECK(vec0.xxx == vec0.uuu);
		CHECK(vec0.xxx == vec0.rrr);
		CHECK(vec0.xxx == vec0.sss);
		CHECK(vec0.xxxx == vec0.uuuu);
		CHECK(vec0.xxxx == vec0.rrrr);
		CHECK(vec0.xxxx == vec0.ssss);

		constexpr float1 vec1(const1);

		CHECK(vec1 != const0);
		CHECK(vec0.x != vec1.x);
		CHECK(vec0 != vec1);

		{
			constexpr float1 result = vec0 + vec1;
			CHECK(close_enough(result.x, (const0 + const1)));
		}
		{
			constexpr float1 result = vec0 - vec1;
			CHECK(close_enough(result.x, (const0 - const1)));
		}
		{
			constexpr float1 result = vec0 * vec1;
			CHECK(close_enough(result.x, (const0 * const1)));
		}
		{
			constexpr float1 result = vec0 / vec1;
			CHECK(close_enough(result.x, (const0 / const1)));
		}
		{
			constexpr float1 result = -vec0;
			CHECK(close_enough(result.x, (-const0)));
		}
		{
			constexpr bool1 result = vec0 > vec1;
			CHECK(!bool(result));
		}
		{
			constexpr bool1 result = vec0 < vec1;
			CHECK(result);
		}
		{
			constexpr bool1 result = !bool1(false);
			CHECK(result);
		}
	}

	SECTION("vector 2")
	{
		CHECK(float2(const0, const0) == float2(const0));

		constexpr float2 vec0(const0, const3);

		constexpr float2 normalized = normalize(vec0);
		constexpr float32 vecLength = length(vec0);

		CHECK(close_enough(vecLength, sqrt(const0 * const0 + const3 * const3)));
		CHECK(close_enough(normalized[0], vec0[0] / vecLength));
		CHECK(close_enough(normalized[1], vec0[1] / vecLength));
		CHECK(close_enough(length(normalized), 1.f));

		CHECK(close_enough(vec0.x, const0));
		CHECK(vec0.xx == float2(const0, const0));
		CHECK(vec0.xxx == float3(const0, const0, const0));
		CHECK(vec0.xxxx == float4(const0, const0, const0, const0));

		CHECK(close_enough(vec0.y, const3));
		CHECK(vec0.yy == float2(const3, const3));
		CHECK(vec0.yyy == float3(const3, const3, const3));
		CHECK(vec0.yyyy == float4(const3, const3, const3, const3));

		CHECK(close_enough(vec0.x, vec0.u));
		CHECK(close_enough(vec0.x, vec0.r));
		CHECK(close_enough(vec0.x, vec0.s));
		CHECK(vec0.xx == vec0.uu);
		CHECK(vec0.xx == vec0.rr);
		CHECK(vec0.xx == vec0.ss);
		CHECK(vec0.xxx == vec0.uuu);
		CHECK(vec0.xxx == vec0.rrr);
		CHECK(vec0.xxx == vec0.sss);
		CHECK(vec0.xxxx == vec0.uuuu);
		CHECK(vec0.xxxx == vec0.rrrr);
		CHECK(vec0.xxxx == vec0.ssss);

		CHECK(close_enough(vec0.y, vec0.v));
		CHECK(close_enough(vec0.y, vec0.g));
		CHECK(close_enough(vec0.y, vec0.t));
		CHECK(vec0.yy == vec0.vv);
		CHECK(vec0.yy == vec0.gg);
		CHECK(vec0.yy == vec0.tt);
		CHECK(vec0.yyy == vec0.vvv);
		CHECK(vec0.yyy == vec0.ggg);
		CHECK(vec0.yyy == vec0.ttt);
		CHECK(vec0.yyyy == vec0.vvvv);
		CHECK(vec0.yyyy == vec0.gggg);
		CHECK(vec0.yyyy == vec0.tttt);

		CHECK(vec0.xyyx == float4(const0, const3, const3, const0));
		CHECK(vec0.xyxy == float4(const0, const3, const0, const3));
		CHECK(vec0.yxxy == float4(const3, const0, const0, const3));
		CHECK(vec0.yxyx == float4(const3, const0, const3, const0));

		CHECK(any(vec0 != const0));
		CHECK(!all(vec0 != const0));
		CHECK(all(vec0 != const2));

		constexpr float2 vec1(const2);

		CHECK(vec1 != const0);
		CHECK(!close_enough(vec0.x, vec1.x));
		CHECK(!close_enough(vec0.y, vec1.y));
		CHECK(vec0 != vec1);

		constexpr float2 vec2(const0, const4);

		{

			constexpr float2 result = vec0 + vec1;

			CHECK(close_enough(result.x, (const0 + const2)));
			CHECK(close_enough(result.y, (const3 + const2)));
		}
		{

			constexpr float2 result = vec0 + const1;

			CHECK(close_enough(result.x, (const0 + const1)));
			CHECK(close_enough(result.y, (const3 + const1)));
		}
		{
			constexpr float2 result = vec0 - vec1;
			CHECK(close_enough(result.x, (const0 - const2)));
			CHECK(close_enough(result.y, (const3 - const2)));
		}
		{
			constexpr float2 result = vec0 - const1;
			CHECK(close_enough(result.x, (const0 - const1)));
			CHECK(close_enough(result.y, (const3 - const1)));
		}
		{
			constexpr float2 result = vec0 * vec1;
			CHECK(close_enough(result.x, (const0 * const2)));
			CHECK(close_enough(result.y, (const3 * const2)));
		}
		{
			constexpr float2 result = vec0 * const1;
			CHECK(close_enough(result.x, (const0 * const1)));
			CHECK(close_enough(result.y, (const3 * const1)));
		}
		{
			constexpr float2 result = vec0 / vec1;
			CHECK(close_enough(result.x, (const0 / const2)));
			CHECK(close_enough(result.y, (const3 / const2)));
		}
		{
			constexpr float2 result = vec0 / const1;
			CHECK(close_enough(result.x, (const0 / const1)));
			CHECK(close_enough(result.y, (const3 / const1)));
		}
		{
			constexpr float2 result = -vec0;
			CHECK(close_enough(result.x, (-const0)));
			CHECK(close_enough(result.y, (-const3)));
		}
		{
			constexpr bool2 result = vec0 > vec1;
			CHECK(!bool(result));
			CHECK(!result.x);
			CHECK(result.y);
		}
		{
			constexpr bool2 result = vec0 > const1;
			CHECK(!bool(result));
			CHECK(!result.x);
			CHECK(result.y);
		}
		{
			constexpr bool2 result = vec0 < vec1;
			CHECK(!bool(result));
			CHECK(result.x);
			CHECK(!result.y);
		}
		{
			constexpr bool2 result = vec0 < const1;
			CHECK(!bool(result));
			CHECK(result.x);
			CHECK(!result.y);
		}
		{
			constexpr bool2 result = vec0 >= vec2;
			CHECK(!bool(result));
			CHECK(result.x);
			CHECK(!result.y);
		}
		{
			constexpr bool2 result = vec0 >= const3;
			CHECK(!bool(result));
			CHECK(!result.x);
			CHECK(result.y);
		}
		{
			constexpr bool2 result = vec0 <= vec2;
			CHECK(bool(result));
			CHECK(result.x);
			CHECK(result.y);
		}
		{
			constexpr bool2 result = vec0 <= const0;
			CHECK(!bool(result));
			CHECK(result.x);
			CHECK(!result.y);
		}
		{
			constexpr bool2 result = !bool2(true, false);
			CHECK(any(result));
			CHECK(!all(result));
			CHECK(!result.x);
			CHECK(result.y);
		}
	}

	SECTION("vector 3")
	{
		CHECK(float3(const0, const0, const0) == float3(const0));

		constexpr float3 vec0(const0, const3, const1);

		constexpr float3 normalized = normalize(vec0);
		constexpr float32 vecLength = length(vec0);

		CHECK(close_enough(vecLength, sqrt(const0 * const0 + const3 * const3 + const1 * const1)));
		CHECK(close_enough(normalized[0], vec0[0] / vecLength));
		CHECK(close_enough(normalized[1], vec0[1] / vecLength));
		CHECK(close_enough(normalized[2], vec0[2] / vecLength));
		CHECK(close_enough(length(normalized), 1.f));

		CHECK(close_enough(vec0.x, const0));
		CHECK(vec0.xx == float2(const0, const0));
		CHECK(vec0.xxx == float3(const0, const0, const0));
		CHECK(vec0.xxxx == float4(const0, const0, const0, const0));

		CHECK(close_enough(vec0.y, const3));
		CHECK(vec0.yy == float2(const3, const3));
		CHECK(vec0.yyy == float3(const3, const3, const3));
		CHECK(vec0.yyyy == float4(const3, const3, const3, const3));

		CHECK(close_enough(vec0.z, const1));
		CHECK(vec0.zz == float2(const1, const1));
		CHECK(vec0.zzz == float3(const1, const1, const1));
		CHECK(vec0.zzzz == float4(const1, const1, const1, const1));

		CHECK(close_enough(vec0.x, vec0.u));
		CHECK(close_enough(vec0.x, vec0.r));
		CHECK(close_enough(vec0.x, vec0.s));
		CHECK(vec0.xx == vec0.uu);
		CHECK(vec0.xx == vec0.rr);
		CHECK(vec0.xx == vec0.ss);
		CHECK(vec0.xxx == vec0.uuu);
		CHECK(vec0.xxx == vec0.rrr);
		CHECK(vec0.xxx == vec0.sss);
		CHECK(vec0.xxxx == vec0.uuuu);
		CHECK(vec0.xxxx == vec0.rrrr);
		CHECK(vec0.xxxx == vec0.ssss);

		CHECK(close_enough(vec0.y, vec0.v));
		CHECK(close_enough(vec0.y, vec0.g));
		CHECK(close_enough(vec0.y, vec0.t));
		CHECK(vec0.yy == vec0.vv);
		CHECK(vec0.yy == vec0.gg);
		CHECK(vec0.yy == vec0.tt);
		CHECK(vec0.yyy == vec0.vvv);
		CHECK(vec0.yyy == vec0.ggg);
		CHECK(vec0.yyy == vec0.ttt);
		CHECK(vec0.yyyy == vec0.vvvv);
		CHECK(vec0.yyyy == vec0.gggg);
		CHECK(vec0.yyyy == vec0.tttt);

		CHECK(close_enough(vec0.z, vec0.w));
		CHECK(close_enough(vec0.z, vec0.b));
		CHECK(close_enough(vec0.z, vec0.p));
		CHECK(vec0.zz == vec0.ww);
		CHECK(vec0.zz == vec0.bb);
		CHECK(vec0.zz == vec0.pp);
		CHECK(vec0.zzz == vec0.www);
		CHECK(vec0.zzz == vec0.bbb);
		CHECK(vec0.zzz == vec0.ppp);
		CHECK(vec0.zzzz == vec0.wwww);
		CHECK(vec0.zzzz == vec0.bbbb);
		CHECK(vec0.zzzz == vec0.pppp);

		CHECK(vec0.xyyx == float4(const0, const3, const3, const0));
		CHECK(vec0.xyxy == float4(const0, const3, const0, const3));
		CHECK(vec0.yxxy == float4(const3, const0, const0, const3));
		CHECK(vec0.yxyx == float4(const3, const0, const3, const0));

		CHECK(vec0.xzzx == float4(const0, const1, const1, const0));
		CHECK(vec0.xzxz == float4(const0, const1, const0, const1));
		CHECK(vec0.zxxz == float4(const1, const0, const0, const1));
		CHECK(vec0.zxzx == float4(const1, const0, const1, const0));

		CHECK(vec0.zyyz == float4(const1, const3, const3, const1));
		CHECK(vec0.zyzy == float4(const1, const3, const1, const3));
		CHECK(vec0.yzzy == float4(const3, const1, const1, const3));
		CHECK(vec0.yzyz == float4(const3, const1, const3, const1));

		CHECK(vec0.zyxz == float4(const1, const3, const0, const1));
		CHECK(vec0.zxzy == float4(const1, const0, const1, const3));
		CHECK(vec0.yzzx == float4(const3, const1, const1, const0));
		CHECK(vec0.yzxz == float4(const3, const1, const0, const1));

		CHECK(any(vec0 != const0));
		CHECK(!all(vec0 != const0));
		CHECK(all(vec0 != const2));

		constexpr float3 vec1(const2);

		CHECK(vec1 != const0);
		CHECK(!close_enough(vec0.x, vec1.x));
		CHECK(!close_enough(vec0.y, vec1.y));
		CHECK(!close_enough(vec0.z, vec1.z));
		CHECK(vec0 != vec1);

		constexpr float3 vec2(const0, const4, const1);

		{

			constexpr float3 result = vec0 + vec1;

			CHECK(close_enough(result.x, (const0 + const2)));
			CHECK(close_enough(result.y, (const3 + const2)));
			CHECK(close_enough(result.z, (const1 + const2)));
		}
		{

			constexpr float3 result = vec0 + const1;

			CHECK(close_enough(result.x, (const0 + const1)));
			CHECK(close_enough(result.y, (const3 + const1)));
			CHECK(close_enough(result.z, (const1 + const1)));
		}
		{
			constexpr float3 result = vec0 - vec1;
			CHECK(close_enough(result.x, (const0 - const2)));
			CHECK(close_enough(result.y, (const3 - const2)));
			CHECK(close_enough(result.z, (const1 - const2)));
		}
		{
			constexpr float3 result = vec0 - const1;
			CHECK(close_enough(result.x, (const0 - const1)));
			CHECK(close_enough(result.y, (const3 - const1)));
			CHECK(close_enough(result.z, (const1 - const1)));
		}
		{
			constexpr float3 result = vec0 * vec1;
			CHECK(close_enough(result.x, (const0 * const2)));
			CHECK(close_enough(result.y, (const3 * const2)));
			CHECK(close_enough(result.z, (const1 * const2)));
		}
		{
			constexpr float3 result = vec0 * const1;
			CHECK(close_enough(result.x, (const0 * const1)));
			CHECK(close_enough(result.y, (const3 * const1)));
			CHECK(close_enough(result.z, (const1 * const1)));
		}
		{
			constexpr float3 result = vec0 / vec1;
			CHECK(close_enough(result.x, (const0 / const2)));
			CHECK(close_enough(result.y, (const3 / const2)));
			CHECK(close_enough(result.z, (const1 / const2)));
		}
		{
			constexpr float3 result = vec0 / const1;
			CHECK(close_enough(result.x, (const0 / const1)));
			CHECK(close_enough(result.y, (const3 / const1)));
			CHECK(close_enough(result.z, (const1 / const1)));
		}
		{
			constexpr float3 result = -vec0;
			CHECK(close_enough(result.x, (-const0)));
			CHECK(close_enough(result.y, (-const3)));
			CHECK(close_enough(result.z, (-const1)));
		}
		{
			constexpr bool3 result = vec0 > vec1;
			CHECK(!bool(result));
			CHECK(!result.x);
			CHECK(result.y);
			CHECK(!result.z);
		}
		{
			constexpr bool3 result = vec0 > const1;
			CHECK(!bool(result));
			CHECK(!result.x);
			CHECK(result.y);
			CHECK(!result.z);
		}
		{
			constexpr bool3 result = vec0 < vec1;
			CHECK(!bool(result));
			CHECK(result.x);
			CHECK(!result.y);
			CHECK(result.z);
		}
		{
			constexpr bool3 result = vec0 < const1;
			CHECK(!bool(result));
			CHECK(result.x);
			CHECK(!result.y);
			CHECK(!result.z);
		}
		{
			constexpr bool3 result = vec0 >= vec2;
			CHECK(!bool(result));
			CHECK(result.x);
			CHECK(!result.y);
			CHECK(result.z);
		}
		{
			constexpr bool3 result = vec0 >= const3;
			CHECK(!bool(result));
			CHECK(!result.x);
			CHECK(result.y);
			CHECK(!result.z);
		}
		{
			constexpr bool3 result = vec0 <= vec2;
			CHECK(bool(result));
			CHECK(result.x);
			CHECK(result.y);
			CHECK(result.z);
		}
		{
			constexpr bool3 result = vec0 <= const0;
			CHECK(!bool(result));
			CHECK(result.x);
			CHECK(!result.y);
			CHECK(!result.z);
		}
		{
			constexpr bool3 result = !bool3(true, false, true);
			CHECK(any(result));
			CHECK(!all(result));
			CHECK(!result.x);
			CHECK(result.y);
			CHECK(!result.z);
		}
	}

	SECTION("vector 4")
	{
		CHECK(float4(const0, const0, const0, const0) == float4(const0));

		constexpr float4 vec0(const0, const3, const1, const5);

		constexpr float4 normalized = normalize(vec0);
		constexpr float32 vecLength = length(vec0);

		CHECK(close_enough(vecLength, sqrt(const0 * const0 + const3 * const3 + const1 * const1 + const5 * const5)));
		CHECK(close_enough(normalized[0], vec0[0] / vecLength));
		CHECK(close_enough(normalized[1], vec0[1] / vecLength));
		CHECK(close_enough(normalized[2], vec0[2] / vecLength));
		CHECK(close_enough(normalized[3], vec0[3] / vecLength));
		CHECK(close_enough(length(normalized), 1.f));

		CHECK(close_enough(vec0.x, const0));
		CHECK(vec0.xx == float2(const0, const0));
		CHECK(vec0.xxx == float3(const0, const0, const0));
		CHECK(vec0.xxxx == float4(const0, const0, const0, const0));

		CHECK(close_enough(vec0.y, const3));
		CHECK(vec0.yy == float2(const3, const3));
		CHECK(vec0.yyy == float3(const3, const3, const3));
		CHECK(vec0.yyyy == float4(const3, const3, const3, const3));

		CHECK(close_enough(vec0.z, const1));
		CHECK(vec0.zz == float2(const1, const1));
		CHECK(vec0.zzz == float3(const1, const1, const1));
		CHECK(vec0.zzzz == float4(const1, const1, const1, const1));

		CHECK(close_enough(vec0.x, vec0.r));
		CHECK(close_enough(vec0.x, vec0.s));
		CHECK(vec0.xx == vec0.rr);
		CHECK(vec0.xx == vec0.ss);
		CHECK(vec0.xxx == vec0.rrr);
		CHECK(vec0.xxx == vec0.sss);
		CHECK(vec0.xxxx == vec0.rrrr);
		CHECK(vec0.xxxx == vec0.ssss);

		CHECK(close_enough(vec0.y, vec0.g));
		CHECK(close_enough(vec0.y, vec0.t));
		CHECK(vec0.yy == vec0.gg);
		CHECK(vec0.yy == vec0.tt);
		CHECK(vec0.yyy == vec0.ggg);
		CHECK(vec0.yyy == vec0.ttt);
		CHECK(vec0.yyyy == vec0.gggg);
		CHECK(vec0.yyyy == vec0.tttt);

		CHECK(close_enough(vec0.z, vec0.b));
		CHECK(close_enough(vec0.z, vec0.p));
		CHECK(vec0.zz == vec0.bb);
		CHECK(vec0.zz == vec0.pp);
		CHECK(vec0.zzz == vec0.bbb);
		CHECK(vec0.zzz == vec0.ppp);
		CHECK(vec0.zzzz == vec0.bbbb);
		CHECK(vec0.zzzz == vec0.pppp);

		CHECK(close_enough(vec0.w, vec0.a));
		CHECK(close_enough(vec0.w, vec0.q));
		CHECK(vec0.ww == vec0.aa);
		CHECK(vec0.ww == vec0.qq);
		CHECK(vec0.www == vec0.aaa);
		CHECK(vec0.www == vec0.qqq);
		CHECK(vec0.wwww == vec0.aaaa);
		CHECK(vec0.wwww == vec0.qqqq);

		CHECK(vec0.xyyx == float4(const0, const3, const3, const0));
		CHECK(vec0.xyxy == float4(const0, const3, const0, const3));
		CHECK(vec0.yxxy == float4(const3, const0, const0, const3));
		CHECK(vec0.yxyx == float4(const3, const0, const3, const0));

		CHECK(vec0.xzzx == float4(const0, const1, const1, const0));
		CHECK(vec0.xzxz == float4(const0, const1, const0, const1));
		CHECK(vec0.zxxz == float4(const1, const0, const0, const1));
		CHECK(vec0.zxzx == float4(const1, const0, const1, const0));

		CHECK(vec0.zyyz == float4(const1, const3, const3, const1));
		CHECK(vec0.zyzy == float4(const1, const3, const1, const3));
		CHECK(vec0.yzzy == float4(const3, const1, const1, const3));
		CHECK(vec0.yzyz == float4(const3, const1, const3, const1));

		CHECK(vec0.xwwx == float4(const0, const5, const5, const0));
		CHECK(vec0.xwxw == float4(const0, const5, const0, const5));
		CHECK(vec0.wxxw == float4(const5, const0, const0, const5));
		CHECK(vec0.wxwx == float4(const5, const0, const5, const0));

		CHECK(vec0.wzzw == float4(const5, const1, const1, const5));
		CHECK(vec0.wzwz == float4(const5, const1, const5, const1));
		CHECK(vec0.zwwz == float4(const1, const5, const5, const1));
		CHECK(vec0.zwzw == float4(const1, const5, const1, const5));

		CHECK(vec0.zyxz == float4(const1, const3, const0, const1));
		CHECK(vec0.zxzy == float4(const1, const0, const1, const3));
		CHECK(vec0.yzzx == float4(const3, const1, const1, const0));
		CHECK(vec0.yzxz == float4(const3, const1, const0, const1));

		CHECK(vec0.wyxz == float4(const5, const3, const0, const1));
		CHECK(vec0.zxwy == float4(const1, const0, const5, const3));
		CHECK(vec0.ywzx == float4(const3, const5, const1, const0));
		CHECK(vec0.yzxw == float4(const3, const1, const0, const5));

		CHECK(any(vec0 != const0));
		CHECK(!all(vec0 != const0));
		CHECK(all(vec0 != const2));

		constexpr float4 vec1(const2);

		CHECK(vec1 != const0);
		CHECK(!close_enough(vec0.x, vec1.x));
		CHECK(!close_enough(vec0.y, vec1.y));
		CHECK(!close_enough(vec0.z, vec1.z));
		CHECK(!close_enough(vec0.w, vec1.w));
		CHECK(vec0 != vec1);

		constexpr float4 vec2(const0, const4, const1, const5);

		{

			constexpr float4 result = vec0 + vec1;

			CHECK(close_enough(result.x, (const0 + const2)));
			CHECK(close_enough(result.y, (const3 + const2)));
			CHECK(close_enough(result.z, (const1 + const2)));
			CHECK(close_enough(result.w, (const5 + const2)));
		}
		{

			constexpr float4 result = vec0 + const1;

			CHECK(close_enough(result.x, (const0 + const1)));
			CHECK(close_enough(result.y, (const3 + const1)));
			CHECK(close_enough(result.z, (const1 + const1)));
			CHECK(close_enough(result.w, (const5 + const1)));
		}
		{
			constexpr float4 result = vec0 - vec1;
			CHECK(close_enough(result.x, (const0 - const2)));
			CHECK(close_enough(result.y, (const3 - const2)));
			CHECK(close_enough(result.z, (const1 - const2)));
			CHECK(close_enough(result.w, (const5 - const2)));
		}
		{
			constexpr float4 result = vec0 - const1;
			CHECK(close_enough(result.x, (const0 - const1)));
			CHECK(close_enough(result.y, (const3 - const1)));
			CHECK(close_enough(result.z, (const1 - const1)));
			CHECK(close_enough(result.w, (const5 - const1)));
		}
		{
			constexpr float4 result = vec0 * vec1;
			CHECK(close_enough(result.x, (const0 * const2)));
			CHECK(close_enough(result.y, (const3 * const2)));
			CHECK(close_enough(result.z, (const1 * const2)));
			CHECK(close_enough(result.w, (const5 * const2)));
		}
		{
			constexpr float4 result = vec0 * const1;
			CHECK(close_enough(result.x, (const0 * const1)));
			CHECK(close_enough(result.y, (const3 * const1)));
			CHECK(close_enough(result.z, (const1 * const1)));
			CHECK(close_enough(result.w, (const5 * const1)));
		}
		{
			constexpr float4 result = vec0 / vec1;
			CHECK(close_enough(result.x, (const0 / const2)));
			CHECK(close_enough(result.y, (const3 / const2)));
			CHECK(close_enough(result.z, (const1 / const2)));
			CHECK(close_enough(result.w, (const5 / const2)));
		}
		{
			constexpr float4 result = vec0 / const1;
			CHECK(close_enough(result.x, (const0 / const1)));
			CHECK(close_enough(result.y, (const3 / const1)));
			CHECK(close_enough(result.z, (const1 / const1)));
			CHECK(close_enough(result.w, (const5 / const1)));
		}
		{
			constexpr float4 result = -vec0;
			CHECK(close_enough(result.x, (-const0)));
			CHECK(close_enough(result.y, (-const3)));
			CHECK(close_enough(result.z, (-const1)));
			CHECK(close_enough(result.w, (-const5)));
		}
		{
			constexpr bool4 result = vec0 > vec1;
			CHECK(!bool(result));
			CHECK(!result.x);
			CHECK(result.y);
			CHECK(!result.z);
			CHECK(result.w);
		}
		{
			constexpr bool4 result = vec0 > const1;
			CHECK(!bool(result));
			CHECK(!result.x);
			CHECK(result.y);
			CHECK(!result.z);
			CHECK(result.w);
		}
		{
			constexpr bool4 result = vec0 < vec1;
			CHECK(!bool(result));
			CHECK(result.x);
			CHECK(!result.y);
			CHECK(result.z);
			CHECK(!result.w);
		}
		{
			constexpr bool4 result = vec0 < const1;
			CHECK(!bool(result));
			CHECK(result.x);
			CHECK(!result.y);
			CHECK(!result.z);
			CHECK(!result.w);
		}
		{
			constexpr bool4 result = vec0 >= vec2;
			CHECK(!bool(result));
			CHECK(result.x);
			CHECK(!result.y);
			CHECK(result.z);
			CHECK(result.w);
		}
		{
			constexpr bool4 result = vec0 >= const3;
			CHECK(!bool(result));
			CHECK(!result.x);
			CHECK(result.y);
			CHECK(!result.z);
			CHECK(result.w);
		}
		{
			constexpr bool4 result = vec0 <= vec2;
			CHECK(bool(result));
			CHECK(result.x);
			CHECK(result.y);
			CHECK(result.z);
			CHECK(result.w);
			CHECK(!any(!result));
		}
		{
			constexpr bool4 result = vec0 <= const0;
			CHECK(!bool(result));
			CHECK(result.x);
			CHECK(!result.y);
			CHECK(!result.z);
			CHECK(!result.w);
		}
		{
			constexpr bool4 result = !bool4(true, false, true, true);
			CHECK(any(result));
			CHECK(!all(result));
			CHECK(!result.x);
			CHECK(result.y);
			CHECK(!result.z);
			CHECK(!result.w);
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

			CHECK(matrix0[0] == float4::right);
			CHECK(matrix0[1] == float4::up);
			CHECK(matrix0[2] == float4::forward);
			CHECK(matrix0[3].xyz == translation0);
			CHECK(close_enough(matrix0[3].w, 1.f));

			CHECK(matrix1[0] == float4::right);
			CHECK(matrix1[1] == float4::up);
			CHECK(matrix1[2] == float4::forward);
			CHECK(matrix1[3].xyz == translation0 + translation1);
			CHECK(close_enough(matrix1[3].w, 1.f));
		}
		{
			constexpr float3 scale0{1, 2, 3};
			constexpr float3 scale1{6, 4, 2.5f};
			constexpr float4x4 matrix0 = scale(float4x4{}, scale0);
			constexpr float4x4 matrix1 = scale(matrix0, scale1);

			CHECK(matrix0[0] == float4::right * scale0.x);
			CHECK(matrix0[1] == float4::up * scale0.y);
			CHECK(matrix0[2] == float4::forward * scale0.z);
			CHECK(matrix0[3] == float4::positiveW);

			CHECK(matrix1[0] == float4::right * scale0.x * scale1.x);
			CHECK(matrix1[1] == float4::up * scale0.y * scale1.y);
			CHECK(matrix1[2] == float4::forward * scale0.z * scale1.z);
			CHECK(matrix1[3] == float4::positiveW);
		}
		{
			const float4x4 matrix0 = rotate(float4x4{}, radians32::deg90, float3::up);
			const float4x4 matrix1 = rotate(matrix0, radians32::deg90, float3::right);

			CHECK(matrix0[0] == float4::backward);
			CHECK(matrix0[1] == float4::up);
			CHECK(matrix0[2] == float4::right);
			CHECK(matrix0[3] == float4::positiveW);

			CHECK(matrix1[0] == float4::backward);
			CHECK(matrix1[1] == float4::right);
			CHECK(matrix1[2] == float4::down);
			CHECK(matrix1[3] == float4::positiveW);
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

			CHECK(mat_is_orthogonal(out));
			CHECK(mat_is_orthogonal(matrix));
			CHECK(matrix == out);
			CHECK(scale == float3::one);
			CHECK(quat_absolute_equal(orientation, quat::identity));
			CHECK(translation == float3::zero);
		}
		{

			constexpr float3 pos(1.f);
			constexpr float3 scal(1.f);
			constexpr float3 axis(1.f, 2.f, 3.f);
			constexpr radians32 angle{2.329f};
			const quat expectedRotation = quat::angle_axis(angle, axis);

			float4x4 matrix = translate(rotate(scale(float4x4{}, scal), angle, axis), pos);

			REQUIRE(mat_is_orthogonal(matrix));

			float3 scale(3);
			quat orientation;
			float3 translation(3);

			decompose(matrix, scale, orientation, translation);

			REQUIRE(close_enough(length(orientation), 1.f));

			float4x4 out = compose(scale, orientation, translation);

			CHECK(mat_is_orthogonal(out));
			CHECK(quat_absolute_equal(orientation, expectedRotation));
			CHECK(scale == scal);
			CHECK(translation == pos);
			CHECK(matrix == out);
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

			CHECK(transposed == matrix);
		}

		{
			float4x4 matrix{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
			float4x4 transposed = transpose(matrix);
			constexpr float4x4 expected{1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};

			CHECK(transposed == expected);
		}
	}

	SECTION("matrix multiplication")
	{
		{
			float4 vec(1.f, 0.f, 0.f, 1.f);
			float4x4 trans = float4x4(1.f);
			trans = translate(trans, float3(1.f, 1.f, 0.f));
			vec = trans * vec;

			CHECK(vec == float4(2.f, 1.f, 0.f, 1.f));
		}
	}

	SECTION("matrix quaternion conversion")
	{
		{
			constexpr float4x4 matrix{};
			quat quaternion{matrix};

			CHECK(quat_absolute_equal(quaternion, quat::identity));
		}
		{
			constexpr float3x3 matrix{};
			quat quaternion{matrix};

			CHECK(quat_absolute_equal(quaternion, quat::identity));
		}
		{
			const float4x4 matrix{
				float4::right * 2.f, float4::up * 3.5f, float4::forward * 6.4f, float4{2.f, 2.5f, 3.2f, 1.f}
			};
			quat quaternion{matrix};

			CHECK(quat_absolute_equal(quaternion, quat::identity));
		}
		{
			const float3x3 matrix{float3::right * 3.12f, float3::up * 8.45f, float3::forward * 0.23f};
			quat quaternion{matrix};

			CHECK(quat_absolute_equal(quaternion, quat::identity));
		}
		{
			const float4x4 matrix{float4::right, float4::forward, float4::down, float4::positiveW};
			quat quaternion{matrix};

			CHECK(quat_absolute_equal(quaternion, quat::rotate_x_90));
		}
		{
			const float3x3 matrix{float3::right, float3::forward, float3::down};
			quat quaternion{matrix};

			CHECK(quat_absolute_equal(quaternion, quat::rotate_x_90));
		}
		{
			const float4x4 matrix{
				float4::right, normalize(float4::up + float4::forward), normalize(float4::forward + float4::down),
				float4::positiveW
			};
			const quat quaternion{matrix};

			CHECK(quat_absolute_equal(quaternion, quat::rotate_x_45));
		}
		{
			const float3x3 matrix{
				float3::right * 5.2125f, (float3::up + float3::forward) * 4.4647f,
				(float3::forward + float3::down) * 3.21654f
			};
			const quat quaternion{matrix};

			CHECK(quat_absolute_equal(quaternion, quat::rotate_x_45));
		}
		{
			const float4x4 matrix{
				float4::right * 0.52165f, (float4::up + float4::forward) * 0.154668f,
				(float4::forward + float4::down) * 11.7567f, float4{4868.f, 1543.125f, 124.155f, 1.f}
			};
			const quat quaternion{matrix};

			CHECK(quat_absolute_equal(quaternion, quat::rotate_x_45));
		}
		{
			const float3x3 matrix{
				float3::right, normalize(float3::up + float3::forward), normalize(float3::forward + float3::down)
			};
			const quat quaternion{matrix};

			CHECK(quat_absolute_equal(quaternion, quat::rotate_x_45));
		}
		{
			constexpr float3 axis(1.f, 2.f, 3.f);
			constexpr radians32 angle{2.329f};
			const quat expectedRotation = quat::angle_axis(angle, axis);

			REQUIRE(close_enough(length(expectedRotation), 1.f));

			float4x4 matrix = float4x4(expectedRotation);
			quat orientation = quat(matrix);

			CHECK(close_enough(length(orientation), 1.f));
			CHECK(close_enough(length(orientation), 1.f));
			CHECK(quat_absolute_equal(orientation, expectedRotation));
		}
	}

	SECTION("matrix look_at")
	{
		{
			const float4x4 matrix = look_at(float3::forward * 3.f, float3::zero, float3::up);

			CHECK(matrix[0] == float4::left);
			CHECK(matrix[1] == float4::up);
			CHECK(matrix[2] == float4::backward);
			CHECK(matrix[3] == float4(0.f, 0.f, 3.f, 1.f));
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
			const quat a = quat::angle_axis(radians32::deg90, float3::right);
			const quat b = quat::angle_axis(radians32::deg90, float3::up) *
						   quat::angle_axis(radians32::deg90, float3::forward) *
						   quat::angle_axis(-radians32::deg90, float3::up);
			CHECK(quat_absolute_equal(a, b));
		}
		{
			const quat quaternion = quat::angle_axis(radians32::deg45, float3::right);
			CHECK(quat_absolute_equal(quaternion, quat::rotate_x_45));
		}
		{
			const quat quaternion = quat::angle_axis(radians32::deg45, float3::up);
			CHECK(quat_absolute_equal(quaternion, quat::rotate_y_45));
		}
		{
			const quat quaternion = quat::angle_axis(radians32::deg45, float3::forward);
			CHECK(quat_absolute_equal(quaternion, quat::rotate_z_45));
		}
	}

	SECTION("quaternion matrix conversion")
	{
		{
			constexpr quat quaternion = identity;
			constexpr float4x4 matrix{quaternion};

			CHECK(matrix[0] == float4::right);
			CHECK(matrix[1] == float4::up);
			CHECK(matrix[2] == float4::forward);
			CHECK(matrix[3] == float4::positiveW);
		}
		{
			constexpr quat quaternion = identity;
			constexpr float3x3 matrix{quaternion};

			CHECK(matrix[0] == float3::right);
			CHECK(matrix[1] == float3::up);
			CHECK(matrix[2] == float3::forward);
		}
		{
			constexpr quat quaternion = rotate_x_90;
			constexpr float4x4 matrix{quaternion};

			CHECK(matrix[0] == float4::right);
			CHECK(matrix[1] == float4::forward);
			CHECK(matrix[2] == float4::down);
			CHECK(matrix[3] == float4::positiveW);
		}
		{
			constexpr quat quaternion = rotate_x_90;
			constexpr float3x3 matrix{quaternion};

			CHECK(matrix[0] == float3::right);
			CHECK(matrix[1] == float3::forward);
			CHECK(matrix[2] == float3::down);
		}
		{
			constexpr quat quaternion = rotate_y_90;
			constexpr float4x4 matrix{quaternion};

			CHECK(matrix[0] == float4::backward);
			CHECK(matrix[1] == float4::up);
			CHECK(matrix[2] == float4::right);
			CHECK(matrix[3] == float4::positiveW);
		}
		{
			constexpr quat quaternion = rotate_y_90;
			constexpr float3x3 matrix{quaternion};

			CHECK(matrix[0] == float3::backward);
			CHECK(matrix[1] == float3::up);
			CHECK(matrix[2] == float3::right);
		}
		{
			constexpr quat quaternion = rotate_z_90;
			constexpr float4x4 matrix{quaternion};

			CHECK(matrix[0] == float4::up);
			CHECK(matrix[1] == float4::left);
			CHECK(matrix[2] == float4::forward);
			CHECK(matrix[3] == float4::positiveW);
		}
		{
			constexpr quat quaternion = rotate_z_90;
			constexpr float3x3 matrix{quaternion};

			CHECK(matrix[0] == float3::up);
			CHECK(matrix[1] == float3::left);
			CHECK(matrix[2] == float3::forward);
		}

		{
			constexpr quat quaternion = rotate_x_45;
			constexpr float4x4 matrix{quaternion};

			CHECK(matrix[0] == float4::right);
			CHECK(matrix[1] == normalize(float4::up + float4::forward));
			CHECK(matrix[2] == normalize(float4::forward + float4::down));
			CHECK(matrix[3] == float4::positiveW);
		}
		{
			constexpr quat quaternion = rotate_x_45;
			constexpr float3x3 matrix{quaternion};

			CHECK(matrix[0] == float3::right);
			CHECK(matrix[1] == normalize(float3::up + float3::forward));
			CHECK(matrix[2] == normalize(float3::forward + float3::down));
		}
		{
			constexpr quat quaternion = rotate_y_45;
			constexpr float4x4 matrix{quaternion};

			CHECK(matrix[0] == normalize(float4::right + float4::backward));
			CHECK(matrix[1] == float4::up);
			CHECK(matrix[2] == normalize(float4::forward + float4::right));
			CHECK(matrix[3] == float4::positiveW);
		}
		{
			constexpr quat quaternion = rotate_y_45;
			constexpr float3x3 matrix{quaternion};

			CHECK(matrix[0] == normalize(float3::right + float3::backward));
			CHECK(matrix[1] == float3::up);
			CHECK(matrix[2] == normalize(float3::forward + float3::right));
		}
		{
			constexpr quat quaternion = rotate_z_45;
			constexpr float4x4 matrix{quaternion};

			CHECK(matrix[0] == normalize(float4::right + float4::up));
			CHECK(matrix[1] == normalize(float4::up + float4::left));
			CHECK(matrix[2] == float4::forward);
			CHECK(matrix[3] == float4::positiveW);
		}
		{
			constexpr quat quaternion = rotate_z_45;
			constexpr float3x3 matrix{quaternion};

			CHECK(matrix[0] == normalize(float3::right + float3::up));
			CHECK(matrix[1] == normalize(float3::up + float3::left));
			CHECK(matrix[2] == float3::forward);
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
