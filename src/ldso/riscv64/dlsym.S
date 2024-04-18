#include "lpad.h"

.global dlsym
.hidden __dlsym
.type dlsym, %function
dlsym:
	LPAD(0)
	mv a2, ra
	tail __dlsym

RISCV_ZICFILP_NOTE_GNU_PROPERTY_SECTION
