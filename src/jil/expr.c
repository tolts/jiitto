
#pragma once

#include "../../uefi/uefi.h"
#include "jil.h"

jil_expr_t *jil_expr_gen(char_t *str){
    jil_expr_t *expr=(jil_expr_t *)calloc(1, sizeof(jil_expr_t));
    const char_t *delim=" ";
    char_t *token;
    for(token=strtok(str, delim); token; token=strtok(NULL, delim)){
        if(!strcmp(jil_func_echo_name, token)){
            expr->func=jil_func_echo;
            expr->parameters->type=JIL_VAR_TYPE_STR;
            char_t *candidate_str=strtok(NULL, "\"\'");
            if(candidate_str==NULL){
                jil_error(JIL_ERROR_VAR_TYPE);
            }else{
                expr->parameters->data.str=candidate_str;
            }
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
