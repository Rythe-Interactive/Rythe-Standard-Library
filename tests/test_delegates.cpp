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
	rsl::size_type counter;

	void func()
	{
		counter++;
	}

	void func2(rsl::uint32& i)
	{
		counter += i;
		i++;
	}

	struct Object
	{
		std::vector<rsl::size_type> ints;

		void memberFunc()
		{
			for (rsl::size_type& i : ints)
			{
				i++;
				counter += i;
			}
		}

		void constMemberFunc() const
		{
			for (rsl::size_type i : ints)
			{
				counter += i;
			}
		}
	};

	std::vector<rsl::size_type> createVec(rsl::size_type idx)
	{
		std::vector<rsl::size_type> vec;
		for (rsl::size_type i = idx + 20; i > idx; i--)
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

	template <typename delegate_type, typename delegate_type2>
	void test_delegate_type()
	{
		delegate_type2 del2;

		counter = 0;

		{
			del2 = &func2;
		}

		REQUIRE(counter == 0);

		{
			rsl::uint32 v = 3u;
			del2(v);
			REQUIRE(counter == 3);
			REQUIRE(v == 4);
		}

		counter = 0;

		delegate_type del;

		{
			del = &func;
		}

		REQUIRE(counter == 0);

		del();

		REQUIRE(counter == 1);

		counter = 0;

		{
			del = []() { counter++; };
		}

		REQUIRE(counter == 0);

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

		REQUIRE(counter == 0);

		del();

		REQUIRE(counter == 2230);

		counter = 0;

		{
			Object obj{createVec(100)};
			del.template assign<Object, &Object::memberFunc>(obj);

			REQUIRE(counter == 0);

			del();

			REQUIRE(counter == 2230);
		}

		counter = 0;

		{
			const Object obj{createVec(100)};
			del.template assign<Object, &Object::constMemberFunc>(obj);

			REQUIRE(counter == 0);

			del();

			REQUIRE(counter == 2210);

			del();

			REQUIRE(counter == 4420);
		}

		counter = 0;
	}

    #define EXECUTE_SHARED_DELEGATE_TESTS(delegateType)                                                                    \
	test_delegate_type<delegateType<void()>, delegateType<void(rsl::uint32&)>>()

} // namespace

TEST_CASE("delegate", "[delegates]")
{
	EXECUTE_SHARED_DELEGATE_TESTS(rsl::delegate);
}

TEST_CASE("multicast_delegate", "[delegates]")
{
	EXECUTE_SHARED_DELEGATE_TESTS(rsl::multicast_delegate);
}
