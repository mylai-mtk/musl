#if defined(__riscv_zicfilp) && defined(__riscv_landing_pad_func_sig)
int invoke_main(int argc, char **argv, char **envp,
                int (*main)(int,char **,char **));
__asm__(
".hidden invoke_main\n"
".type invoke_main, %function\n"
"invoke_main:\n"
"\tlui t2, 0xd0639\n" // Set CFI label for main function
"\tjr a3\n"
);
#else // #if defined(__riscv_zicfilp) && defined(__riscv_landing_pad_func_sig)
__attribute__((__visibility__("hidden")))
inline int invoke_main(int argc, char **argv, char **envp,
                       int (*main)(int,char **,char **)) {
	return main(argc, argv, envp);
}
#endif // #if defined(__riscv_zicfilp) && defined(__riscv_landing_pad_func_sig)
