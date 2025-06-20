#define RYTHE_VALIDATE

#include <rsl/heap_allocator>

namespace
{
	class test_heap_allocator : private rsl::heap_allocator
	{
	public:
		using value_type = void;
		rsl::id_type id = 1012234;

		using rsl::heap_allocator::heap_allocator;
		explicit constexpr test_heap_allocator(rsl::id_type _id) noexcept
			: id(_id)
		{
		}

		using rsl::heap_allocator::allocate;
		using rsl::heap_allocator::deallocate;
		using rsl::heap_allocator::reallocate;
		using rsl::heap_allocator::is_valid;
	};
} // namespace

#define RSL_DEFAULT_ALLOCATOR_OVERRIDE test_heap_allocator

#include <rsl/map>

#include <catch2/catch_test_macros.hpp>

namespace
{
	constexpr int const1 = 123456;
	// constexpr int const2 = 234567;
	constexpr int const3 = 345678;
	constexpr int const4 = 456789;
	constexpr int const5 = 567891;
	constexpr int const6 = 678912;

	constexpr float key1 = 1.23456f;
	constexpr float key2 = 2.34567f;
	constexpr float key3 = 3.45678f;
	constexpr float key4 = 4.56789f;
	// constexpr float key5 = 5.67891f;
	// constexpr float key6 = 6.78912f;

	struct test_struct
	{
		int value = 0;
		test_struct() = default;
		test_struct(const int i)
			: value(i)
		{
		}
	};
} // namespace

TEST_CASE("dynamic_map", "[containers]")
{
	using namespace rsl;

	SECTION("construction")
	{
		{
			rsl::dynamic_map<float32, test_struct> map{};
			REQUIRE(map.get_allocator().id == 1012234);
		}
		{
			test_heap_allocator alloc{1234};
			rsl::dynamic_map<float32, test_struct> map{alloc};
			REQUIRE(map.get_allocator().id == 1234);
		}
		{
			default_pmu_allocator alloc;
			allocator_storage<polymorphic_allocator> store(&alloc);
			rsl::dynamic_map<float32, test_struct, hash_map_flags::defaultFlags, polymorphic_allocator> map{store};
			REQUIRE((&(map.get_allocator())) == &alloc);
		}
	}

	SECTION("emplace")
	{
		rsl::dynamic_map<float32, test_struct> map{};

		{
			auto [result, emplaced] = map.try_emplace(key1, const1);
			REQUIRE(result.value == const1);
			REQUIRE(emplaced);
		}

		REQUIRE(map.contains(key1));
		REQUIRE(!map.contains(key2));
		REQUIRE(!map.contains(key3));
		REQUIRE(!map.contains(key4));

		{
			auto [result, emplaced] = map.try_emplace(key1, const1);
			REQUIRE(result.value == const1);
			REQUIRE(!emplaced);
		}

		REQUIRE(map.contains(key1));
		REQUIRE(!map.contains(key2));
		REQUIRE(!map.contains(key3));
		REQUIRE(!map.contains(key4));

		{
			auto [result, emplaced] = map.try_emplace(key2, const6);
			REQUIRE(result.value == const6);
			REQUIRE(emplaced);
		}

		REQUIRE(map.contains(key1));
		REQUIRE(map.contains(key2));
		REQUIRE(!map.contains(key3));
		REQUIRE(!map.contains(key4));

		{
			auto [result, emplaced] = map.try_emplace(key2, const5);
			REQUIRE(result.value == const6);
			REQUIRE(!emplaced);
		}

		REQUIRE(map.contains(key1));
		REQUIRE(map.contains(key2));
		REQUIRE(!map.contains(key3));
		REQUIRE(!map.contains(key4));

		{
			test_struct& result = map.emplace(key3, const3);
			REQUIRE(result.value == const3);
		}

		REQUIRE(map.contains(key1));
		REQUIRE(map.contains(key2));
		REQUIRE(map.contains(key3));
		REQUIRE(!map.contains(key4));

		{
			test_struct& result = map.emplace(key3, const4);
			REQUIRE(result.value == const3);
		}

		REQUIRE(map.contains(key1));
		REQUIRE(map.contains(key2));
		REQUIRE(map.contains(key3));
		REQUIRE(!map.contains(key4));

		{
			test_struct& result = map.emplace_or_replace(key3, const4);
			REQUIRE(result.value == const4);
		}

		REQUIRE(map.contains(key1));
		REQUIRE(map.contains(key2));
		REQUIRE(map.contains(key3));
		REQUIRE(!map.contains(key4));

		{
			test_struct& result = map.emplace(key4, const5);
			REQUIRE(result.value == const5);
		}

		REQUIRE(map.contains(key1));
		REQUIRE(map.contains(key2));
		REQUIRE(map.contains(key3));
		REQUIRE(map.contains(key4));

		map.clear();

		REQUIRE(!map.contains(key1));
		REQUIRE(!map.contains(key2));
		REQUIRE(!map.contains(key3));
		REQUIRE(!map.contains(key4));
	}

	SECTION("get") {}
	SECTION("erase") {}
	SECTION("get_or_emplace") {}
	SECTION("emplace_or_replace") {}
}
