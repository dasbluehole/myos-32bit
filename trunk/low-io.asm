;*****************************************************************
;LOW LEVEL IO ROUTINES; C CALLABLE
;*****************************************************************
[section .text]
[bits 32]
[global inportb]
[global outportb]
[global inportw]
[global gdt_flush]
	
;-----------------------
; C style function entry
;-----------------------
%macro prologue 1 
	push ebp 
	mov ebp,esp 
	sub esp,%1 
%endmacro 
;----------------------
;C style function exit
;----------------------
%macro epilogue 0
	mov esp,ebp 
	pop ebp 
%endmacro 
;===================================
;C callable functions
;===================================
;------------------------------------------
;unsigned char inportb(unsigned short port)
;------------------------------------------

inportb:
	prologue 4
        xor eax,eax
	mov dx, [ebp + 8 ]
	in  al, dx
	
	epilogue
	ret
;-------------------------------------------
;unsigned short inportw(unsigned short port)
;-------------------------------------------
inportw:
	prologue 4
	xor eax,eax
	mov dx, [ebp + 8 ]
	in  ax, dx

	epilogue
	ret

;------------------------------------------------------
;void outportb(unsigned short port,unsigned char value)
;------------------------------------------------------
outportb:
	prologue 8
	xor eax,eax
	mov dx, [ebp + 8 ]
	mov al, [ebp + 12 ]
	out  dx, al
	
	epilogue
	ret
;-----------------------------------------------------
;void gdt_flush(unsigned int &gdtr)
; This loads our new gdt !!!NOT GENERIC!!!
;-----------------------------------------------------
gdt_flush:
   prologue 4	
   mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
   lgdt [eax]        ; Load the new GDT pointer
   mov ax, 0x08		; code segment
   mov cs, ax
   mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
   mov ds, ax        ; Load all data segment selectors
   mov es, ax
   mov fs, ax
   mov gs, ax
   mov ax, 0x18	
   mov ss, ax      ;stack segment offset in our gdt 
   jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
   epilogue
   ret
