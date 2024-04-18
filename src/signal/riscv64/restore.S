#include "lpad.h"

.global __restore
.type __restore, %function
__restore:
.global __restore_rt
.type __restore_rt, %function
__restore_rt:
	LPAD(0)
	li a7, 139 # SYS_rt_sigreturn
	ecall

RISCV_ZICFILP_NOTE_GNU_PROPERTY_SECTION
