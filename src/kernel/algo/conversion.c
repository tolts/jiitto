
char* convertIntAscii(int n){
    char str[10];
    int i = 0;
    n *= 10;
    while((n /= 10) > 0){
        str[i++] = (n % 10) + '0';
    }
    int len = charLen(str);
    i = 0;
    char temp;
    while(i < len/2){
        temp = str[len - i - 1] ;
        str[len - i - 1] = str[i];
        str[i] = temp;
        i++;
    }
    str[len + 1] = '\0';
    return str;
}