#include "lpad.h"

.global __set_thread_area
.type   __set_thread_area, %function
__set_thread_area:
	LPAD(0)
	mv tp, a0
	li a0, 0
	ret

RISCV_ZICFILP_NOTE_GNU_PROPERTY_SECTION
