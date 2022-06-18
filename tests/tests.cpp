#include "pch.h"
#include "CppUnitTest.h"
#include "../ynot/str.h"
#include "../ynot/str.cpp"
#include <vector>
using namespace std;
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
		
		TEST_METHOD(test_to_lower)
		{
			assert_equal("abcdefg", ynot::to_lower("AbCdEfG"));
		}

		TEST_METHOD(test_to_upper)
		{
			assert_equal("ABCDEFG", ynot::to_upper("AbCdEfG"));
		}

		TEST_METHOD(test_dont_slice)
		{
			assert_equal("abcdefg", ynot::slice("abcdefg", 0));
		}

		TEST_METHOD(test_slice)
		{
			assert_equal("defg", ynot::slice("abcdefg", 3));
		}

		TEST_METHOD(test_slice_with_negative_start)
		{
			assert_equal("efg", ynot::slice("abcdefg", -3));
		}

		TEST_METHOD(test_slice_with_negative_start_and_end)
		{
			assert_equal("e", ynot::slice("abcdefg", -3, -2));
		}

		TEST_METHOD(test_slice_with_early_end)
		{
			assert_equal("abcd", ynot::slice("abcdefg", 0, 4));
		}

		TEST_METHOD(test_slice_with_empty_return)
		{
			assert_equal("", ynot::slice("abcdefg", -1, 4));
		}

		TEST_METHOD(test_slice_returning_one_character)
		{
			assert_equal("c", ynot::slice("abcdefg", 2, 3));
		}

		TEST_METHOD(test_slice_returning_two_characters)
		{
			assert_equal("cd", ynot::slice("abcdefg", 2, 4));
		}

		TEST_METHOD(test_slice_with_negative_end)
		{
			assert_equal("cde", ynot::slice("abcdefg", 2, -2));
		}

		TEST_METHOD(test_slice_with_step_of_1)
		{
			assert_equal("abcdef", ynot::slice("abcdefg", 0, 6, 1));
		}

		TEST_METHOD(test_slice_with_step_of_2)
		{
			assert_equal("ace", ynot::slice("abcdefg", 0, 6, 2));
		}

		TEST_METHOD(test_slice_with_step_of_3)
		{
			assert_equal("ad", ynot::slice("abcdefg", 0, 6, 3));
		}

		TEST_METHOD(test_slice_with_step_of_negative_1)
		{
			assert_equal("fedcba", ynot::slice("abcdefg", 0, 6, -1));
		}

		TEST_METHOD(test_slice_with_step_of_negative_2)
		{
			assert_equal("eca", ynot::slice("abcdefg", 0, 6, -2));
		}

		TEST_METHOD(test_split_with_default_argument)
		{
			vector<string> v = ynot::split("a b c");
			assert_equal("a", v[0]);
			assert_equal("b", v[1]);
			assert_equal("c", v[2]);
			assert_equal(3, v.size());
		}

		TEST_METHOD(test_split_with_trailing_split)
		{
			vector<string> v = ynot::split("1.23.456.", ".");
			assert_equal("1", v[0]);
			assert_equal("23", v[1]);
			assert_equal("456", v[2]);
			assert_equal("", v[3]);
			assert_equal(4, v.size());
		}

		TEST_METHOD(test_split_with_leading_split)
		{
			vector<string> v = ynot::split("\nline\nanother line", "\n");
			assert_equal("", v[0]);
			assert_equal("line", v[1]);
			assert_equal("another line", v[2]);
			assert_equal(3, v.size());
		}

		TEST_METHOD(test_dont_split)
		{
			vector<string> v = ynot::split("sldkfj", "3");
			assert_equal("sldkfj", v[0]);
			assert_equal(1, v.size());
		}

		TEST_METHOD(test_split_by_empty_string)
		{
			auto func = [] {ynot::split("sldkfj", ""); };
			Assert::ExpectException<invalid_argument>(func);
		}

		TEST_METHOD(test_split_by_multiple_characters)
		{
			vector<string> v = ynot::split("123asdfkjs123alkj123", "123");
			assert_equal("", v[0]);
			assert_equal("asdfkjs", v[1]);
			assert_equal("alkj", v[2]);
			assert_equal("", v[3]);
			assert_equal(4, v.size());
		}

		TEST_METHOD(test_join)
		{
			assert_equal("1, 2, 3", ynot::join({ "1", "2", "3" }, ", "));
		}

		TEST_METHOD(test_join_on_empty_string)
		{
			assert_equal("123", ynot::join({ "1", "2", "3" }));
		}

		TEST_METHOD(test_strip)
		{
			assert_equal("e", ynot::strip("abc", "baceabca"));
		}

		TEST_METHOD(test_lstrip)
		{
			assert_equal("eabca", ynot::lstrip("abc", "baceabca"));
		}

		TEST_METHOD(test_rstrip)
		{
			assert_equal("bace", ynot::rstrip("abc", "baceabca"));
		}

		TEST_METHOD(test_center)
		{
			assert_equal("  hi  ", ynot::center("hi", 6));
		}

		TEST_METHOD(test_center_multiple_times)
		{
			std::string str = ynot::center("hi", 6);
			str = ynot::center(str, 8, '*');
			str = ynot::center(str, 10, '~');
			assert_equal("~*  hi  *~", str);
		}

		TEST_METHOD(test_center_with_width_that_prevents_perfect_centering)
		{
			assert_equal(" hi  ", ynot::center("hi", 5));
		}

		TEST_METHOD(test_center_with_small_width)
		{
			assert_equal("hello", ynot::center("hello", 3));
		}

		TEST_METHOD(test_ljust)
		{
			assert_equal("hi   ", ynot::ljust("hi", 5));
		}

		TEST_METHOD(test_ljust_with_stars)
		{
			assert_equal("hi***", ynot::ljust("hi", 5, '*'));
		}

		TEST_METHOD(test_ljust_with_small_width)
		{
			assert_equal("hello", ynot::ljust("hello", 3));
		}

		TEST_METHOD(test_rjust)
		{
			assert_equal("   hi", ynot::rjust("hi", 5));
		}

		TEST_METHOD(test_rjust_with_stars)
		{
			assert_equal("***hi", ynot::rjust("hi", 5, '*'));
		}

		TEST_METHOD(test_rjust_with_small_width)
		{
			assert_equal("hello", ynot::rjust("hello", 3));
		}

		TEST_METHOD(test_startswith)
		{
			assert_equal(true, ynot::startswith("abc", "a"));
		}

		TEST_METHOD(test_startswith_multiple_characters)
		{
			assert_equal(true, ynot::startswith("abc", "ab"));
		}

		TEST_METHOD(test_not_startswith)
		{
			assert_equal(false, ynot::startswith("abc", "d"));
		}

		TEST_METHOD(test_endswith)
		{
			assert_equal(true, ynot::endswith("abc", "c"));
		}

		TEST_METHOD(test_endswith_multiple_characters)
		{
			assert_equal(true, ynot::endswith("abc", "bc"));
		}

		TEST_METHOD(test_not_endswith)
		{
			assert_equal(false, ynot::endswith("abc", "d"));
		}

		TEST_METHOD(test_indent)
		{
			assert_equal("    first\n    second\n    third", ynot::indent("first\nsecond\nthird"));
		}

		TEST_METHOD(test_indent_with_tabs)
		{
			assert_equal("\tfirst\n\tsecond\n\tthird", ynot::indent("first\nsecond\nthird", "\t"));
		}

		TEST_METHOD(test_indent_with_bullet_points)
		{
			assert_equal("* first\n* second\n* third", ynot::indent("first\nsecond\nthird", "* "));
		}

		TEST_METHOD(test_dedent)
		{
			assert_equal("first\nsecond\nthird", ynot::dedent("   first\n   second\n   third"));
		}

		TEST_METHOD(test_dedent_with_tabs)
		{
			assert_equal("first\nsecond\nthird", ynot::dedent("\tfirst\n\tsecond\n\tthird"));
		}

		TEST_METHOD(test_dedent_with_uneven_spaces)
		{
			assert_equal("first\n second\n  third", ynot::dedent(" first\n  second\n   third"));
		}

		TEST_METHOD(test_dedent_with_uneven_tabs)
		{
			assert_equal("\tfirst\nsecond\n\t\tthird", ynot::dedent("\t\tfirst\n\tsecond\n\t\t\tthird"));
		}

		TEST_METHOD(test_wrap)
		{
			assert_equal("a aa\naaa", ynot::wrap("a aa aaa", 7));
		}

		TEST_METHOD(test_wrap_with_indentation)
		{
			assert_equal("  a aa\n  aaa", ynot::wrap("a aa aaa", 7, "  "));
		}

		TEST_METHOD(test_dont_wrap)
		{
			assert_equal("a aa aaa", ynot::wrap("a aa aaa", 30));
		}

		TEST_METHOD(test_wrap_with_word_that_is_too_long)
		{
			assert_equal("what if long word\nfloccinaucinihilipil\nification",
				ynot::wrap("what if long word floccinaucinihilipilification", 20));
		}

		TEST_METHOD(test_wrap_with_invalid_width)
		{
			auto func = [] { ynot::wrap("blah blah", 5, "more than 5 characters"); };
			Assert::ExpectException<std::invalid_argument>(func);
		}

		TEST_METHOD(test_contains)
		{
			assert_equal(true, ynot::contains("slkdjflsk\nslkdjflks", "\n"));
		}

		TEST_METHOD(test_contains_with_multiple_characters)
		{
			assert_equal(true, ynot::contains("abcdefghijklm", "ghi"));
		}

		TEST_METHOD(test_not_contains)
		{
			assert_equal(false, ynot::contains("asdfasdfasdf", "z"));
		}

		TEST_METHOD(test_not_contains_multiple_characters)
		{
			assert_equal(false, ynot::contains("asdfasdfsafd", "zxc"));
		}

		TEST_METHOD(test_contains_empty_string)
		{
			assert_equal(true, ynot::contains("asjdfklj", ""));
		}

		TEST_METHOD(test_find_next_space_at_start)
		{
			assert_equal(0, ynot::find_next_space(" hi"));
		}

		TEST_METHOD(test_find_next_space)
		{
			assert_equal(1, ynot::find_next_space("h i"));
		}

		TEST_METHOD(test_find_next_space_at_end)
		{
			assert_equal(2, ynot::find_next_space("hi "));
		}

		TEST_METHOD(test_find_next_space_starting_in_middle)
		{
			assert_equal(9, ynot::find_next_space("hey there ", 4));
		}

		TEST_METHOD(test_find_previous_space_at_end)
		{
			assert_equal(2, ynot::find_previous_space("hi "));
		}

		TEST_METHOD(test_find_previous_space)
		{
			assert_equal(1, ynot::find_previous_space("h i"));
		}

		TEST_METHOD(test_find_previous_space_at_start)
		{
			assert_equal(0, ynot::find_previous_space(" hi"));
		}

		TEST_METHOD(test_find_previous_space_starting_in_middle)
		{
			assert_equal(0, ynot::find_previous_space(" there hey", 5));
		}
	};
}
