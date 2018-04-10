loader.asm
[BITS 32]
global start
start:
    mov esp, _sys_stack
    jmp stublet

ALIGN 4
mboot:
    MBOOT_PAGE_ALIGN        equ 1<<0
    MBOOT_MEMORY_INFO       equ 1<<1
    MBOOT_HEADER_MAGIC      equ 0x1BADB002
    MBOOT_HEADER_FLAGS      equ MBOOT_PAGE_ALIGN | MBOOT_MEMORY_INFO
    MBOOT_CHECKSUM          equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

    ; This is the GRUB Multiboot header. A boot signature
    dd MBOOT_HEADER_MAGIC
    dd MBOOT_HEADER_FLAGS
    dd MBOOT_CHECKSUM

stublet:
    EXTERN kernel_main
    call kernel_main
    jmp $
SECTION .bss
    resb 8192

_sys_stack:
