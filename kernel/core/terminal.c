
#pragma once

#include "../../lib/def.c"
#include "graphics.c"
#include "cursor.c"

void terminalInput(uint16_t start, uint16_t stop){
  uint8_t* input;
  uint16_t* buffer=(uint16_t*)0xB8000;
  uint16_t i=0;
  while(i<=stop-start){
    input[i]=buffer[start+i];
    i++;
  }
  input[i]='\0';
  return;
}

void terminalInit(uint8_t* message){
  kprint(message, cursorPosition, __WHITE__, __BLACK__);
  terminalBufferStartPoint=cursorPosition;
  terminalBufferEndPoint=cursorPosition;
  return;
}

