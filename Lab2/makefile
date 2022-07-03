CC = gcc
CFLAGS = -Wall -lm
outLab = lab2
depLab = Funciones.o lab2.o
source = lab2.c Funciones.c
salidas=$(outLab)
objects = $(source:.c=.o)
all: $(salidas)
$(outLab): $(depLab)
	$(CC) -o $@ $^ $(CFLAGS)

$(objects): %.o:%.c
	$(CC) -c $*.c

run:
	./lab2 -i test10000.csv -o salida.txt -d 100 -n 4 -c 100 -h 4 -b

clear:
	rm -f $(outLab) $(objects)
