all:opacue_c.exe

opacue.out:test_opacue.c point.c
	g++ test_opacue.c point.c -o $@
opacue_c.exe:test_opacue.c point.c
	#will report warning :point.c:13: warning: useless storage class specifier in empty declaration
	gcc test_opacue.c point.c -o $@
