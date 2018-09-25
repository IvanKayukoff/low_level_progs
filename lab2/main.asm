global _start

section .data
; here will be const messages to print
msg_not_found: db "Word not found", 0
msg_overflow: db "Buffer ovelflow", 0

section .text

%include "words.inc"

extern find_word
extern read_word
extern string_length
extern print_string
extern print_newline
extern print_err

_start:
    sub rsp, 256            ; malloc 256 bytes on the stack
    mov rsi, 255            ; max word length

    mov rdi, rsp
    call read_word
    test rax, rax           ; rax == 0 if input was too long
    jz .buffer_overflow
    
    mov rdi, rsp            ; wanted word to rdi
    mov rsi, first          ; first - pointer to the first element in the list

    call find_word          ; if word wasn't found, go to the .not_found
    test rax, rax           ; otherwise rax contains the pointer to the wanted
    jz .not_found           ; element in the list

    lea rdi, [rax+8]        ; first 8 bytes - pointer to the first elem
    call string_length      ; next N bytes - string with name
    lea rdi, [rdi+rax+1]    ; so we need skip it
                            ; plus one for the null-terminator
    call print_string
    jmp .exit
    
    .buffer_overflow:
        mov rdi, msg_overflow
        call print_err
        jmp .exit

    .not_found:
        mov rdi, msg_not_found
        call print_err

    .exit:
        call print_newline
        xor rdi, rdi        ; rdi - return code
        mov rax, 60         ; 60 - exit 
        syscall

