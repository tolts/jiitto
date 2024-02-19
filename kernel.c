
void kernel_main(void){
  unsigned char* p=(unsigned char*)0xB8000;
  *p='p';
  return;
}
