#include "pizzazz.h"
#include "tui_tests.h"
using namespace std;
namespace paz = pizzazz;  // shorter alias for easier use
using paz::Style;
using paz::CursorStyle;
using paz::Coord;

int main() {
	run_tui_tests();
	cout << endl;
	return 0;
}
