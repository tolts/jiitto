
#include "../uefi/uefi.h"
#include "minilua.c"

int main(int argc, wchar_t **argv){
	printf("Starting 'BOOTX64.efi' ... Expected 'init.lua' ");

	DIR *dh;
    struct dirent *de;

    if((dh = opendir("\\EFI"))) {
        while ((de = readdir(dh)) != NULL) {
            printf("%c %04x %s\n", de->d_type == DT_DIR ? 'd' : '.', de->d_type, de->d_name);
        }
        closedir(dh);
    } else{
        fprintf(stderr, "Unable to open directory\n");
	}

	//lua_State *L=luaL_newstate();
	//lua_close(L);

	return 0;
}
