
make:
	gcc -o notebook src/main.c src/file.c src/dynaArray.c src/btree.c

clean:
	rm -f notebook