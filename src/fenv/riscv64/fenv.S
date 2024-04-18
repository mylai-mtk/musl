#include "lpad.h"

#ifdef __riscv_flen

.global feclearexcept
.type feclearexcept, %function
feclearexcept:
	LPAD(0)
	csrc fflags, a0
	li a0, 0
	ret

.global feraiseexcept
.type feraiseexcept, %function
feraiseexcept:
	LPAD(0)
	csrs fflags, a0
	li a0, 0
	ret

.global fetestexcept
.type fetestexcept, %function
fetestexcept:
	LPAD(0)
	frflags t0
	and a0, t0, a0
	ret

.global fegetround
.type fegetround, %function
fegetround:
	LPAD(0)
	frrm a0
	ret

.global __fesetround
.type __fesetround, %function
__fesetround:
	LPAD(0)
	fsrm t0, a0
	li a0, 0
	ret

.global fegetenv
.type fegetenv, %function
fegetenv:
	LPAD(0)
	frcsr t0
	sw t0, 0(a0)
	li a0, 0
	ret

.global fesetenv
.type fesetenv, %function
fesetenv:
	LPAD(0)
	li t2, -1
	li t1, 0
	beq a0, t2, 1f
	lw t1, 0(a0)
1:	fscsr t1
	li a0, 0
	ret

#endif

RISCV_ZICFILP_NOTE_GNU_PROPERTY_SECTION
