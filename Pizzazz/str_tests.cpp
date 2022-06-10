#include "str_tests.h"
#include <iostream>
#include <stdexcept>
using namespace std;
namespace paz = pizzazz;

void run_str_tests()
{
	test_lstrip();
	test_rstrip();
	test_strip();
	test_split();
}

void test_lstrip()
{
	if (paz::lstrip("abc", "baceabca") != "eabca")
		throw runtime_error("lstrip");
}

void test_rstrip()
{
	if (paz::rstrip("abc", "baceabca") != "bace")
		throw runtime_error("rstrip");
}

void test_strip()
{
	if (paz::strip("abc", "baceabca") != "e")
		throw runtime_error("strip");
}

void test_split()
{
	vector<string> v = paz::split("1.23.456.", ".");
	if (v[0] != "1")
		throw runtime_error("v[0]");
	if (v[1] != "23")
		throw runtime_error("v[1]");
	if (v[2] != "456")
		throw runtime_error("v[2]");

	v = paz::split("a b c");
	if (v[0] != "a")
		throw runtime_error("v[0]");

	cout << "\npaz::split(\"a b c\") -> ";
	for (string e : paz::split("a b c"))
		cout << e << " ";
	cout << "\npaz::split(\"sldkfj\", \"3\") -> ";
	for (string e : paz::split("sldkfj", "3"))
		cout << e << " ";
	cout << endl;
}
