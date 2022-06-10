# Pizzazz

Add some color, style, advanced cursor movements, autocomplete suggestions, and more to your C++ terminal app!

This is a fully cross-platform library. See common.h, str.h, and TextField.h for the list of functions and their descriptions. Take a look at the test files for examples of how to use them. C++17 or newer is required; in Visual Studio, you can choose the version of C++ with project > Properties > C/C++ > Language > C++ Language Standard.

Here's a simple example of one of this library's functions, getline_ac, which can give autocomplete suggestions (not autocorrect) and has optional built-in input validation:

```cpp
#include "pizzazz.h"
using namespace std;
namespace paz = pizzazz;  // shorter alias for easier use

int main() {
    cout << "\n  Sample menu:\n"
        "   * Create\n"
        "   * Read\n"
        "   * Update\n"
        "   * Delete\n"
        "  > ";
    string choice = paz::getline_ac(
        { "Create", "Read", "Update", "Delete" },
        "type an option");
    cout << "\n  You chose " << choice << endl;
    return 0;
}
```

![](https://media.giphy.com/media/Rqoco5DR2a2AjDAqtX/giphy.gif)
