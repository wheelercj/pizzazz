// source: https://github.com/GiovanniDicanio/UnicodeConversions

/*
MIT License

Copyright (c) 2016 by Giovanni Dicanio

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

////////////////////////////////////////////////////////////////////////////////
//
// utf8conv.h -- Copyright (C) by Giovanni Dicanio
//
// This header file defines a couple of C++ functions to convert between
// UTF-8 and UTF-16 Unicode encodings.
//
// std::string is used to store UTF-8-encoded strings.
// std::wstring is used to store UTF-16-encoded strings.
//
// In case of conversion errors, instances of the Utf8ConversionException
// class are thrown.
//
// If the input string is too big and its size can't be safely converted
// to a value of type int, a std::overflow_error exception is thrown.
//
// Note that std::wstring is non-portable, as wchar_t's size is non-portable
// (e.g. 16 bits on Win32/Visual C++, 32 bits on Linux/GCC).
// But since this code directly interacts with Win32 APIs (MultiByteToWideChar
// and WideCharToMultiByte in particular), this portion of code is already
// platform-specific.
//
// This code is based on my MSDN Magazine article published 
// on the 2016 September issue:
//
// "C++ - Unicode Encoding Conversions with STL Strings and Win32 APIs"
// https://msdn.microsoft.com/magazine/mt763237
//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GIOVANNI_DICANIO_WIN32_UTF8CONV_H_
#define GIOVANNI_DICANIO_WIN32_UTF8CONV_H_
#ifdef _WIN32

#include <string>        // For std::string and std::wstring
#include "utf8except.h"  // Custom exception class for UTF-8 conversion errors

namespace ynot
{
    namespace win32
    {
        //------------------------------------------------------------------------------
        // Convert form UTF-8 to UTF-16.
        // Throws Utf8ConversionException on conversion errors
        // (e.g. invalid UTF-8 sequence found in input string).
        //------------------------------------------------------------------------------
        std::wstring Utf8ToUtf16(const std::string& utf8);

        //------------------------------------------------------------------------------
        // Convert form UTF-16 to UTF-8.
        // Throws Utf8ConversionException on conversion errors
        // (e.g. invalid UTF-16 sequence found in input string).
        //------------------------------------------------------------------------------
        std::string Utf16ToUtf8(const std::wstring& utf16);

    } // namespace win32

#endif // GIOVANNI_DICANIO_WIN32_UTF8CONV_H_

} // namespace ynot

#endif // _WIN32
