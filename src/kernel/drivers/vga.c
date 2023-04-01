#pragma once

void clear(void){
	unsigned char* address = (unsigned char*) 0xB8000;
	for(unsigned int i = 0; i <= 2000; i++){
		*address++ = 0x0;
		*address++ = 0x0F;
	}
}

void kprint(const char* string, int x, char colour){
	unsigned char* address = (unsigned char*) 0xB8000 + (x << 1);
	while(*string != 0){
		*address++ = *string++;
		*address++ = colour;
	}
}
