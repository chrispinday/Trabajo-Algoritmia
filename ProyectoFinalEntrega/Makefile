all: listaDinamicaDeDias fragmenta normalizacion listaKDistancias distancia maestro

listaDinamicaDeDias: listaDinamicaDeDias.c
	gcc -g -c listaDinamicaDeDias.c -Wall

fragmenta: fragmenta.c
	gcc -g -c fragmenta.c -Wall

normalizacion: normalizacion.c
	gcc -g -c normalizacion.c -Wall

listaKDistancias: listaKDistancias.c
	gcc -g -c listaKDistancias.c -Wall

distancia: distancia.c
	gcc -g -c distancia.c -lm -Wall

maestro: main.c normalizacion.o fragmenta.o listaDinamicaDeDias.o listaKDistancias.o distancia.o
	gcc -g -o main main.c normalizacion.o fragmenta.o listaDinamicaDeDias.o listaKDistancias.o distancia.o -lm -Wall

ejec: main
	./main

clean:
	rm -f *.o main
