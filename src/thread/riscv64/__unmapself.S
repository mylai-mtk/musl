#include "lpad.h"

.global __unmapself
.type __unmapself, %function
__unmapself:
	LPAD(0)
	li a7, 215 # SYS_munmap
	ecall
	li a7, 93  # SYS_exit
	ecall

RISCV_ZICFILP_NOTE_GNU_PROPERTY_SECTION
