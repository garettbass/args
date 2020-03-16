#pragma once

#ifdef __cplusplus

    #include <cstdio>

    static inline int
    echo(const char* x, bool v)
    { return printf("%s: %s\n", x, v?"true":"false"); }

    static inline int
    echo(const char* x, char v)
    { return printf("%s: '%c'\n", x, v); }

    static inline int
    echo(const char* x, signed char v)
    { return printf("%s: %hhi\n", x, v); }

    static inline int
    echo(const char* x, signed short v)
    { return printf("%s: %hi\n", x, v); }

    static inline int
    echo(const char* x, signed int v)
    { return printf("%s: %i\n", x, v); }

    static inline int
    echo(const char* x, signed long int v)
    { return printf("%s: %li\n", x, v); }

    static inline int
    echo(const char* x, signed long long int v)
    { return printf("%s: %lli\n", x, v); }

    static inline int
    echo(const char* x, unsigned char v)
    { return printf("%s: %hhu\n", x, v); }

    static inline int
    echo(const char* x, unsigned short v)
    { return printf("%s: %hu\n", x, v); }

    static inline int
    echo(const char* x, unsigned int v)
    { return printf("%s: %u\n", x, v); }

    static inline int
    echo(const char* x, unsigned long int v)
    { return printf("%s: %lu\n", x, v); }

    static inline int
    echo(const char* x, unsigned long long int v)
    { return printf("%s: %llu\n", x, v); }

    static inline int
    echo(const char* x, float v)
    { return printf("%s: %g\n", x, v); }

    static inline int
    echo(const char* x, double v)
    { return printf("%s: %g\n", x, v); }

    static inline int
    echo(const char* x, long double v)
    { return printf("%s: %Lg\n", x, v); }

    static inline int
    echo(const char* x, const char* v)
    { return printf("%s: \"%s\"\n", x, v); }

    static inline int
    echo(const char* x, const void* v)
    { return printf("%s: %p\n", x, v); }

    #define echo(expr) (echo(#expr, (expr)))

#else // C11

    #include <stdbool.h>
    #include <stdio.h>

    static inline int
    echo_bool(const char* x, bool v)
    { return printf("%s: %s\n", x, v?"true":"false"); }

    static inline int
    echo_char(const char* x, char v)
    { return printf("%s: '%c'\n", x, v); }

    static inline int
    echo_schar(const char* x, signed char v)
    { return printf("%s: %hhi\n", x, v); }

    static inline int
    echo_short(const char* x, short v)
    { return printf("%s: %hi\n", x, v); }

    static inline int
    echo_int(const char* x, int v)
    { return printf("%s: %i\n", x, v); }

    static inline int
    echo_long(const char* x, long v)
    { return printf("%s: %li\n", x, v); }

    static inline int
    echo_long_long(const char* x, long long v)
    { return printf("%s: %lli\n", x, v); }

    static inline int
    echo_uchar(const char* x, unsigned char v)
    { return printf("%s: %hhu\n", x, v); }

    static inline int
    echo_ushort(const char* x, unsigned short v)
    { return printf("%s: %hu\n", x, v); }

    static inline int
    echo_uint(const char* x, unsigned int v)
    { return printf("%s: %u\n", x, v); }

    static inline int
    echo_ulong(const char* x, unsigned long v)
    { return printf("%s: %lu\n", x, v); }

    static inline int
    echo_ulong_long(const char* x, unsigned long long v)
    { return printf("%s: %llu\n", x, v); }

    static inline int
    echo_float(const char* x, float v)
    { return printf("%s: %g\n", x, v); }

    static inline int
    echo_double(const char* x, double v)
    { return printf("%s: %g\n", x, v); }

    static inline int
    echo_long_double(const char* x, long double v)
    { return printf("%s: %Lg\n", x, v); }

    static inline int
    echo_cstr(const char* x, const char* v)
    { return printf("%s: \"%s\"\n", x, v); }

    static inline int
    echo_ptr(const char* x, const void* v)
    { return printf("%s: %p\n", x, v); }

    #define echo(expr)\
            (_Generic((expr),\
                bool:                   echo_bool,\
                char:                   echo_char,\
                /* signed */\
                signed char:            echo_schar,\
                short:                  echo_short,\
                int:                    echo_int,\
                long:                   echo_long,\
                long long:              echo_long_long,\
                /* unsigned */\
                unsigned char:          echo_uchar,\
                unsigned short:         echo_ushort,\
                unsigned int:           echo_uint,\
                unsigned long:          echo_ulong,\
                unsigned long long:     echo_ulong_long,\
                /* floating point */\
                float:                  echo_float,\
                double:                 echo_double,\
                long double:            echo_long_double,\
                /* pointers */\
                const char*:            echo_cstr,\
                char*:                  echo_cstr,\
                default:                echo_ptr\
            )(#expr, expr))

#endif
