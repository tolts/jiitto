
#pragma once

typedef struct{
  uint16_t offset0_15;
  uint16_t segmentSelector16_31;
  uint8_t reserved32_39;
  uint8_t typeAttributes40_47;
  uint16_t offset48_63;
}__attribute__((packed)) idtSegmentDescriptor_t;

typedef struct{
  uint16_t size;
  idtSegmentDescriptor_t* offset;
}__attribute__((packed)) idtDescriptor_t;

idtSegmentDescriptor_t idt[256];

idtDescriptor_t idtr = {sizeof(idt) - 1, idt};

