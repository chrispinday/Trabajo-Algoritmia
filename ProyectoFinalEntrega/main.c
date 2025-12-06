#include <stdio.h>
#include <stdlib.h>
#include "listaDinamicaDeDias.h"
#include "fragmenta.h"
#include "normalizacion.h"
#include "distancia.h"
#include "listaKDistancias.h"


// Clasifica un dia usando K=1 el vecino más cercano y devuelve el índice la distancia mínima y la prediccion
void ejecutarK1(tipoListaDinamicaDeDias *lista, tipoDia diaObjetivo, celdaListaDeDias *celdaAIgnorar, int *mejorIndice, float *minDistancia, bool *clasePredicha) {

    celdaListaDeDias *recorrido = lista->ini;
    *minDistancia = 100000.0;
    *mejorIndice = -1;
    int i = 1;

    while (recorrido != NULL) {
        if (recorrido != celdaAIgnorar) { //para no comaprrse consigo mismo

            float dist = calcularDistancia(diaObjetivo, recorrido->elem);

            if (dist < *minDistancia) {
                *minDistancia = dist;
                *clasePredicha = recorrido->elem.rainTomorrow;
                *mejorIndice = i;
            }
        }
        recorrido = recorrido->sig;
        i++;
    }
}


void ejecutarKN(tipoListaDinamicaDeDias *lista, tipoDia diaObjetivo, celdaListaDeDias *celdaAIgnorar, int K, int *idxMejor, float *minDist, bool *prediccion) {

    //lista k vecinos mas cercanos
    tipoListaKDIstancias kLista;
    nuevaLista(&kLista, K);

    celdaListaDeDias *recorrido = lista->ini;
    int i = 1;

    //Se recorre todo el dataset
    while (recorrido != NULL) {
        if (recorrido != celdaAIgnorar) {

            float dist = calcularDistancia(diaObjetivo, recorrido->elem);

            //elemento candidato
            tipoElementoLista candidato;
            candidato.distancia = dist;
            candidato.clase = recorrido->elem.rainTomorrow;
            candidato.indice = i;

            // Intentamos insertar
            insertar(&kLista, candidato);
        }
        recorrido = recorrido->sig;
        i++;
    }

    //resultados
    *prediccion = devuelvePrediccion(kLista);

    //vecino mas cercano al inciop de la lista
    tipoElementoLista mejor = elemMasCercano(kLista);
    *minDist = mejor.distancia;
    *idxMejor = mejor.indice;

    //limpiar lista
    vaciarLista(&kLista);
}

int algoritmoWilson(tipoListaDinamicaDeDias listaDeDias, tipoListaDinamicaDeDias *listaDeWilson, int k) {

    celdaListaDeDias *celdaActual = listaDeDias.ini;
    int conservados = 0;
    int totalProcesados = 0;

    //nueva lista dinamica
    nuevaListaDinamicaDeDias(listaDeWilson);

    //recorrer lista original
    while (celdaActual != NULL) {

        bool prediccion;
        int mejor;    //variable innecesaria oara el proceso de wilson
        float distancia; //variable innecesaria oara el proceso de wilson

        //celda celdaActual pasa como celda a ignorar
        ejecutarKN(&listaDeDias, celdaActual->elem, celdaActual, k, &mejor, &distancia, &prediccion);

        // Si los vecinos aciertan la clase real es bueno insertamos en nuestra lista sino no se guarda (Algoritmo Wilson)
        if (prediccion == celdaActual->elem.rainTomorrow) {
            insertarListaDinamicaDeDias(listaDeWilson, celdaActual->elem);
            conservados++;
        }

        celdaActual = celdaActual->sig;
        totalProcesados++;
    }

    printf("\n");
    return conservados;
}

