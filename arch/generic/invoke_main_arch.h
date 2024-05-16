__attribute__((__visibility__("hidden")))
inline int invoke_main(int argc, char **argv, char **envp,
                       int (*main)(int,char **,char **)) {
	return main(argc, argv, envp);
}
