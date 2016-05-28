;--------------------------------------------------------------------------------------
;
;	boot.s	-- 内核开始的地方
;
;--------------------------------------------------------------------------------------

; Multiboot 魔数
MBOOT_HEADER_MAGIC	equ	0x1BADB002

; 0号位表示所有的引导模块将按页(4kb)对齐
MBOOT_PAGE_ALIGN	equ	1 << 0

; 1
MBOOT_MEM_INFO		equ	1 << 1

MBOOT_HEADER_FLAGS	equ	MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO

MBOOT_CHECKSUM		equ	-(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

;--------------------------------------------------------------------------------------

[BITS 32]
section .text

dd	MBOOT_HEADER_MAGIC
dd	MBOOT_HEADER_FLAGS
dd	MBOOT_CHECKSUM

[GLOBAL start]			; 向外部声明内核代码入口, （提供给Linker）
[GLOBAL glb_mboot_ptr]	; 向外部声明 struct multiboot * 变量
[EXTERN kern_entry]		; 声明内核 C 代码的入口函数

start:
	cli
	
	mov		esp, STACK_TOP			; 设置内核栈地址
	mov 	ebp, 0					; 帧指针修改为 0
	and 	esp, 0xFFFFFFF0			; 栈地址按照 16 bytes 对齐
	mov		[glb_mboot_ptr], ebx	; 将 ebx 中储存的指针存入全局变量
	call	kern_entry				; 调用内核入口函数

stop:
	hlt				; 停机指令，可以降低CPU功耗
	jmp		stop	; 
	
;--------------------------------------------------------------------------------------

section .bss					; 未初始化的数据段从这里开始
stack:
	resb	32768				; 这里作为内核栈
glb_mboot_ptr:					; 全局的 multiboot 结构体指针
	resb	4

STACK_TOP	equ		$-stack-1	; 内核栈顶

;--------------------------------------------------------------------------------------