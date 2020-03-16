#pragma once
#include <assert.h>
#include <iso646.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------------------------------------------------

#ifndef __cplusplus

    #ifndef static_assert
        #define static_assert(...) _Static_assert(__VA_ARGS__)
    #endif // static_assert

    #ifdef bool
        static_assert(sizeof(bool) == sizeof(_Bool), "nonstandard bool");
        #undef bool
        typedef _Bool bool;
    #endif // bool

#endif // __cplusplus

//------------------------------------------------------------------------------

typedef bool        args_bool;
typedef int         args_int;
typedef float       args_float;
typedef const char* args_path;
typedef const char* args_file;
typedef const char* args_string;

//------------------------------------------------------------------------------

const char* _args_consume(int* argc, const char** argp) {
    const char* const arg = *argp;
    while (*argp) { *argc -= 1; argp[0] = argp[1]; ++argp; }
    return arg;
}

//------------------------------------------------------------------------------

#define args_struct(NAME, ...)\
        _args_struct(NAME,\
            arg(bool, help, , "Display this information."),\
            __VA_ARGS__\
        )

#define _args_struct(NAME, ...)\
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/\
    typedef struct NAME {\
        const char* program;\
        _ARGS_MAP(_args_entry_, __VA_ARGS__)\
    } NAME;\
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/\
    static inline int\
    NAME##_usage(const NAME* const args) {\
        printf("Usage: %s [options] ...\n", args->program);\
        puts("Options:");\
        int columns[2] = {0};\
        _ARGS_MAP(_args_option_width_, __VA_ARGS__)\
        _ARGS_MAP(_args_option_print_, __VA_ARGS__)\
        return 1;\
    }\
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/\
    static inline NAME\
    NAME##_parse(int* const argc, const char* argv[]) {\
        NAME args;\
        args.program = _args_option_parse_program(argc, argv);\
        _ARGS_MAP(_args_option_parse_, __VA_ARGS__)\
        return args;\
    }

//------------------------------------------------------------------------------

#define _args_entry_(ARG) _ARGS_DEFER(__args_entry_##ARG)

#define __args_entry_arg(TYPE, NAME, FLAG, HELP)\
        args_##TYPE NAME;

#define __args_entry_end
#define __args_entry_

//------------------------------------------------------------------------------

#define _args_option_width_(ARG) _ARGS_DEFER(__args_option_width_##ARG)

