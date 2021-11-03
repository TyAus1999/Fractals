.intel_syntax noprefix
.text		#First 6 params fo into rdi, rsi, rdx, rcx, r8, r9
			#rbx is preserved for all function calls
.global example

example:
	mov rax,1
	ret

