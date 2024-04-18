#include "feature_1_and_arch.h"

#if USE_FEATURE_1_AND != 0

#include <sys/prctl.h>
#include "feature_1_and.h"
#include "syscall.h"

int __handle_feature_1_and_arch(const uint32_t feature_1_and)
{
	int err = 0;
#ifdef __riscv_zicfilp
	if (feature_1_and & (GNU_PROPERTY_RISCV_FEATURE_1_CFI_LP_SIMPLE |
	                     GNU_PROPERTY_RISCV_FEATURE_1_CFI_LP_FUNC_SIG))
		err = syscall(SYS_prctl, PR_SET_INDIR_BR_LP_STATUS, PR_INDIR_BR_LP_ENABLE);
#endif // #ifdef __riscv_zicfilp
	return err;
}

#endif // #if USE_FEATURE_1_AND != 0
