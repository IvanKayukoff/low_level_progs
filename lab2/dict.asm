global find_word
extern string_equals

section .text

; rdi - pointer to null-terminated search word
; rsi - pointer to the first element of the linked-list

; rax - result, if found then pointer to the found word, otherwise - 0
find_word:
    push r12
    push r13
    mov r12, rdi
    mov r13, rsi

    .loop:
        test r13, r13       ; if pointer to the element is null, then
        jz .not_found       ; return 0

        mov rdi, r12
        lea rsi, [r13 + 8]  ; using offset, because first 8 bytes is busy
                            ; by pointer to the previous element
        call string_equals  ; trying to find the wanted string

        test rax, rax       ; if not found, then load next elem to r13
        jnz .found          ; and continue
        mov r13, [r13]
        jmp .loop

    .not_found:
        xor rax, rax
        jmp .return

    .found:
        mov rax, r13

    .return:
        pop r13
        pop r12
        ret

