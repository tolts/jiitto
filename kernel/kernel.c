/*  kernel.c
    Copyright (C) 2022 schochtlts

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

unsigned int char_len(unsigned char* l){
    unsigned int len=0;
    while(l[len]){
        len++;
    }return len;
}

void clearc(unsigned int x){
    unsigned char* a=(unsigned char*)0xB8000+x*2;
    *a=0x20;
}

void clearl(unsigned int y){
    unsigned int x=y*80;
    for(unsigned int i=0; i<=80; i++){
        clearc(x+i);
    }
}

void printc(unsigned char c, unsigned int x){
    unsigned char* a=(unsigned char*)0xB8000+x*2;
    *a=c;
}

void printl(unsigned char* l, unsigned int x){
    unsigned int l_len=char_len(l);
    for(unsigned int i=0; i<=l_len; i++){
        printc(l[i], x+i);
    }
}

void kernel_enabled(void){
    clearl(0);
    unsigned char msg[]="Hello world!";
    printl(msg, 0);
}
