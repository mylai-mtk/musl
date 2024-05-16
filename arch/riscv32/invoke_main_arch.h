#if defined(__riscv_zicfilp) && __riscv_zicfilp_label_scheme == 2
int invoke_main(int argc, char **argv, char **envp,
                int (*main)(int,char **,char **));
__asm__(
".hidden invoke_main\n"
".type invoke_main, %function\n"
"invoke_main:\n"
"\tlui t2, 0xe088e\n" // Set CFI label for main function
"\tjr a3\n"
);
#else // #if defined(__riscv_zicfilp) && __riscv_zicfilp_label_scheme == 2
__attribute__((__visibility__("hidden")))
inline int invoke_main(int argc, char **argv, char **envp,
                       int (*main)(int,char **,char **)) {
	return main(argc, argv, envp);
}
#endif // #if defined(__riscv_zicfilp) && __riscv_zicfilp_label_scheme == 2
