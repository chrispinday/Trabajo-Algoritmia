#include "listaKDistancias.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

void nuevaLista(tipoListaKDIstancias* l,int k){
	l->maxEl = k;
	l->numEl = 0;
	l->validos = 0;
	l->ini = NULL;
	l->fin = NULL;
}

void vaciarLista(tipoListaKDIstancias *l){
	celdaListaKDistancias *aux = l->fin;
	while(aux != NULL){
		l->fin = l->fin->ant;
		free(aux);
		aux = l->fin;
	}
	l->ini = NULL;
	l->numEl = 0;
	l->validos = 0;
}

void insertar(tipoListaKDIstancias* l, tipoElementoLista e){
	if(l->numEl == l-> maxEl){// si lsita llena
		if(e.distancia<l->fin->elem.distancia){ //si el nuevo es mejor que el peor q teniamos
			if(l->fin->elem.clase){
				l->validos --;
			}
			eliminarMayor(l); //se elimian al peor
			l->numEl--;
			insertar(l, e); //recursividad se inserta el nuevo en el elseif de abajo
		}

	}
	else if(l->numEl<l->maxEl){ //tiene hueco
			l->numEl ++;
			celdaListaKDistancias* nuevo;
			nuevo = (celdaListaKDistancias*)malloc(sizeof(celdaListaKDistancias));
			nuevo->elem = e;
			if(e.clase){
				l->validos ++;
			}
			if(esNulaLista(*l)){ //lista vacía entra primero
				nuevo->ant = NULL;
				nuevo->sig = NULL;
				l->ini = nuevo;
				l->fin = nuevo;
			}
			else if(l->ini == l->fin){ //solo hay 1 se coloca antes o despues
				if(l->ini->elem.distancia > e.distancia){
					l->ini->sig = NULL;
					nuevo->ant = NULL;
					nuevo->sig = l->ini;
					l->ini = nuevo;
					nuevo->sig->ant = nuevo;
				}
				else if(l->ini->elem.distancia <= e.distancia){
					nuevo->sig = NULL;
					nuevo->ant = l->ini;
					l->fin = nuevo;
					nuevo->ant->sig = nuevo;
				}
			}
			else{ //mas de 1 elemento
				if(nuevo->elem.distancia <= l->ini->elem.distancia){//menor que el primero
					nuevo->ant = NULL;
					nuevo->sig = l->ini;
					l->ini = nuevo;
					nuevo->sig->ant = nuevo;
				}
				else if(nuevo->elem.distancia >= l->fin->elem.distancia){//mayor que el ultimo
					nuevo->sig = NULL;
					nuevo->ant = l->fin;
					l->fin = nuevo;
					nuevo->ant->sig = nuevo;
				}
				else{//en medio de la lista
					nuevo->sig = l->ini;
 					while(nuevo->elem.distancia >= nuevo->sig->elem.distancia){
	 					nuevo->ant = nuevo->sig;
 						nuevo->sig = nuevo->sig->sig;
 					}
 					nuevo->ant->sig = nuevo;
					nuevo->sig->ant = nuevo;
				}
			}
		}
}


tipoElementoLista elemMasCercano(tipoListaKDIstancias l){
	return l.ini->elem; //devuelve distancia, indice y clase
}


void imprimeResultado(tipoListaKDIstancias l){
	bool prediction;
	printf("Closest example e : %d\n",l.ini->elem.indice);
	printf("Min distance %f\n",l.ini->elem.distancia);
	if(l.validos>(l.maxEl/2)){
		prediction = true;
	}else if(l.validos==(l.maxEl/2)){
		if (elemMasCercano(l).clase){
			prediction = true;
		}else{
			prediction = false;
		}
	}else{
		prediction = false;
	}
	printf("prediction: Class = %d\n", prediction);
	//printf("Prediction Success = %d\n",devuelvePrediccion(l));
}

bool devuelvePrediccion(tipoListaKDIstancias l){
	if(l.validos>(l.maxEl/2)){ //mayoria
		return true;
	}else if(l.validos==(l.maxEl/2)){ //empate se escoge la decision del mas cercano
		if (elemMasCercano(l).clase){
			return true;
		}else{
			return false;
		}
	}else{ //minoria
		return false;
	}
}

void eliminarMayor(tipoListaKDIstancias* l) {
    if (l->ini == l->fin) {
        free(l->ini);
        l->ini = NULL;
        l->fin = NULL;
    } else {
        celdaListaKDistancias* temp = l->fin;
        l->fin = l->fin->ant;
        free(temp);
        l->fin->sig = NULL;
    }
}



bool esNulaLista(tipoListaKDIstancias l){
	return(l.ini == NULL);
}

void imprimirLista (tipoListaKDIstancias l){
	celdaListaKDistancias* aux = l.ini;
	while(aux != l.fin){
		printf("%lf ", aux->elem.distancia);
		aux = aux->sig;
	}
	printf("%lf\n", aux->elem.distancia);
}
