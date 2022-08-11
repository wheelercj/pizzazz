# ynot

Add color, style, advanced cursor movements, autocomplete suggestions, and more to your C++ terminal app! This fully cross-platform library includes a variety of functions and classes to make common challenges easier.

## download

You can download this library into your project with git by using `git submodule add https://github.com/wheelercj/ynot` and then adding `#include "../ynot/ynot/ynot.h"` (and optionally `using namespace ynot;`) to the files you want to use this library in. If you're using Visual Studio, you will also need to [add the library's files to your project's solution explorer](https://wheelercj.github.io/notes/pages/20220807002725.html). Get future updates with `git submodule update --remote`. The library's version number is saved in ynot.h so that you can check whether you have the latest version in the future.

Alternatively, you can manually download [a zip file or a tar.gz file](https://github.com/wheelercj/ynot/tags) and go through a similar setup process.

## examples

See the header files for the lists of functions and their descriptions. There are many examples of how you can use this library below, in the test files, in [terminal paint](https://github.com/wheelercj/terminal-paint), and in [the matrix](https://github.com/wheelercj/the-matrix). C++17 or newer is required; in Visual Studio, you can choose the version of C++ with project > Properties > C/C++ > Language > C++ Language Standard.

The ynot library's `print` function (and other functions that print) allows you to print any emoji or other [Unicode](https://en.wikipedia.org/wiki/Unicode) symbols to a terminal. On Windows, printing emoji with C++ is normally very complicated. With this print function, it couldn't be more simple.

```cpp
ynot::print("🔥🐊");
ynot::print("\n I can print anything 😎🤖");
```

To print emoji, the file must be [saved with the UTF-8 encoding](https://docs.microsoft.com/en-us/visualstudio/ide/how-to-save-and-open-files-with-encoding?view=vs-2022) (_without_ signature/BOM) and your code must run in a modern terminal (such as [Windows Terminal](https://aka.ms/terminal); see [how to run your C++ app in Windows Terminal](https://wheelercj.github.io/notes/pages/20220506214620.html)). On Windows, you must use the emoji or Unicode symbol itself in your code rather than a Unicode code point (Unicode code points only work in `wstring`s).

Here is the `get_key` function which can instantly detect key presses without an enter key press:

```cpp
string key = "";
while (key != "escape")
{
    key = ynot::get_key();
    ynot::print("\r                               \r You pressed " + key);
}
```

![get_key demo](https://media.giphy.com/media/BVk0mTxjIu8Pgbeo1h/giphy.gif)

Below is an example of ynot's `Menu` class, which makes it easy to create centered menus controlled with the arrow and enter keys.

```cpp
ynot::Menu menu("sample menu", { "create", "list", "edit", "delete", "help", "settings", "exit" });
string choice = menu.run();
ynot::print("\nYou chose " + choice);
```

![menu demo](https://media.giphy.com/media/vUiPYlobVhnGrhKCTc/giphy.gif)

Here's an example of `dedent` which removes indentation from a multiline string, and `getline_ac` which can give autocomplete suggestions (not autocorrect) and has optional built-in input validation:

```cpp
string menu = ynot::dedent(R"(
    Sample menu:
     * Create
     * Read
     * Update
     * Delete
    > )");
ynot::print(menu);
string choice = ynot::getline_ac(
    { "Create", "Read", "Update", "Delete" },
    "type an option");
ynot::print("\n You chose " + choice);
```

![autocomplete menu example](https://media.giphy.com/media/Rqoco5DR2a2AjDAqtX/giphy.gif)

Below is another example. With ynot's `Paginator` class, you can cleanly present long pieces of text in a terminal.

```cpp
string article_title = "3.6 Git Branching - Rebasing";
string article_body = ynot::dedent(R"(
    Article body here.
    Indent with tabs or spaces, not both.)");
string line_prefix = "\n    ";
ynot::Paginator paginator(article_title, article_body, line_prefix);
paginator.run();
```

![paginator demo](https://media.giphy.com/media/tAn8Pis7lLUfA39MFa/giphy.gif)

The sample text is from [git-scm.com](https://git-scm.com/book/en/v2/Git-Branching-Rebasing).

### multithreading with terminal.h

If your app uses multiple threads that call functions in terminal.h, you can prevent race conditions by combining strings before printing.

Race condition example:

```cpp
string message;
int x, y, green;

...

ynot::set_cursor_coords(x, y);  // set_cursor_coords prints an ANSI code
ynot::print_rgb(0, green, 0, message);
```

![race condition example](https://media.giphy.com/media/Obc0RuoYP7XkHppQ0I/giphy.gif)

Each character is supposed to get steadily darker after it appears on screen, but sometimes the `print_rgb` function in one thread ends up being called between the `set_cursor_coords` and `print_rgb` calls in another thread.

Prevent this output scrambling by combining all strings into one before printing. The terminal.h functions that start with `ret_` return strings instead of printing them so that you can wait to print until after combining strings.

```cpp
string coord_str = ynot::ret_set_cursor_coords(x, y);
ynot::print_rgb(0, green, 0, coord_str + message);
```

![matrix demo](https://media.giphy.com/media/iArQ9LLVS30McyVR3u/giphy.gif)

You can see all of the code for what's shown in the gif [here](https://github.com/wheelercj/the-matrix).
