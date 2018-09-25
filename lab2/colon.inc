%ifndef _COLON_
%define _COLON_

%define first 0

%macro colon 2
%%elem_prev: dq first
db %1, 0

%2:
%define first %%elem_prev
%endmacro

%endif

; vim: syntax=nasm

