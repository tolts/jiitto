
#pragma once

#include "../../uefi/uefi.h"
#include "jil.h"

enum JIL_VAR_TYPES{
    JIL_VAR_TYPE_NUL,
    JIL_VAR_TYPE_INT,
    JIL_VAR_TYPE_CHR,
    JIL_VAR_TYPE_STR,
    JIL_VAR_TYPE_RET, // case of expr (it is not an actual variable)
};

typedef struct{
    char_t type;
    union{
        char_t *str;
        // TODO : jil_expr_t
    }data;
}jil_var_t;

typedef struct{
    jil_var_t *(*func)(jil_var_t[2]);
    jil_var_t parameters[2];
}jil_expr_t;
