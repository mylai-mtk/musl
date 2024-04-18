#include "lpad.h"

.global vfork
.type vfork,@function
vfork:
	LPAD(0)
	/* riscv does not have SYS_vfork, so we must use clone instead */
	/* note: riscv's clone = clone(flags, sp, ptidptr, tls, ctidptr) */
	li a7, 220
	li a0, 0x100 | 0x4000 | 17 /* flags = CLONE_VM | CLONE_VFORK | SIGCHLD */
	mv a1, sp
	/* the other arguments are ignoreable */
	ecall
	.hidden __syscall_ret
	j __syscall_ret

RISCV_ZICFILP_NOTE_GNU_PROPERTY_SECTION
