#include <catch2/catch_test_macros.hpp>

#define RYTHE_VALIDATE

#include <rsl/containers>
#include <rsl/delegate>
#include <rsl/primitives>

#include <any>
#include <chrono>
#include <iostream>

namespace
{
	size_t counter;

	void func()
	{
		counter++;
	}

	struct Object
	{
		std::vector<size_t> ints;

		void memberFunc()
		{
			for (size_t& i : ints)
			{
				i++;
				counter += i;
			}
		}

		void constMemberFunc() const
		{

			for (size_t i : ints)
			{
				counter += i;
			}
		}
	};

	std::vector<size_t> createVec(size_t idx)
	{
		std::vector<size_t> vec;
		for (size_t i = idx + 20; i > idx; i--)
		{
			vec.push_back(i);
		}
		return vec;
	}

	struct testComp
	{
		std::string content;
		const char* printout() { return content.c_str(); }
	};
} // namespace

TEST_CASE("multicast_delegate", "[delegates]")
{
	rsl::multicast_delegate<void()> del;

	{
		del = &func;
	}

	del();

	REQUIRE(counter == 1);

	counter = 0;

	{
		del = []() { counter++; };
	}

	del();

	REQUIRE(counter == 1);

	counter = 0;

	{
		std::vector<size_t> ints = createVec(100);
		del = [ints]()
		{
			for (size_t i : ints)
			{
				i++;
				counter += i;
			}
		};
	}

	del();

	REQUIRE(counter == 2230);

	counter = 0;

	{
		Object obj{createVec(100)};
		{
			del.assign<Object, &Object::memberFunc>(obj);
		}

		del();

		REQUIRE(counter == 2230);

		counter = 0;

		del.assign<Object, &Object::constMemberFunc>(obj);

		del();

		REQUIRE(counter == 2230);
	}
}
