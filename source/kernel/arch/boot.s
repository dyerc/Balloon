

MBOOT_PAGE_ALIGN    equ 1<<0
MBOOT_MEM_INFO      equ 1<<1
MBOOT_HEADER_MAGIC  equ 0x1BADB002
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

bits 32

section .text

global mboot
extern code
extern bss
extern end

mboot:
    dd  MBOOT_HEADER_MAGIC

    dd  MBOOT_HEADER_FLAGS
    dd  MBOOT_CHECKSUM
    dd mboot
          dd  code
          dd  bss
          dd  end
          dd  start


global start:function start.end-start
extern kernel_main

start:
    cli
    mov esp, stack
    push ebx
    mov ebp, 0

    call kernel_main
    jmp $
.end:

section .bss
    resb 32768
stack:
