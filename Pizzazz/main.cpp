#include "pizzazz.h"
#include "tests.h"
#include <iostream>
using namespace std;
namespace paz = pizzazz;  // shorter alias for easier use
using paz::Style;
using paz::CursorStyle;
using paz::Coord;

int main() {
	run_tests();
	cout << endl;
	return 0;
}
