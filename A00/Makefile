hello : hello.c
	gcc hello.c -o hello

fortune: fortune.c
	gcc fortune.c -o fortune

compileAll: hello.c fortune.c
	for file in $(ls *.c); do echo gcc $(file); done

clean :
	rm hello
	rm fortune