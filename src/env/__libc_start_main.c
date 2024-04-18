#include <elf.h>
#include <poll.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include "syscall.h"
#include "atomic.h"
#include "libc.h"
#include "feature_1_and_arch.h"
#if USE_FEATURE_1_AND != 0
#include "feature_1_and.h"
#endif // #if USE_FEATURE_1_AND != 0

static void dummy(void) {}
weak_alias(dummy, _init);

extern weak hidden void (*const __init_array_start)(void), (*const __init_array_end)(void);

static void dummy1(void *p) {}
weak_alias(dummy1, __init_ssp);

#define AUX_CNT 38

#ifdef __GNUC__
__attribute__((__noinline__))
#endif
void __init_libc(char **envp, char *pn)
{
	size_t i, *auxv, aux[AUX_CNT] = { 0 };
	__environ = envp;
	for (i=0; envp[i]; i++);
	libc.auxv = auxv = (void *)(envp+i+1);
	for (i=0; auxv[i]; i+=2) if (auxv[i]<AUX_CNT) aux[auxv[i]] = auxv[i+1];
	__hwcap = aux[AT_HWCAP];
	if (aux[AT_SYSINFO]) __sysinfo = aux[AT_SYSINFO];
	libc.page_size = aux[AT_PAGESZ];

	if (!pn) pn = (void*)aux[AT_EXECFN];
	if (!pn) pn = "";
	__progname = __progname_full = pn;
	for (i=0; pn[i]; i++) if (pn[i]=='/') __progname = pn+i+1;

	__init_tls(aux);
	__init_ssp((void *)aux[AT_RANDOM]);

	if (aux[AT_UID]==aux[AT_EUID] && aux[AT_GID]==aux[AT_EGID]
		&& !aux[AT_SECURE]) return;

	struct pollfd pfd[3] = { {.fd=0}, {.fd=1}, {.fd=2} };
	int r =
#ifdef SYS_poll
	__syscall(SYS_poll, pfd, 3, 0);
#else
	__syscall(SYS_ppoll, pfd, 3, &(struct timespec){0}, 0, _NSIG/8);
#endif
	if (r<0) a_crash();
	for (i=0; i<3; i++) if (pfd[i].revents&POLLNVAL)
		if (__sys_open("/dev/null", O_RDWR)<0)
			a_crash();
	libc.secure = 1;
}

#if USE_FEATURE_1_AND != 0
#if UINTPTR_MAX == 0xffffffff
typedef Elf32_Phdr Phdr;
typedef Elf32_auxv_t auxv_t;
#else
typedef Elf64_Phdr Phdr;
typedef Elf64_auxv_t auxv_t;
#endif // #if UINTPTR_MAX == 0xffffffff

#ifndef __PIE__
extern weak hidden char __ehdr_start[];

static const void * find_base_addr(const auxv_t * const auxv)
{
	if (__ehdr_start) return __ehdr_start;

	const Phdr *ph = (void*)(auxv[AT_PHDR].a_un.a_val);
	const size_t phent = auxv[AT_PHENT].a_un.a_val;
	const size_t phnum = auxv[AT_PHNUM].a_un.a_val;
	const Phdr *ph_end = (char *)(ph) + phent * phnum;
	for (; ph < ph_end; ph = (char *)(ph) + phent) {
		if (ph->p_type == PT_PHDR)
			return (void *)(auxv[AT_PHDR].a_un.a_val - ph->p_vaddr);
	}
	return NULL;
}
#endif // #ifndef __PIE__

static int handle_feature_1_and(const auxv_t * const auxv)
{
#ifdef __PIE__
	// For libc.so, the feature_1_and is already parsed in dynlink.c
	(void)(auxv);
#else
	const void * const base_addr = find_base_addr(auxv);
	if (!base_addr) return -1;
	__feature_1_and &= __parse_feature_1_and(base_addr);
#endif // #ifndef __PIE__

	return __handle_feature_1_and_arch(__feature_1_and);
}
#endif // #if USE_FEATURE_1_AND != 0

static void libc_start_init(void)
{
	_init();
	uintptr_t a = (uintptr_t)&__init_array_start;
	for (; a<(uintptr_t)&__init_array_end; a+=sizeof(void(*)()))
		(*(void (**)(void))a)();
}

weak_alias(libc_start_init, __libc_start_init);

typedef int lsm2_fn(int (*)(int,char **,char **), int, char **);
static lsm2_fn libc_start_main_stage2;

int __libc_start_main(int (*main)(int,char **,char **), int argc, char **argv,
	void (*init_dummy)(), void(*fini_dummy)(), void(*ldso_dummy)())
{
	char **envp = argv+argc+1;

	/* External linkage, and explicit noinline attribute if available,
	 * are used to prevent the stack frame used during init from
	 * persisting for the entire process lifetime. */
	__init_libc(envp, argv[0]);

#if USE_FEATURE_1_AND != 0
	if (handle_feature_1_and(libc.auxv))
		a_crash();
#endif // #if USE_FEATURE_1_AND != 0

	/* Barrier against hoisting application code or anything using ssp
	 * or thread pointer prior to its initialization above. */
	lsm2_fn *stage2 = libc_start_main_stage2;
	__asm__ ( "" : "+r"(stage2) : : "memory" );
	return stage2(main, argc, argv);
}

static int libc_start_main_stage2(int (*main)(int,char **,char **), int argc, char **argv)
{
	char **envp = argv+argc+1;
	__libc_start_init();

	/* Pass control to the application */
	exit(main(argc, argv, envp));
	return 0;
}
