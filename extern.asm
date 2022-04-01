[bits 64]
[extern kernel_access]

kernel_enabled: 
    call kernel_access
    jmp $

times 1024-($-$$) db 0