#pragma once

char* intToAscii(int n, int base){
	if(n == 0){
		return "0";
	}
	static char buffer[32];
	int i = 32;
	for(; n && i ; --i, n /= base){
		buffer[i] = "0123456789abcdef"[n % base];
    }
	return &buffer[i + 1];
}