#define __args_option_width_arg(TYPE, NAME, FLAG, HELP)\
        int NAME##_columns[2] = { sizeof("--") + sizeof(#NAME), 0 };\
        if (#FLAG[0]) NAME##_columns[1] = sizeof("-x");\
        if (columns[0] < NAME##_columns[0]) columns[0] = NAME##_columns[0];\
        if (columns[1] < NAME##_columns[1]) columns[1] = NAME##_columns[1];\

#define __args_option_width_end
#define __args_option_width_

//------------------------------------------------------------------------------

#define _args_option_print_(ARG) _ARGS_DEFER(__args_option_print_##ARG)

#define __args_option_print_arg(TYPE, NAME, FLAG, HELP)\
        fputs("  ", stdout);\
        if (#FLAG[0]) printf("-%s, ", #FLAG);\
        printf("--%-*s", columns[0], #NAME);\
        printf(" %s\n", HELP);\

#define __args_option_print_end
#define __args_option_print_

//------------------------------------------------------------------------------

#define _args_option_parse_(ARG) _ARGS_DEFER(__args_option_parse_##ARG)

#define __args_option_parse_arg(TYPE, NAME, FLAG, HELP)\
        args.NAME = _ARGS_DEFER(_ARGS_CONCAT2(_args_option_parse_,TYPE)(#NAME, #FLAG));

#define __args_option_parse_end
#define __args_option_parse_

static inline const char*
_args_option_parse_program(int* const argc, const char* argv[]) {
    const char* program = _args_consume(argc, argv);
    if (program) {
        const char* itr = program;
        const char* const last = itr + strlen(program) - 1;
        for (; itr < last; ++itr) {
            if (itr[0] == '\\' or itr[0] == '/') {
                program = itr + 1;
            }
        }
        return program;
    }
    return "";
}

static inline bool
_args_option_parse_bool(const char* name, const char* flag) {
    return true;
}

static inline int
_args_option_parse_int(const char* name, const char* flag) {
    return 2;
}

static inline float
_args_option_parse_float(const char* name, const char* flag) {
    return 3;
}

static inline const char*
_args_option_parse_path(const char* name, const char* flag) {
    return "path";
}

static inline const char*
_args_option_parse_file(const char* name, const char* flag) {
    return "file";
}

static inline const char*
_args_option_parse_string(const char* name, const char* flag) {
    return "string";
}

//------------------------------------------------------------------------------

#define _ARGS_DEFER(...) __ARGS_DEFER(__VA_ARGS__)
#define __ARGS_DEFER(...) __VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _ARGS_ARG_1(_1, ...)     _1
#define _ARGS_ARG_2(_1, _2, ...) _2

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _ARGS_CONCAT2(A,B) __ARGS_CONCAT2(A,B)
#define __ARGS_CONCAT2(A,B) A##B

#define _ARGS_CONCAT3(A,B,C) __ARGS_CONCAT3(A,B,C)
#define __ARGS_CONCAT3(A,B,C) A##B##C

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _ARGS_QUOTE(...) __ARGS_QUOTE(__VA_ARGS__)
#define __ARGS_QUOTE(...) #__VA_ARGS__

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _ARGS_UNPACK(BODY) _ARGS_DEFER(__ARGS_UNPACK BODY)
#define __ARGS_UNPACK(...) __VA_ARGS__

//------------------------------------------------------------------------------

#define _ARGS_VA_LEN(...)\
        _ARGS_DEFER(__ARGS_VA_LEN(__VA_ARGS__,\
            79,78,77,76,75,74,73,72,71,70,\
            69,68,67,66,65,64,63,62,61,60,\
            59,58,57,56,55,54,53,52,51,50,\
            49,48,47,46,45,44,43,42,41,40,\
            39,38,37,36,35,34,33,32,31,30,\
            29,28,27,26,25,24,23,22,21,20,\
            19,18,17,16,15,14,13,12,11,10,\
             9, 8, 7, 6, 5, 4, 3, 2, 1,end))
#define __ARGS_VA_LEN(\
            _79,_78,_77,_76,_75,_74,_73,_72,_71,_70,\
            _69,_68,_67,_66,_65,_64,_63,_62,_61,_60,\
            _59,_58,_57,_56,_55,_54,_53,_52,_51,_50,\
            _49,_48,_47,_46,_45,_44,_43,_42,_41,_40,\
            _39,_38,_37,_36,_35,_34,_33,_32,_31,_30,\
            _29,_28,_27,_26,_25,_24,_23,_22,_21,_20,\
            _19,_18,_17,_16,_15,_14,_13,_12,_11,_10,\
            __9,__8,__7,__6,__5,__4,__3,__2,__1,X,...) X

//------------------------------------------------------------------------------

#define _ARGS_MAP(M, ...)\
        _ARGS_DEFER(_ARGS_CONCAT2(__ARGS_MAP_,_ARGS_VA_LEN(__VA_ARGS__))(M,__VA_ARGS__))
#define __ARGS_MAP_1(M, _1)       M(_1)
#define __ARGS_MAP_2(M, _1, ...)  M(_1)_ARGS_DEFER(__ARGS_MAP_1(M, __VA_ARGS__))
#define __ARGS_MAP_3(M, _1, ...)  M(_1)_ARGS_DEFER(__ARGS_MAP_2(M, __VA_ARGS__))
#define __ARGS_MAP_4(M, _1, ...)  M(_1)_ARGS_DEFER(__ARGS_MAP_3(M, __VA_ARGS__))
#define __ARGS_MAP_5(M, _1, ...)  M(_1)_ARGS_DEFER(__ARGS_MAP_4(M, __VA_ARGS__))
#define __ARGS_MAP_6(M, _1, ...)  M(_1)_ARGS_DEFER(__ARGS_MAP_5(M, __VA_ARGS__))
#define __ARGS_MAP_7(M, _1, ...)  M(_1)_ARGS_DEFER(__ARGS_MAP_6(M, __VA_ARGS__))
#define __ARGS_MAP_8(M, _1, ...)  M(_1)_ARGS_DEFER(__ARGS_MAP_7(M, __VA_ARGS__))
#define __ARGS_MAP_9(M, _1, ...)  M(_1)_ARGS_DEFER(__ARGS_MAP_8(M, __VA_ARGS__))
#define __ARGS_MAP_10(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_9(M, __VA_ARGS__))
#define __ARGS_MAP_11(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_10(M, __VA_ARGS__))
#define __ARGS_MAP_12(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_11(M, __VA_ARGS__))
#define __ARGS_MAP_13(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_12(M, __VA_ARGS__))
#define __ARGS_MAP_14(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_13(M, __VA_ARGS__))
#define __ARGS_MAP_15(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_14(M, __VA_ARGS__))
#define __ARGS_MAP_16(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_15(M, __VA_ARGS__))
#define __ARGS_MAP_17(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_16(M, __VA_ARGS__))
#define __ARGS_MAP_18(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_17(M, __VA_ARGS__))
#define __ARGS_MAP_19(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_18(M, __VA_ARGS__))
#define __ARGS_MAP_20(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_19(M, __VA_ARGS__))
#define __ARGS_MAP_21(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_20(M, __VA_ARGS__))
#define __ARGS_MAP_22(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_21(M, __VA_ARGS__))
#define __ARGS_MAP_23(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_22(M, __VA_ARGS__))
#define __ARGS_MAP_24(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_23(M, __VA_ARGS__))
#define __ARGS_MAP_25(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_24(M, __VA_ARGS__))
#define __ARGS_MAP_26(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_25(M, __VA_ARGS__))
#define __ARGS_MAP_27(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_26(M, __VA_ARGS__))
#define __ARGS_MAP_28(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_27(M, __VA_ARGS__))
#define __ARGS_MAP_29(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_28(M, __VA_ARGS__))
#define __ARGS_MAP_30(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_29(M, __VA_ARGS__))
#define __ARGS_MAP_31(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_30(M, __VA_ARGS__))
#define __ARGS_MAP_32(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_31(M, __VA_ARGS__))
#define __ARGS_MAP_33(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_32(M, __VA_ARGS__))
#define __ARGS_MAP_34(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_33(M, __VA_ARGS__))
#define __ARGS_MAP_35(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_34(M, __VA_ARGS__))
#define __ARGS_MAP_36(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_35(M, __VA_ARGS__))
#define __ARGS_MAP_37(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_36(M, __VA_ARGS__))
#define __ARGS_MAP_38(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_37(M, __VA_ARGS__))
#define __ARGS_MAP_39(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_38(M, __VA_ARGS__))
#define __ARGS_MAP_40(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_39(M, __VA_ARGS__))
#define __ARGS_MAP_41(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_40(M, __VA_ARGS__))
#define __ARGS_MAP_42(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_41(M, __VA_ARGS__))
#define __ARGS_MAP_43(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_42(M, __VA_ARGS__))
#define __ARGS_MAP_44(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_43(M, __VA_ARGS__))
#define __ARGS_MAP_45(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_44(M, __VA_ARGS__))
#define __ARGS_MAP_46(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_45(M, __VA_ARGS__))
#define __ARGS_MAP_47(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_46(M, __VA_ARGS__))
#define __ARGS_MAP_48(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_47(M, __VA_ARGS__))
#define __ARGS_MAP_49(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_48(M, __VA_ARGS__))
#define __ARGS_MAP_50(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_49(M, __VA_ARGS__))
#define __ARGS_MAP_51(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_50(M, __VA_ARGS__))
#define __ARGS_MAP_52(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_51(M, __VA_ARGS__))
#define __ARGS_MAP_53(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_52(M, __VA_ARGS__))
#define __ARGS_MAP_54(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_53(M, __VA_ARGS__))
#define __ARGS_MAP_55(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_54(M, __VA_ARGS__))
#define __ARGS_MAP_56(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_55(M, __VA_ARGS__))
#define __ARGS_MAP_57(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_56(M, __VA_ARGS__))
#define __ARGS_MAP_58(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_57(M, __VA_ARGS__))
#define __ARGS_MAP_59(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_58(M, __VA_ARGS__))
#define __ARGS_MAP_60(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_59(M, __VA_ARGS__))
#define __ARGS_MAP_61(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_60(M, __VA_ARGS__))
#define __ARGS_MAP_62(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_61(M, __VA_ARGS__))
#define __ARGS_MAP_63(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_62(M, __VA_ARGS__))
#define __ARGS_MAP_64(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_63(M, __VA_ARGS__))
#define __ARGS_MAP_65(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_64(M, __VA_ARGS__))
#define __ARGS_MAP_66(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_65(M, __VA_ARGS__))
#define __ARGS_MAP_67(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_66(M, __VA_ARGS__))
#define __ARGS_MAP_68(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_67(M, __VA_ARGS__))
#define __ARGS_MAP_69(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_68(M, __VA_ARGS__))
#define __ARGS_MAP_70(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_69(M, __VA_ARGS__))
#define __ARGS_MAP_71(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_70(M, __VA_ARGS__))
#define __ARGS_MAP_72(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_71(M, __VA_ARGS__))
#define __ARGS_MAP_73(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_72(M, __VA_ARGS__))
#define __ARGS_MAP_74(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_73(M, __VA_ARGS__))
#define __ARGS_MAP_75(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_74(M, __VA_ARGS__))
#define __ARGS_MAP_76(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_75(M, __VA_ARGS__))
#define __ARGS_MAP_77(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_76(M, __VA_ARGS__))
#define __ARGS_MAP_78(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_77(M, __VA_ARGS__))
#define __ARGS_MAP_79(M, _1, ...) M(_1)_ARGS_DEFER(__ARGS_MAP_78(M, __VA_ARGS__))

//------------------------------------------------------------------------------
