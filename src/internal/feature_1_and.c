#include "feature_1_and_arch.h"

#if USE_FEATURE_1_AND != 0

#include <elf.h>
#include <stdint.h>
#include <string.h>
#include "feature_1_and.h"

#if UINTPTR_MAX == 0xffffffff
#define IS_64_BIT (0)
typedef Elf32_Ehdr Ehdr;
typedef Elf32_Phdr Phdr;
#else
#define IS_64_BIT (1)
typedef Elf64_Ehdr Ehdr;
typedef Elf64_Phdr Phdr;
#endif

uint32_t __feature_1_and = 0xffffffff;

uint32_t __parse_feature_1_and(const void * const elf_base)
{
	typedef struct {
		uint32_t pr_type;
		uint32_t pr_datasz;
		unsigned char pr_data[];
	} Elf_Prop;
	typedef struct {
		uint32_t n_namsz;
		uint32_t n_descsz;
		uint32_t n_type;
		char n_name[4];
		Elf_Prop n_desc[];
	} GnuPropertyNote;

	const Ehdr * const eh = elf_base;
	const Phdr *phdr = (char *)(elf_base) + eh->e_phoff;
	const Phdr *ph;
	for (ph = (char *)(phdr) + eh->e_phnum * eh->e_phentsize - eh->e_phentsize;
	     ph >= phdr;
	     ph = (char *)(ph) - eh->e_phentsize) {
		if (ph->p_type != PT_GNU_PROPERTY) continue;

		/* The following assumes that there's only one PT_GNU_PROPERTY segment in ELF */
		const GnuPropertyNote * const note = (char *)(elf_base) + ph->p_offset;
		const Elf_Prop *prop = note->n_desc;
		const void * const prop_end = (char *)(note->n_desc) + note->n_descsz;
		while ((void *)(prop) < prop_end) {
			if (prop->pr_type == GNU_PROPERTY_FEATURE_1_AND_TYPE) {
				return *(uint32_t *)(prop->pr_data);
			}
#if IS_64_BIT != 0
			prop = (Elf_Prop *)((uintptr_t)(prop->pr_data + prop->pr_datasz + (8 - 1)) & -8);
#else
			prop = (Elf_Prop *)((uintptr_t)(prop->pr_data + prop->pr_datasz + (4 - 1)) & -4);
#endif // #if IS_64_BIT != 0
		}
		return 0;
	}
	return 0;
}

#endif // #if USE_FEATURE_1_AND != 0
