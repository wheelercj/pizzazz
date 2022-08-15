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
// utf8except.cpp -- Copyright (C) by Giovanni Dicanio
//
// This file defines a C++ exception class that represents
// UTF-8 encoding conversion errors.
// This class is written using portable cross-platform C++ code,
// so this exception can be caught in cross-platform C++ portions of
// code (even if the throwing point is Windows-specific).
//
////////////////////////////////////////////////////////////////////////////////

#include "utf8except.h"

namespace ynot
{
    namespace win32
    {
        Utf8ConversionException::Utf8ConversionException(
            const char* const message,
            const uint32_t errorCode,
            const ConversionType type)

            : std::runtime_error(message)
            , _errorCode(errorCode)
            , _conversionType(type)
        {
        }

        Utf8ConversionException::Utf8ConversionException(
            const std::string& message,
            const uint32_t errorCode,
            const ConversionType type)

            : std::runtime_error(message)
            , _errorCode(errorCode)
            , _conversionType(type)
        {
        }

        uint32_t Utf8ConversionException::ErrorCode() const
        {
            return _errorCode;
        }

        Utf8ConversionException::ConversionType Utf8ConversionException::Direction() const
        {
            return _conversionType;
        }

    } // namespace win32

} // namespace ynot
