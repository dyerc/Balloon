global read_eip
read_eip: ; Clever girl
  pop eax
  jmp eax

global copy_page_physical
copy_page_physical:
    push ebx
    pushf
    cli
    mov ebx, [esp+12]
    mov ecx, [esp+16]
    mov edx, cr0
    and edx, 0x7FFFFFFF
    mov cr0, edx
    mov edx, 0x400
.page_loop:
    mov eax, [ebx]
    mov [ecx], eax
    add ebx, 4
    add ecx, 4
    dec edx
    jnz .page_loop
    mov edx, cr0
    or  edx, 0x80000000
    mov cr0, edx
    popf
    pop ebx
    ret
