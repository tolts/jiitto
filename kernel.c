void kernel_access(void){
    char* video_adress = (char*)0xB8000;
    *video_adress = 'X';
    while(0);
}