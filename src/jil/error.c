
#pragma once

#include "../../uefi/uefi.h"
#include "jil.h"

enum JIL_ERROR_NO{
    JIL_ERROR_UNRESOLVED,
    JIL_ERROR_VAR_TYPE,
};

#define JIL_ERROR_MSG_TEMPLATE(str) "[ JIL error : " str " ]\n"

void jil_error(char_t error){
    switch(error){
        case JIL_ERROR_UNRESOLVED:
            printf(JIL_ERROR_MSG_TEMPLATE("unknown error"));
        case JIL_ERROR_VAR_TYPE:
            printf(JIL_ERROR_MSG_TEMPLATE("unfit variable type"));
    }
    return;
}
