# ynot

Add some color, style, advanced cursor movements, autocomplete suggestions, and more to your C++ terminal app! This fully cross-platform library also includes various functions and classes to make some common challenges easier.

## download

You can download this library into your project with git by using `git submodule add https://github.com/wheelercj/ynot` and then adding `#include "../ynot/ynot/ynot.h"` to the file you want to use this library in. Get future updates with `git submodule update --remote`.

Alternatively, you can manually download a zip file or a tar.gz file of this library at https://github.com/wheelercj/ynot/tags. The library's version number is saved in ynot.h so that you can check whether you have the latest version in the future.

## usage

See the header files for the lists of functions and their descriptions. Take a look at the test files or [the matrix](https://github.com/wheelercj/the-matrix) for examples of how to use them. C++20 or newer is required; in Visual Studio, you can choose the version of C++ with project > Properties > C/C++ > Language > C++ Language Standard.

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

![autocomplete menu example](https://media.giphy.com/media/Rqoco5DR2a2AjDAqtX/giphy.gif)

Below is another example. With ynot's `Paginator` class and `dedent` function, you can cleanly present long pieces of text in a terminal.

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

![paginator demo](https://media.giphy.com/media/tAn8Pis7lLUfA39MFa/giphy.gif)

The sample text is from [git-scm.com](https://git-scm.com/book/en/v2/Git-Branching-Rebasing).

### multithreading with terminal.h

If your app uses multiple threads, you can give the functions in terminal.h an osyncstream object to prevent race conditions.

Race condition example (no osyncstream object given):

```cpp
ynot::set_rgb(0, green, 0);  // set_rgb communicates with the terminal
ynot::print_at(x, y, message);
```

![race condition example](https://media.giphy.com/media/Zvfk7gsfI0n4dB4dkG/giphy.gif)

Each character is supposed to get steadily darker after it appears on screen, but sometimes the `print_at` function in one thread ends up being called between the `set_rgb` and `print_at` calls in another thread.

Prevent this output scrambling by giving the functions an [osyncstream](https://en.cppreference.com/w/cpp/io/basic_osyncstream) object, which gathers output and waits to print until it goes out of scope.

```cpp
#include <syncstream>  // requires C++20 or newer

...

{
    std::osyncstream sout(std::cout);
    ynot::set_rgb(0, green, 0, sout);
    ynot::print_at(x, y, message, sout);
}
```

![osyncstream demo](https://media.giphy.com/media/iArQ9LLVS30McyVR3u/giphy.gif)

You can see all of the code for what's shown in the gif [here](https://github.com/wheelercj/the-matrix).

If you're using wide strings (`std::wstring`s or `wchar_t*`s), many of the functions in terminal.h have wide string equivalents with names starting with `w_`. If you're multithreading with wide strings, you can give these wide string functions a `wosyncstream` object with a limited scope to prevent race conditions.

```cpp
{
    std::wstring message = L"this is a wide string";
    std::wosyncstream wsout(std::wcout);
    ynot::w_set_rgb(0, green, 0, wsout);
    ynot::w_print_at(x, y, message, wsout);
}
```
