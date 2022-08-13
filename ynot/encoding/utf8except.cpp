#ifdef _WIN32

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

#endif // _WIN32
