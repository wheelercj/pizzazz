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

		TEST_METHOD(test_dont_slice)
		{
			assert_equal("abcdefg", paz::slice("abcdefg", 0));
		}

		TEST_METHOD(test_slice)
		{
			assert_equal("defg", paz::slice("abcdefg", 3));
		}

		TEST_METHOD(test_slice_with_negative_start)
		{
			assert_equal("efg", paz::slice("abcdefg", -3));
		}

		TEST_METHOD(test_slice_with_negative_start_and_end)
		{
			assert_equal("e", paz::slice("abcdefg", -3, -2));
		}

		TEST_METHOD(test_slice_with_early_end)
		{
			assert_equal("abcd", paz::slice("abcdefg", 0, 4));
		}

		TEST_METHOD(test_slice_with_empty_return)
		{
			assert_equal("", paz::slice("abcdefg", -1, 4));
		}

		TEST_METHOD(test_slice_returning_one_character)
		{
			assert_equal("c", paz::slice("abcdefg", 2, 3));
		}

		TEST_METHOD(test_slice_returning_two_characters)
		{
			assert_equal("cd", paz::slice("abcdefg", 2, 4));
		}

		TEST_METHOD(test_slice_with_negative_end)
		{
			assert_equal("cde", paz::slice("abcdefg", 2, -2));
		}

		TEST_METHOD(test_slice_with_step_of_1)
		{
			assert_equal("abcdef", paz::slice("abcdefg", 0, 6, 1));
		}

		TEST_METHOD(test_slice_with_step_of_2)
		{
			assert_equal("ace", paz::slice("abcdefg", 0, 6, 2));
		}

		TEST_METHOD(test_slice_with_step_of_3)
		{
			assert_equal("ad", paz::slice("abcdefg", 0, 6, 3));
		}

		TEST_METHOD(test_slice_with_step_of_negative_1)
		{
			assert_equal("fedcba", paz::slice("abcdefg", 0, 6, -1));
		}

		TEST_METHOD(test_slice_with_step_of_negative_2)
		{
			assert_equal("eca", paz::slice("abcdefg", 0, 6, -2));
		}

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

		TEST_METHOD(test_split_with_no_matches)
		{
			vector<string> v = paz::split("sldkfj", "3");
			assert_equal("sldkfj", v[0]);
			assert_equal(1, v.size());
		}

		TEST_METHOD(test_split_with_empty_string)
		{
			auto func = [] {paz::split("sldkfj", ""); };
			Assert::ExpectException<invalid_argument>(func);
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

		TEST_METHOD(test_indent)
		{
			assert_equal("    first\n    second\n    third", paz::indent("first\nsecond\nthird"));
		}

		TEST_METHOD(test_indent_tabs)
		{
			assert_equal("\tfirst\n\tsecond\n\tthird", paz::indent("first\nsecond\nthird", "\t"));
		}

		TEST_METHOD(test_indent_bullet_points)
		{
			assert_equal("* first\n* second\n* third", paz::indent("first\nsecond\nthird", "* "));
		}

		TEST_METHOD(test_dedent)
		{
			assert_equal("first\nsecond\nthird", paz::dedent("   first\n   second\n   third"));
		}

		TEST_METHOD(test_dedent_with_tabs)
		{
			assert_equal("first\nsecond\nthird", paz::dedent("\tfirst\n\tsecond\n\tthird"));
		}

		TEST_METHOD(test_dedent_with_uneven_spaces)
		{
			assert_equal("first\n second\n  third", paz::dedent(" first\n  second\n   third"));
		}

		TEST_METHOD(test_dedent_with_uneven_tabs)
		{
			assert_equal("\tfirst\nsecond\n\t\tthird", paz::dedent("\t\tfirst\n\tsecond\n\t\t\tthird"));
		}

		TEST_METHOD(test_wrap)
		{
			assert_equal("a aa\naaa", paz::wrap("a aa aaa", 7));
		}

		TEST_METHOD(test_wrap_with_indentation)
		{
			assert_equal("  a aa\n  aaa", paz::wrap("a aa aaa", 7, "  "));
		}

		TEST_METHOD(test_dont_wrap)
		{
			assert_equal("a aa aaa", paz::wrap("a aa aaa", 30));
		}

		TEST_METHOD(test_wrap_with_word_that_is_too_long)
		{
			assert_equal("what if long word\nfloccinaucinihilipil\nification",
				paz::wrap("what if long word floccinaucinihilipilification", 20));
		}

		TEST_METHOD(test_wrap_with_invalid_width)
		{
			auto func = [] { paz::wrap("blah blah", 5, "more than 5 characters"); };
			Assert::ExpectException<std::invalid_argument>(func);
		}

		TEST_METHOD(test_find_next_space_at_start)
		{
			assert_equal(0, paz::find_next_space(" hi"));
		}

		TEST_METHOD(test_find_next_space)
		{
			assert_equal(1, paz::find_next_space("h i"));
		}

		TEST_METHOD(test_find_next_space_at_end)
		{
			assert_equal(2, paz::find_next_space("hi "));
		}

		TEST_METHOD(test_find_next_space_starting_in_middle)
		{
			assert_equal(9, paz::find_next_space("hey there ", 4));
		}

		TEST_METHOD(test_find_previous_space_at_end)
		{
			assert_equal(2, paz::find_previous_space("hi "));
		}

		TEST_METHOD(test_find_previous_space)
		{
			assert_equal(1, paz::find_previous_space("h i"));
		}

		TEST_METHOD(test_find_previous_space_at_start)
		{
			assert_equal(0, paz::find_previous_space(" hi"));
		}

		TEST_METHOD(test_find_previous_space_starting_in_middle)
		{
			assert_equal(0, paz::find_previous_space(" there hey", 5));
		}
	};
}