int main() {
    tipoListaDinamicaDeDias listaDeDias;
    FILE *dataset;
    char *stringDia;
    tipoDia dia;
    int numeroDeDias = 0;
    int opcion;


    // 1 inicializar la lista
    nuevaListaDinamicaDeDias(&listaDeDias);

    // 2 abrir el archivo CSV
    dataset = fopen("dataset_final.csv", "r");
    if (dataset == NULL) {
        perror("Error: No se encuentra 'weatherAUS_limpio.csv'");
        return -1;
    }
    // reservar memoria para leer líneas
    stringDia = (char*)malloc(256 * sizeof(char));

    // saltar primer afila (nombre columnas)
    fscanf(dataset, " %[^\n]", stringDia);

    // cargar Datos
    printf("Cargando datos...\n");
    while (fscanf(dataset, " %[^\n]", stringDia) != EOF) {
        dia = fragmenta(stringDia);
        insertarListaDinamicaDeDias(&listaDeDias, dia);
        numeroDeDias++;
    }
    fclose(dataset);
    printf("Datos cargados: %d dias.\n", numeroDeDias);

    // 4 NORMALIZACIÓN
    // Esto transforma todos los datos a rango 0-1
    normalizarDataset(&listaDeDias);
    printf("Datos normalizados correctamente.\n");


    //MENU PRINCIPAL
    do {
        printf("\n------- MENU K-NN  -------- \n");
        printf("1 - Introducir un dia nuevo y clasificarlo (K=1)\n");
        printf("2 - Comparar todos con todos K = 1 // Limite 1000\n");
        printf("3 - Introducir un dia nuevo y clasificarlo (K=N)\n");
        printf("4 - Comparar todos con todos K = N // Limite 1000\n");
        printf("5 - Aplicar algoritmo de wilson\n");
        printf("6 - Evolucion de Accuracy Sin Wilson\n");
        printf("7 - Evolucion de Accuracy Con Wilson \n");
        printf("8 - Salir\n");
        printf("Escoja una opcion: ");
        scanf("%d", &opcion);

        if (opcion == 1) {
            printf("\nIntroduzca los datos del dia (CSV):\n");
            printf("Formato: MinTemp,MaxTemp,Rainfall,WindGustSpeed,Hum9am,Pres9am,Hum3pm,Pres3pm,RainToday,RainTomorrow\n");
            printf("Ejemplo: 13.4,22.9,0.6,44.0,71.0,1007.7,22.0,1007.1,0,0\n");
            printf("> ");
            scanf("%s", stringDia);


            tipoDia nuevoDia = fragmenta(stringDia);
            celdaListaDeDias celda;
            celda.elem = nuevoDia;
            normalizarCelda(&listaDeDias, &celda);
            tipoDia nuevoDiaNorm = celda.elem;

            // Recorrer toda la lista para buscar el vecino más cercano
            int mejorIndice;
            float distanciaMin;
            bool prediccion;

            //funcion compra
            ejecutarK1(&listaDeDias, nuevoDiaNorm, NULL, &mejorIndice, &distanciaMin, &prediccion);

            printf("\n--- RESULTADO ---\n");
            printf("Vecino mas cercano: Indice %d (Distancia: %.4f)\n", mejorIndice, distanciaMin);

            char* predStr = prediccion ? "LLUEVE" : "NO LLUEVE";
            char* realStr = nuevoDia.rainTomorrow ? "LLUEVE" : "NO LLUEVE";

            printf("Prediccion: %s\n", predStr);
            printf("Realidad:   %s\n", realStr);

            if (prediccion == nuevoDia.rainTomorrow) {
                printf(">> CORRECTO\n");
            } else {
                printf(">> INCORRECTO\n");
            }
        }
        else if (opcion == 2) {
            int LIMITE_PRUEBA = 1000; //Limite 1000
            printf("\nComparar todos con todos, k=1 // Calculando Matriz de Confusión\n");

            int TP = 0;
            int TN = 0;
            int FP = 0;
            int FN = 0;

            int aciertos = 0;
            int totalEvaluados = 0;
            celdaListaDeDias *actual = listaDeDias.ini;

            int idx;
            float dist;
            bool pred;

            while(actual != NULL && totalEvaluados < LIMITE_PRUEBA) {
                ejecutarK1(&listaDeDias, actual->elem, actual, &idx, &dist, &pred);

                if (pred == actual->elem.rainTomorrow) {
                    aciertos++;
                }

                bool real = actual->elem.rainTomorrow;

                if (real == 1 && pred == 1) {
                    TP++; // Llovio y acertamos
                }
                else if (real == 0 && pred == 0) {
                    TN++; // No llovio y acertamos
                }
                else if (real == 0 && pred == 1) {
                    FP++; // Dijimos que llovia pero no
                }
                else if (real == 1 && pred == 0) {
                    FN++; // Dijimos que no llovia pero llovio
                }

                totalEvaluados++;
                actual = actual->sig; // Pasamos al siguiente dia


            }

            float accuracy = (float)(TP + TN) / totalEvaluados;
            float sensibilidad = (float)TP / (TP + FN);
            float especificidad = (float)TN / (TN + FP);
            float precision = (float)TP / (TP + FP);

            //RESULTADOS
            printf("\n\n");
            printf("                                 MATRIZ DE CONFUSIÓN            \n");
            printf("                       | Pred: SI Llueve| Pred: NO Llueve |\n");
            printf(" Real: SI Llueve (1)   |       %d      |        %d      |\n", TP, FN);
            printf(" Real: NO Llueve (0)   |       %d       |        %d      |\n", FP, TN);
            printf("\n\n");

            printf("MÉTRICAS DETALLADAS\n");
            printf("1. Accuracy (Exactitud):   %.2f%%\n", accuracy * 100.0);
            printf("2. Sensibilidad (Recall):  %.2f%%\n", sensibilidad * 100.0);
            printf("3. Especificidad:          %.2f%%\n", especificidad * 100.0);
            printf("4. Precision:              %.2f%%\n", precision * 100.0);
            printf("Total Evaluados: %d\n", totalEvaluados);

        }

        else if (opcion == 3) {
            int K;
            printf("Introduzca el valor de K: ");
            scanf("%d", &K);

            printf("\nIntroduzca los datos del dia (CSV):\n");
            printf("Formato: MinTemp,MaxTemp,Rainfall,WindGustSpeed,Hum9am,Pres9am,Hum3pm,Pres3pm,RainToday,RainTomorrow\n");
            printf("Ejemplo: 13.4,22.9,0.6,44.0,71.0,1007.7,22.0,1007.1,0,0\n");
            printf("> ");
            scanf("%s", stringDia);

            tipoDia nuevoDia = fragmenta(stringDia);
            celdaListaDeDias celdaTemp;
            celdaTemp.elem = nuevoDia;
            normalizarCelda(&listaDeDias, &celdaTemp);

            int mejorIndice;
            float minDistancia;
            bool prediccion;

            // Ejecutar algoritmo K=N
            ejecutarKN(&listaDeDias, celdaTemp.elem, NULL, K, &mejorIndice, &minDistancia, &prediccion);

            printf("\nVecino mas cercano del grupo: Indice %d (Dist: %.4f)\n", mejorIndice, minDistancia);
            printf("Prediccion (K=%d): %s\n", K, prediccion ? "LLUEVE" : "NO LLUEVE");
            printf("Realidad: %s\n", nuevoDia.rainTomorrow ? "LLUEVE" : "NO LLUEVE");
        }

        else if (opcion == 4) {
            int K;
            printf("Introduzca el valor de K: ");
            scanf("%d", &K);

            printf("\nComparar todos con todos, K = %d // Calculando Matriz de Confusión\n",K);

            int LIMITE = 1000;
            int TP=0;
            int TN=0;
            int FP=0;
            int FN=0;

            celdaListaDeDias *actual = listaDeDias.ini;
            int totalEvaluados = 0;

            while(actual != NULL && totalEvaluados < LIMITE) {
                int idx; float dist; bool pred;

                // Pasamos 'actual' para ignorarlo
                ejecutarKN(&listaDeDias, actual->elem, actual, K, &idx, &dist, &pred);

                bool real = actual->elem.rainTomorrow;

                if (real == 1 && pred == 1) {
                    TP++; // Llovio y acertamos
                }
                else if (real == 0 && pred == 0) {
                    TN++; // No llovio y acertamos
                }
                else if (real == 0 && pred == 1) {
                    FP++; // Dijimos que llovia pero no
                }
                else if (real == 1 && pred == 0) {
                    FN++; // Dijimos que no llovia pero llovio
                }

                actual = actual->sig;
                totalEvaluados++;

            }

            float accuracy = (float)(TP + TN) / totalEvaluados;
            float sensibilidad = (float)TP / (TP + FN);
            float especificidad = (float)TN / (TN + FP);
            float precision = (float)TP / (TP + FP);

            //RESULTADOS
            printf("\n\n");
            printf("                                 MATRIZ DE CONFUSIÓN            \n");
            printf("                       | Pred: SI Llueve| Pred: NO Llueve |\n");
            printf(" Real: SI Llueve (1)   |       %d      |        %d      |\n", TP, FN);
            printf(" Real: NO Llueve (0)   |       %d       |        %d      |\n", FP, TN);
            printf("\n\n");

            printf("MÉTRICAS DETALLADAS\n");
            printf("1. Accuracy (Exactitud):   %.2f%%\n", accuracy * 100.0);
            printf("2. Sensibilidad (Recall):  %.2f%%\n", sensibilidad * 100.0);
            printf("3. Especificidad:          %.2f%%\n", especificidad * 100.0);
            printf("4. Precision:              %.2f%%\n", precision * 100.0);
            printf("Total Evaluados: %d\n", totalEvaluados);

        }

		else if (opcion == 5) {
            int K;
            printf("\n--- ALGORITMO DE WILSON ---\n");
            printf("Introduzca el valor de K: ");
            scanf("%d", &K);

            tipoListaDinamicaDeDias listaFiltrada;

            // Ejecutar Wilson
            printf("Ejecutando Wilson (K=%d)...", K);
            int conservados = algoritmoWilson(listaDeDias, &listaFiltrada, K);

            printf("\n--- RESULTADOS DE REDUCCION ---\n");
            printf("Originales:   %d\n", numeroDeDias);
            printf("Conservados:  %d\n", conservados);
            printf("Eliminados:   %d (Ruido)\n", numeroDeDias - conservados);

            vaciarListaDias(&listaFiltrada);
        }

		 else if (opcion == 6) {
            int maxK;
            printf("\n--- ESTUDIO DE EVOLUCION SIN WILSON ---\n");
            printf("Ingrese el valor maximo de K: ");
            scanf("%d", &maxK);

            printf("\nCalculando evolucion de K=1 a K=%d \n\n", maxK);
            printf(" K  | Accuracy\n");
            printf("----|----------\n");

            float mejorAcc = 0.0;
            int mejorK = 0;

            // bucle desde k= 1 hasta el k maximo establecido
            for (int k_actual = 1; k_actual <= maxK; k_actual++) {

                int aciertos = 0;
                int total = 0;
                celdaListaDeDias *actual = listaDeDias.ini;

                while(actual != NULL) {
                    int idx; float dist; bool pred;
                    if (k_actual == 1)
                        ejecutarK1(&listaDeDias, actual->elem, actual, &idx, &dist, &pred);
                    else
                        ejecutarKN(&listaDeDias, actual->elem, actual, k_actual, &idx, &dist, &pred);

                    if (pred == actual->elem.rainTomorrow) {
                        aciertos++;
                    }
                    total++;
                    actual = actual->sig;
                }

                float accuracy = (float)aciertos / total * 100.0;

                // guardar el mejor
                if (accuracy > mejorAcc) {
                    mejorAcc = accuracy;
                    mejorK = k_actual;
                }

                // resultado
                printf(" %2d |  %.2f%%\n", k_actual, accuracy);
            }

            printf("----|----------\n");
            printf("\n>> CONCLUSION:\n");
            printf("   Mejor K: %d (Accuracy: %.2f%%)\n", mejorK, mejorAcc);
        }


        else if (opcion == 7) {
            int maxK, k_wilson;
            printf("\n--- ESTUDIO DE EVOLUCION CON WILSON ---\n");
            printf("1. Primero filtramos con Wilson (K Fijo)\n");
            printf("2. Luego probamos Accuracy con K variable (1..MaxK)\n");

            printf("Introduzca K para Wilson: ");
            scanf("%d", &k_wilson);

            printf("Introduzca Max K para clasificacion: ");
            scanf("%d", &maxK);

            //filtrar con wilson
            tipoListaDinamicaDeDias listaFiltrada;
            int conservados = algoritmoWilson(listaDeDias, &listaFiltrada, k_wilson);

            printf("\n--- RESULTADOS DE REDUCCION ---\n");
            printf("Originales:   %d\n", numeroDeDias);
            printf("Conservados:  %d\n", conservados);
            printf("Eliminados:   %d (Ruido)\n", numeroDeDias - conservados);

            // PASO 2: Evolución de accuracy
            printf("\nCalculando evolucion K=1 a K=%d \n", maxK);
            printf(" K  | Accuracy (Wilson K=%d)\n", k_wilson);
            printf("----|----------------------\n");

            float mejorAcc = 0.0;
            int mejorK = 0;


            //en este caso usaremos lal ista filtrada por el algoritmo de wilson
            for (int k_clasif = 1; k_clasif <= maxK; k_clasif++) {
                int aciertos = 0;
                int total = 0;
                celdaListaDeDias *actual = listaDeDias.ini;

                while(actual != NULL) {
                    int idx; float dist; bool pred;
                    if (k_clasif == 1)
                        ejecutarK1(&listaFiltrada, actual->elem, NULL, &idx, &dist, &pred);
                    else
                        ejecutarKN(&listaFiltrada, actual->elem, NULL, k_clasif, &idx, &dist, &pred);

                    if (pred == actual->elem.rainTomorrow) aciertos++;

                    total++;
                    actual = actual->sig;
                }

                float accuracy = (float)aciertos / total * 100.0;
                if (accuracy > mejorAcc) { mejorAcc = accuracy; mejorK = k_clasif; }
                printf(" %2d |  %.2f%%\n", k_clasif, accuracy);
            }

            printf("----|----------------------\n");
            printf("Mejor K Clasif: %d (Accuracy: %.2f%%) [Usando Wilson K=%d]\n", mejorK, mejorAcc, k_wilson);

            vaciarListaDias(&listaFiltrada);
        }

    } while (opcion != 8);


    free(stringDia);
    vaciarListaDias(&listaDeDias);

    return 0;
}


