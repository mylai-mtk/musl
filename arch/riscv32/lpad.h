#ifndef _ARCH_RISCV32_LPAD_H
#define _ARCH_RISCV32_LPAD_H

#if defined(__riscv_zicfilp) && __riscv_zicfilp_label_scheme > 0

#	define GNU_PROPERTY_RISCV_FEATURE_1_AND (0xc0000000)

#	define GNU_PROPERTY_RISCV_FEATURE_1_CFI_LP_SIMPLE   (1 << 0)
#	define GNU_PROPERTY_RISCV_FEATURE_1_CFI_LP_FUNC_SIG (1 << 2)

#	if __riscv_zicfilp_label_scheme == 1
#		define LPAD(label) lpad 0
#		define SETUP_LPAD_LABEL(label)
#		define RISCV_ZICFILP_FEATURE_1_AND \
		       GNU_PROPERTY_RISCV_FEATURE_1_CFI_LP_SIMPLE
#	elif __riscv_zicfilp_label_scheme == 2
#		define LPAD(label) lpad label
#		define SETUP_LPAD_LABEL(label) lui t2, label
#		define RISCV_ZICFILP_FEATURE_1_AND \
		       GNU_PROPERTY_RISCV_FEATURE_1_CFI_LP_FUNC_SIG
#	else
#		error "Unsupported __riscv_zicfilp_label_scheme"
#	endif

#	define RISCV_ZICFILP_NOTE_GNU_PROPERTY_SECTION \
	       .pushsection .note.gnu.property, "a"   ;\
	       .p2align 3                             ;\
	       .word 4                                ;\
	       .word 16                               ;\
	       .word 5                                ;\
	       .asciz "GNU"                           ;\
	       .word GNU_PROPERTY_RISCV_FEATURE_1_AND ;\
	       .word 4                                ;\
	       .word RISCV_ZICFILP_FEATURE_1_AND      ;\
	       .word 0                                ;\
	       .popsection                            ;
#else
#	define LPAD(label)
#	define SETUP_LPAD_LABEL(label)
#	define RISCV_ZICFILP_NOTE_GNU_PROPERTY_SECTION
#endif

#endif // #ifndef _ARCH_RISCV32_LPAD_H
