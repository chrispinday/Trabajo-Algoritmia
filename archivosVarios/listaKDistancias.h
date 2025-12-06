#ifndef LISTAKDISTANCIAS_H
#define LISTAKDISTANCIAS_H
#include <stdbool.h>


typedef struct tipoElementoLista{
	float distancia;
	bool clase;
	int indice;
}tipoElementoLista;

typedef struct celdaListaKDistancias{
	tipoElementoLista elem;
	struct celdaListaKDistancias *sig;
	struct celdaListaKDistancias *ant;
}celdaListaKDistancias;

typedef struct tipoListaKDIstancias{
	celdaListaKDistancias *ini;
	celdaListaKDistancias *fin;
	int numEl;
	int maxEl;
	int validos;
}tipoListaKDIstancias;


tipoElementoLista elemMasCercano(tipoListaKDIstancias l);
void imprimeResultado(tipoListaKDIstancias l);

bool devuelvePrediccion(tipoListaKDIstancias l);

void nuevaLista(tipoListaKDIstancias *, int);

void vaciarLista(tipoListaKDIstancias *);

void insertar(tipoListaKDIstancias *, tipoElementoLista);

void eliminarMayor(tipoListaKDIstancias *);

bool esNulaLista(tipoListaKDIstancias);

void imprimirLista (tipoListaKDIstancias);

#endif 
