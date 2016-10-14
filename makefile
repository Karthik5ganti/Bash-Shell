all: bash

bash: kernel.o echo.o pinfo.o redirect.o pipe1.o 
	gcc kernel.o echo.o pinfo.o redirect.o pipe1.o -o kernel
	rm -rf *o
kernel.o: kernel.c 
	  gcc -c kernel.c

echo.o: echo.c
	gcc -c echo.c

pipe1.o: pipe1.c
	gcc -c pipe1.c

redirect.o: redirect.c
	gcc -c redirect.c

pinfo.o: pinfo.c
	gcc -c pinfo.c

clean:
	rm -rf *o bash

printf("Executable : kernel\n");
