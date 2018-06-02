make:
	gcc -o notebook main.c file.c dynaArray.c btree.c

clean:
	rm notebook