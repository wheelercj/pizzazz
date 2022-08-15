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
// utf8except.h -- Copyright (C) by Giovanni Dicanio
//
// This header file defines a C++ exception class that represents
// UTF-8 encoding conversion errors.
// This class is written using portable cross-platform C++ code,
// so this exception can be caught in cross-platform C++ portions of
// code (even if the throwing point is Windows-specific).
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GIOVANNI_DICANIO_WIN32_UTF8EXCEPT_H_
#define GIOVANNI_DICANIO_WIN32_UTF8EXCEPT_H_

#include <stdint.h>   // for uint32_t
#include <stdexcept>  // for std::runtime_error
#include <string>     // for std::string

namespace ynot
{
    namespace win32
    {

        //------------------------------------------------------------------------------
        // Error occurred during UTF-8 encoding conversions
        //------------------------------------------------------------------------------
        class Utf8ConversionException
            : public std::runtime_error
        {
        public:

            // Possible conversion "directions"
            enum class ConversionType
            {
                FromUtf8ToUtf16 = 0,
                FromUtf16ToUtf8
            };

            // Initialize with error message raw C-string, last Win32 error code and conversion direction
            Utf8ConversionException(const char* message, uint32_t errorCode, ConversionType type);

            // Initialize with error message string, last Win32 error code and conversion direction
            Utf8ConversionException(const std::string& message, uint32_t errorCode, ConversionType type);

            // Retrieve error code associated to the failed conversion
            uint32_t ErrorCode() const;

            // Direction of the conversion (e.g. from UTF-8 to UTF-16)
            ConversionType Direction() const;

        private:
            // Error code from GetLastError()
            uint32_t _errorCode;

            // Direction of the conversion
            ConversionType _conversionType;
        };

    } // namespace win32

#endif // GIOVANNI_DICANIO_WIN32_UTF8EXCEPT_H_

} // namespace ynot
