
#pragma once

#include "../../uefi/uefi.h"

/*

------------------- Jiitto Interpreter Language --------------------

*/

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

const char_t *jil_func_echo_name="echo";

jil_var_t *jil_func_echo(jil_var_t parameters[2]){
    if(parameters[0].type!=JIL_VAR_TYPE_STR){
        jil_error(JIL_ERROR_VAR_TYPE);
    }else{
        printf(parameters[0].data.str);
    }
    return NULL;
}


jil_expr_t *jil_expr_gen(char_t *str){
    jil_expr_t *expr=(jil_expr_t *)calloc(1, sizeof(jil_expr_t));
    const char_t *delim=" ";
    char_t *token;
    for(token=strtok(str, delim); token; token=strtok(NULL, delim)){
        if(!strcmp(jil_func_echo_name, token)){
            expr->func=jil_func_echo;
            expr->parameters->type=JIL_VAR_TYPE_INT;
            expr->parameters->data.str=strtok(NULL, "\"");
            break;
        }
    }
    return expr;
}

void jil_expr_exec(jil_expr_t *expr){
    expr->func(expr->parameters);
    free(expr);
    return;
}



