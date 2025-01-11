
#pragma once

#include "../../../uefi/uefi.h"
#include "../jil.h"

const char_t *jil_func_echo_name="echo";

jil_var_t *jil_func_echo(jil_var_t parameters[2]){
    if(parameters[0].type!=JIL_VAR_TYPE_STR){
        jil_error(JIL_ERROR_VAR_TYPE);
    }else{
        printf(parameters[0].data.str);
    }
    return NULL;
}

