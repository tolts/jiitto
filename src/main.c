
#include "../uefi/uefi.h"
#include "jil/jil.h"

int main(int argc, char_t **argv){
    jil_expr_exec(jil_expr_gen("echo \"cc\n\""));
    sleep(3);
    return 0;
}
