#pragma once

int charLen(char* str){
    int len = 0;
    while(str[len]){
        len++;
    }
    return len;
}

char* charAddNull(char* str){
    str[charLen(str) + 1] = 0;
    return str;
}