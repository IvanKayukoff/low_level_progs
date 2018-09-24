%define last 0

%macro colon 2
%%elem_prev: dq last
db %1, 0

%2:
%define last %%elem_prev
%endmacro

colon "third word", third_word
db "third word explanation", 0

colon "second word", second_word
db "second word explanation", 0

colon "first word", first_word
db "first word explanation", 0

; vim: syntax=nasm

