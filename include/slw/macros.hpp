#ifndef SLW_MACROS_HPP
#define SLW_MACROS_HPP

/// Build

#if defined(NDEBUG)
    
    #define SLW_BUILD_RELEASE

#else
    
    #define SLW_BUILD_DEBUG

#endif

/// Platform

#if defined(_WIN32)
    
    #define SLW_PLATFORM_WINDOWS

#elif defined(__linux__)

    #define SLW_PLATFORM_LINUX

#else

    #warning Unsupported Platform

#endif

/// Compiler

#if defined(_MSC_VER)
    
    // Microsoft VisualStudio C/C++
    #define SLW_COMPILER_MSVC _MSC_VER

#elif defined(__clang__)
    
    // LLVM Clang Compiler
    #define SLW_COMPILER_CLANG __clang__

#elif defined(__GNUC__)
    
    // GNU Compiler Collection
    #define SLW_COMPILER_GCC __GNUC__

#else

    #warning Unsupported Compiler

#endif

/// Function Name

#if defined(SLW_COMPILER_MSVC)

    // e.g. void add(int, int)
    #define SLW_FUNCTION_NAME __FUNCSIG__

#elif defined(SLW_COMPILER_CLANG) or defined(SLW_COMPILER_GCC)

    // e.g. void add(int, int)
    #define SLW_FUNCTION_NAME __PRETTY_FUNCTION__

#else

    // e.g. add
    #define SLW_FUNCTION_NAME __func__

#endif

/// Endian

#if defined(SLW_COMPILER_MSVC)

    // https://learn.microsoft.com/en-us/cpp/standard-library/bit-enum?view=msvc-170
    #define SLW_LITTLE_ENDIAN 0
    #define SLW_BIG_ENDIAN    1
    #define SLW_ENDIAN        SLW_LITTLE_ENDIAN

#elif defined(SLW_COMPILER_CLANG) or defined(SLW_COMPILER_GCC)

    #define SLW_LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
    #define SLW_BIG_ENDIAN    __ORDER_BIG_ENDIAN__
    #define SLW_ENDIAN        __BYTE_ORDER__

#else

    #warning Unable to Determine Endianness

#endif

/// Language

#if defined(__cplusplus)
    
    #define SLW_CXX __cplusplus

    #define SLW_CXX98

    #if SLW_CXX >= 201103L
        #define SLW_CXX11
    #endif

    #if SLW_CXX >= 201402L
        #define SLW_CXX14
    #endif

    #if SLW_CXX >= 201703L
        #define SLW_CXX17
    #endif

    #if SLW_CXX >= 202002L
        #define SLW_CXX20
    #endif

    #if SLW_CXX >= 202302L
        #define SLW_CXX23
    #endif

    #if SLW_CXX >= 202302L
        #define SLW_CXX23
    #endif

    #if SLW_CXX > 202302L // TODO: Determine correct cutoff
        #define SLW_CXX26
    #endif

#elif defined(__STDC_VERSION__)

    #define SLW_C __STDC_VERSION__

    #define SLW_C89

    #if SLW_C >= 199901L
        #define SLW_C99
    #endif

    #if SLW_C >= 201112L
        #define SLW_C11
    #endif

    #if SLW_C >= 201710L
        #define SLW_C17
    #endif

    #if SLW_C >= 202311L
        #define SLW_C23
    #endif

#else

    #warning Unsupported Language

#endif

/// Stringify

#define _SLW_STRINGIFY(x) #x
#define SLW_STRINGIFY(x) _SLW_STRINGIFY(x)

///
/// Linker Import/Export Visibility
///

#if defined(SLW_COMPILER_MSCV)

    #define SLW_API_EXPORT _declspec(dllexport)
    #define SLW_API_IMPORT _declspec(dllimport)

#elif defined(SLW_COMPILER_CLANG) || defined(SLW_COMPILER_GCC)

    #define SLW_API_EXPORT __attribute__((__visibility__("default")))
    #define SLW_API_IMPORT __attribute__((__visibility__("default")))

#endif

#endif // SLW_MACROS_HPP