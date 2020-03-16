////usr/bin/env \
    [ -n "${PATHEXT}" ] && ext='.exe'; \
    bin="$(dirname $0)/$(basename ${0%.*})$ext"; \
    clang -std=c11 -Werror -g -o $bin $0 \
    && \
    $bin "$@"; \
    status=$?; \
    rm $bin; \
    exit $status

////usr/bin/env ccx -r %@; exit $?
/*ccx:{
    cc:"-std=c11 -Werror -I../serial"
    configurations:{
        debug:{
            cc:"-g"
        }
        release:{
            cc:"-O2"
        }
    }
}*/

#include "args.h"
#include "echo.h"

args_struct(
    appc_args,
    arg(bool,   ast,               a, "print AST to stdout"),
    arg(bool,   debug,             d, "print debug summary to stdout"),
    arg(path,   include_directory, I, "Add <dir> to include search path."),
    arg(string, define_macro,      D, "Define <macro> to <value> (or 1 if <value> omitted)"),
    arg(bool,   test,              t, "run tests"),
    arg(bool,   glsl,              g, "translate GPUC to GLSL"),
    arg(bool,   hlsl,              h, "translate GPUC to HLSL"),
    arg(bool,   metal,             m, "translate GPUC to Metal"),
    arg(file,   frag,              f, "write output to <file>, or '-' for stdout"),
    arg(file,   vert,              v, "write output to <file>, or '-' for stdout"),
);

int main(int argc, const char* argv[], const char* envp[]) {
    const appc_args args = appc_args_parse(&argc, argv);
    // echo(args.program);
    // echo(args.help);
    // echo(args.ast);
    // echo(args.debug);
    // echo(args.frag);
    // echo(args.vert);
    if (args.help) {
        return appc_args_usage(&args);
    }
    return 0;
}