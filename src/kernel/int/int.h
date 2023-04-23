#pragma once

#define picMasterCommand 0x20
#define picMasterData 0x21
#define picSlaveCommand 0xA0
#define picSlaveData 0xA1
#define picEndInterrupt 0x20
#define picDisable_ 0xFF

#define picMasterOffset 0x20
#define picSlaveOffset 0x28
#define ICW1NoICW4 0x01
#define ICW1Init 0x10
#define ICW3SlaveExist 0x04
#define ICW3SlaveCascade 0x02
#define ICW4_8086 0x01

typedef struct{
    unsigned int ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int intNumber, intCode;
    unsigned int eip, cs, eflags, useresp, ss;
}__attribute__((packed)) registers;

typedef void (*isr)(registers);