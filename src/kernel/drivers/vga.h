#pragma once

void cursorEnable(unsigned char rowStart, unsigned char rowEnd){
    portByteOut(0x3D4, 0x0A);
    portByteOut(0x3D5, (portByteIn(0x3D5) & 0xC0) | rowStart);
    portByteOut(0x3D4, 0x0B);
    portByteOut(0x3D5, (portByteIn(0x3D5) & 0xE0) | rowEnd);
}

void cursorDisable(void){
    portByteOut(0x3D4, 0x0A);
    portByteOut(0x3D5, 0x20);
}

void cursorMove(int x){
    portByteOut(0x3D4, 0x0F);
    portByteOut(0x3D5, (unsigned short)(x & 0xFF));
    portByteOut(0x3D4, 0x0E);
    portByteOut(0x3D5, (unsigned short)((x >> 8) & 0xFF));
}

unsigned short cursorPos(void){
	unsigned short x = 0;
	portByteOut(0x3D4, 0x0F);
	x |= portByteIn(0x3D5);
	portByteOut(0x3D4, 0x0E);
	x |= ((unsigned short)portByteIn(0x3D5)) << 8;
	return x;
}

void clear(void){
	unsigned char* address = (unsigned char*) 0xB8000;
	for(unsigned int i = 0; i <= 2000; i++){
		*address++ = 0x0;
		*address++ = 0x0F;
	}
}

void kprint(char* str, int x, char colour){
	unsigned char* vga = (unsigned char*) 0xB8000 + (x << 1);
	int len = 0;
	while(*str != 0){
		*vga++ = *str++;
		*vga++ = colour;
		len++;
	}
	cursorMove(x + len);
}

void kprintc(char c, int x, int y, char colour)
{
	int pos = y * 80 + x;
	unsigned char* vga = (unsigned char*) 0xB8000 + (pos << 1);
	*vga = c;
	*(vga + 1) = colour;
	
	cursorMove(pos);
}