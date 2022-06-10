#include "pch.h"
#include "CppUnitTest.h"
#include "../Pizzazz/str.h"
#include "../Pizzazz/str.cpp"
#include <vector>
using namespace std;
namespace paz = pizzazz;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{
	void assert_equal(std::string str1, std::string str2)
	{
		Assert::AreEqual(str1, str2);
	}

	void assert_equal(size_t num1, size_t num2)
	{
		Assert::AreEqual(num1, num2);
	}

	TEST_CLASS(tests)
	{
	public:
		
		TEST_METHOD(test_split_with_default_argument)
		{
			vector<string> v = paz::split("a b c");
			assert_equal("a", v[0]);
			assert_equal("b", v[1]);
			assert_equal("c", v[2]);
			assert_equal(3, v.size());
		}

		TEST_METHOD(test_split_with_trailing_split)
		{
			vector<string> v = paz::split("1.23.456.", ".");
			assert_equal("1", v[0]);
			assert_equal("23", v[1]);
			assert_equal("456", v[2]);
			assert_equal("", v[3]);
			assert_equal(4, v.size());
		}

		TEST_METHOD(test_split_with_leading_split)
		{
			vector<string> v = paz::split("\nline\nanother line", "\n");
			assert_equal("", v[0]);
			assert_equal("line", v[1]);
			assert_equal("another line", v[2]);
			assert_equal(3, v.size());
		}

		TEST_METHOD(test_split_with_no_split)
		{
			vector<string> v = paz::split("sldkfj", "3");
			assert_equal("sldkfj", v[0]);
			assert_equal(1, v.size());
		}

		TEST_METHOD(test_split_by_multiple_characters)
		{
			vector<string> v = paz::split("123asdfkjs123alkj123", "123");
			assert_equal("", v[0]);
			assert_equal("asdfkjs", v[1]);
			assert_equal("alkj", v[2]);
			assert_equal("", v[3]);
			assert_equal(4, v.size());
		}

		TEST_METHOD(test_join)
		{
			assert_equal("1, 2, 3", paz::join({ "1", "2", "3" }, ", "));
		}

		TEST_METHOD(test_join_on_empty_string)
		{
			assert_equal("123", paz::join({ "1", "2", "3" }));
		}

		TEST_METHOD(test_strip)
		{
			assert_equal("e", paz::strip("abc", "baceabca"));
		}

		TEST_METHOD(test_lstrip)
		{
			assert_equal("eabca", paz::lstrip("abc", "baceabca"));
		}

		TEST_METHOD(test_rstrip)
		{
			assert_equal("bace", paz::rstrip("abc", "baceabca"));
		}

		TEST_METHOD(test_startswith)
		{
			assert_equal(true, paz::startswith("abc", "a"));
		}

		TEST_METHOD(test_startswith_multiple_characters)
		{
			assert_equal(true, paz::startswith("abc", "ab"));
		}

		TEST_METHOD(test_not_startswith)
		{
			assert_equal(false, paz::startswith("abc", "d"));
		}

		TEST_METHOD(test_endswith)
		{
			assert_equal(true, paz::endswith("abc", "c"));
		}

		TEST_METHOD(test_endswith_multiple_characters)
		{
			assert_equal(true, paz::endswith("abc", "bc"));
		}

		TEST_METHOD(test_not_endswith)
		{
			assert_equal(false, paz::endswith("abc", "d"));
		}

	};
}
