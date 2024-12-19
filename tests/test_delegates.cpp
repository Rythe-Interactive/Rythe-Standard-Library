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

	void func2()
	{
		counter *= 2;
	}

	void funcParam(rsl::uint32& i)
	{
		counter += i;
		i++;
	}

	void funcParam2(rsl::uint32& i)
	{
		counter += i;
		i *= 2;
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

		void memberFunc2()
		{
			for (rsl::size_type& i : ints)
			{
				i += 2;
			}
		}

		void constMemberFunc() const
		{
			for (rsl::size_type i : ints)
			{
				counter += i;
			}
		}

		void constMemberFunc2() const
		{
			for (rsl::size_type i : ints)
			{
				counter += i - 2;
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

		REQUIRE(del2.empty());
		REQUIRE(del2 == nullptr);

		counter = 0;

		{
			del2 = &funcParam;
		}

		REQUIRE(!del2.empty());
		REQUIRE(del2 != nullptr);
		REQUIRE(counter == 0);

		{
			rsl::uint32 v = 3u;
			del2(v);
			REQUIRE(counter == 3);
			REQUIRE(v == 4);
		}

		counter = 0;

		delegate_type del;

		REQUIRE(del.empty());
		REQUIRE(del == nullptr);

		{
			del = &func;
		}

		REQUIRE(!del.empty());
		REQUIRE(del != nullptr);
		REQUIRE(counter == 0);

		del();

		REQUIRE(counter == 1);

		del.clear();
		REQUIRE(del.empty());
		REQUIRE(del == nullptr);

		counter = 0;

		{
			del = []() { counter++; };
		}

		REQUIRE(!del.empty());
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
	SECTION("Shared delegate tests")
	{
		EXECUTE_SHARED_DELEGATE_TESTS(rsl::multicast_delegate);
	}

	SECTION("Multicast delegate specific tests")
	{

		rsl::multicast_delegate<void(rsl::uint32&)> del2;

		REQUIRE(del2.empty());
		REQUIRE(del2 == nullptr);
		REQUIRE(del2.size() == 0);

		counter = 0;

		{
			del2 += &funcParam;
		}

		REQUIRE(del2.size() == 1);

		{
			del2 += &funcParam2;
		}

		REQUIRE(del2.size() == 2);

		REQUIRE(!del2.empty());
		REQUIRE(del2 != nullptr);
		REQUIRE(counter == 0);

		{
			rsl::uint32 v = 3u;
			del2(v);
			REQUIRE(counter == 7);
			REQUIRE(v == 8);
			del2(v);
			REQUIRE(counter == 24);
			REQUIRE(v == 18);
			del2(v);
			REQUIRE(counter == 61);
			REQUIRE(v == 38);
		}

		del2.remove(funcParam);
		REQUIRE(!del2.empty());
		REQUIRE(del2 != nullptr);
		REQUIRE(del2.size() == 1);

		del2.remove(funcParam);
		REQUIRE(!del2.empty());
		REQUIRE(del2 != nullptr);
		REQUIRE(del2.size() == 1);

		del2 -= funcParam2;
		REQUIRE(del2.empty());
		REQUIRE(del2 == nullptr);
		REQUIRE(del2.size() == 0);

		counter = 0;

		rsl::multicast_delegate<void()> del;

		{
			del += &func;
		}
		{
			del += &func2;
		}

		REQUIRE(counter == 0);

		del();
		REQUIRE(counter == 2);
		del();
		REQUIRE(counter == 6);
		del();
		REQUIRE(counter == 14);

		del.clear();
		REQUIRE(del.empty());
		REQUIRE(del == nullptr);
		REQUIRE(del.size() == 0);

		counter = 0;

		{
			del = []() { counter++; };
		}
		{
			auto lambda = []() { counter *= 3; };
			del += lambda;

			REQUIRE(!del.empty());
			REQUIRE(del != nullptr);
			REQUIRE(del.size() == 2);

			REQUIRE(counter == 0);

			del();
			REQUIRE(counter == 3);
			del();
			REQUIRE(counter == 12);
			del();
			REQUIRE(counter == 39);

			del -= lambda;

			REQUIRE(!del.empty());
			REQUIRE(del != nullptr);
			REQUIRE(del.size() == 1);
			REQUIRE(!del.contains(lambda));
		}

		del.clear();

		REQUIRE(del.empty());
		REQUIRE(del == nullptr);
		REQUIRE(del.size() == 0);

		counter = 0;

		{
			std::vector<size_t> ints = createVec(100);
			del = [&ints]()
			{
				for (size_t i : ints)
				{
					counter += i;
				}
			};
			del += [&ints]()
			{
				for (size_t& i : ints)
				{
					i++;
				}
			};

			REQUIRE(counter == 0);

			del();
			REQUIRE(counter == 2210);
			del();
			REQUIRE(counter == 4440);
			del();
			REQUIRE(counter == 6690);
		}

		counter = 0;

		{
			Object obj{createVec(100)};
			del.assign<Object, &Object::memberFunc>(obj);
			del.push_back<Object, &Object::memberFunc2>(obj);

			REQUIRE(counter == 0);

			del();
			REQUIRE(counter == 2230);
			del();
			REQUIRE(counter == 4520);
			del();
			REQUIRE(counter == 6870);
			del();
			REQUIRE(counter == 9280);

			del.pop_back();

			REQUIRE(!del.empty());
			REQUIRE(del != nullptr);
			REQUIRE(del.size() == 1);
			REQUIRE(del.contains<Object, &Object::memberFunc>(obj));
			REQUIRE(!del.contains<Object, &Object::memberFunc2>(obj));

			del.pop_back();

			REQUIRE(del.empty());
			REQUIRE(del == nullptr);
			REQUIRE(del.size() == 0);
			REQUIRE(!del.contains<Object, &Object::memberFunc>(obj));
			REQUIRE(!del.contains<Object, &Object::memberFunc2>(obj));
		}

		counter = 0;

		{
			const Object obj{createVec(100)};
			del.assign<Object, &Object::constMemberFunc>(obj);
			del.push_back<Object, &Object::constMemberFunc2>(obj);

			REQUIRE(counter == 0);

			del();
			REQUIRE(counter == 4380);
			del();
			REQUIRE(counter == 8760);
		}

		counter = 0;
	}
}
