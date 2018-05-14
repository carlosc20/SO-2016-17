CC=gcc

bash:
	$(CC) -o Bash main.c file.c struct.c

clean:
	rm Bash