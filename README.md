# ynot

Add some color, style, advanced cursor movements, autocomplete suggestions, and more to your C++ terminal app! This fully cross-platform library also includes various functions and classes to make some common challenges easier.

See the header files for the lists of functions and their descriptions. Take a look at the test files for examples of how to use them. C++17 or newer is required; in Visual Studio, you can choose the version of C++ with project > Properties > C/C++ > Language > C++ Language Standard.

Here's a simple example of one of this library's functions, getline_ac, which can give autocomplete suggestions (not autocorrect) and has optional built-in input validation:

```cpp
#include "ynot.h"
using namespace std;

int main() {
    cout << "\n  Sample menu:\n"
        "   * Create\n"
        "   * Read\n"
        "   * Update\n"
        "   * Delete\n"
        "  > ";
    string choice = ynot::getline_ac(
        { "Create", "Read", "Update", "Delete" },
        "type an option");
    cout << "\n  You chose " << choice << endl;
    return 0;
}
```

![](https://media.giphy.com/media/Rqoco5DR2a2AjDAqtX/giphy.gif)

Below is another example. With ynot's `Paginator` class, you can cleanly present long pieces of text in a terminal.

```cpp
#include "ynot.h"
using namespace std;

int main() {
	string article_title = "3.6 Git Branching - Rebasing";
	string article_body = ynot::dedent(R"(
		Article body here.
		Indent with tabs or spaces, not both.)");
	string line_prefix = "\n    ";
	ynot::Paginator paginator(article_title, article_body, line_prefix);
	paginator.run();
}
```

![](https://media.giphy.com/media/tAn8Pis7lLUfA39MFa/giphy.gif)

## download

You can download this library into your project with git by using `git submodule add https://github.com/wheelercj/ynot` and then adding `#include "../ynot/ynot/ynot.h"` to the file you want to use this library in.

Alternatively, you can manually download a zip file or a tar.gz file of this library at https://github.com/wheelercj/ynot/tags. The library's version number is saved in ynot.h so that you can check whether you have the latest version in the future.
