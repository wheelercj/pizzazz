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

	};
}
