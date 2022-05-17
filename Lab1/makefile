CC = gcc
CFLAGS = -Wall -lm
outLab = lab1
outVis = vis
depLab = Funciones.o lab1.o
depCal = Funciones.o vis.o
source = lab1.c vis.c Funciones.c
salidas=$(outLab) $(outVis)
objects = $(source:.c=.o)
all: $(salidas)
$(outVis): $(depCal)
	$(CC) -o $@ $^ $(CFLAGS)
$(outLab): $(depLab)
	$(CC) -o $@ $^ $(CFLAGS)

$(objects): %.o:%.c
	$(CC) -c $*.c

run:
	./lab1 -i test10000.csv -o salida.txt -d 4 -n 100 -b

clean:
	rm -f $(outVis) $(outLab) $(objects)
