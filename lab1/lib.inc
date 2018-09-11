section .data

newline_char: db 10

section .text

string_length:
    xor rax, rax				; here will be result
	.loop:	
		cmp byte [rdi+rax], 0	; compare current symbol and null-terminator
		je .return				; jump if current symbol is 0
		inc rax					; otherwise increment counter
		jmp .loop
	.return:
    ret

print_string:
	call string_length			; result of string_length -> rax
	mov rsi, rdi				; point to text
	mov rdx, rax				; amount of bytes
	mov rax, 1					; write
	mov rdi, 1					; stdout
	syscall
	xor rax, rax
    ret


print_char:
	push rdi					; pushing char in the stack
	mov rsi, rsp				; point to char
	mov rax, 1					; write
	mov rdi, 1					; stdout
	mov rdx, 1					; amount of bytes
	syscall
	pop rdi						; restore stack
    xor rax, rax
    ret

; printing newline character
print_newline:
	mov rax, 1					; write
	mov rdi, 1					; stdout
	mov rsi, newline_char		; point to text
	mov rdx, 1					; amount of bytes
	syscall
    xor rax, rax				; rax <- 0
    ret


print_uint:
	xor rcx, rcx				; nulling counter
	mov rax, rdi				; rax always keeps the quotient
	mov r9, 10					; radix
	push r12
	mov r12, rsp
	dec rsp
	mov byte [rsp], 0			; null-terminator
	.loop:
		dec rsp
		xor rdx, rdx
		div r9					; dividing by radix
		add dl, 0x30
		mov [rsp], dl			; push remainder
		test rax, rax			; set ZF = 0 if rax == 0
		jnz .loop				; if quotient == 0 then divided
	.return:
	mov rdi, rsp
	call print_string
	mov rsp, r12
	pop r12
    xor rax, rax
    ret


print_int:
    xor rax, rax
	test rdi, rdi
	js .negative				; if negative print '-' character
	call print_uint				; make it positive
	jmp .ret					; and print uint
	.negative:
	mov r8, rdi
	mov rdi, '-'
	call print_char
	mov rdi, r8
	neg rdi
	call print_uint
	.ret:
    ret

string_equals:
    xor rcx, rcx
	.loop:
		mov r8b, byte [rdi+rcx]
		mov r9b, byte [rsi+rcx]
		cmp r8b, r9b
		jne .not_equals
		test r8b, r8b
		jz .equals
		inc rcx
		jmp .loop
	.equals:
	mov rax, 1
    ret
	.not_equals:
	mov rax, 0
	ret


read_char:
    xor rax, rax
	xor rdi, rdi
	mov rdx, 1
	dec rsp
	mov rsi, rsp
	syscall
	xor rax, rax
	mov al, [rsp]
	inc rsp
    ret 

read_word:
	mov r8, rdi					; buffer address to r8
	mov r9, rsi					; buffer size to r9
	xor rcx, rcx				; nulling counter
	.loop:
		call read_char
		cmp rcx, r9				; if counter >= buffer size
		jge .overflow			; then go to .overflow
		cmp al, 0x9
		je .return				; if character == ' ' or '\t' or '\n'
		cmp al, 0x10			; then go to .return
		je .return
		cmp al, 0x20
		je .return
		mov byte [r8+rcx], al	; if character == 0 then go to .return
		test al, al
		jz .return				; otherwise write the character to the buffer
		inc rcx					; and do one more iteration
		jmp .loop
	.overflow:
	xor rax, rax				; return 0
    ret
	.return:
	test rcx, rcx
	jz .loop
	mov rdx, rcx				; word length to rdx
	mov rax, r8					; buffer address to rax
	inc rcx
	mov byte [rax+rcx], 0		; null-terminator at the end of word
	ret

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
    xor rax, rax
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rax, rax
    ret 


string_copy:
	xor rcx, rcx				; nulling counter
	.loop:
		cmp rcx, rdx			; if counter == buffer's length
		je .return				; then string overflows the buffer
		mov rax, [rdi+rcx]		
		mov [rsi+rcx], rax
		cmp rax, 0				; if current byte == 0
		je .return_fits			; then string fits the buffer
		inc rcx
		jmp .loop
	.return_fits:				; returns buffer's pointer
	mov rax, rsi				; if string fits the buffer
    ret
	.return:					; returns zero
	xor rax, rax				; if buffer has been overflowed
	ret


