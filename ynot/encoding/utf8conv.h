// source of this library: https://github.com/GiovanniDicanio/UnicodeConversions

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
