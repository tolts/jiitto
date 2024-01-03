
#pragma once

typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int int64_t;
typedef unsigned long long int uint64_t;

int8_t* typesInt2CharReversed(uint32_t n, uint32_t base){
  /*int8_t* str = "0";
  for(uint8_t i = 0; i<32; i++){
    str[i]="0123456789ABCDEF"[n%base];
    n/=10;
  }
  return str;*/
  static char buf[32] = {0};
	
	int i = 30;
	
	for(; n && i ; --i, n /= base)
	
		buf[i] = "0123456789abcdef"[n % base];
	
	return &buf[i+1];
}

