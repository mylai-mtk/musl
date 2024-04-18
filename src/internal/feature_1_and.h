#ifndef _INTERNAL_FEATURE_1_AND_H
#define _INTERNAL_FEATURE_1_AND_H

#include <stdint.h>

extern uint32_t __feature_1_and;

uint32_t __parse_feature_1_and(const void * const elf_base);
int __handle_feature_1_and_arch(const uint32_t feature_1_and);

#endif // #ifndef _INTERNAL_FEATURE_1_AND_H
