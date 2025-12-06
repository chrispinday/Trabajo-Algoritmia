                    MANUAL DE USO - CLASIFICADOR K-NN

DESCRIPCIÓN DEL PROGRAMA

Este proyecto implementa un clasificador K-NN (K vecinos mas cercanos) en lenguaje C
para predecir si lloverá mañana en Australia, basandose en datos meteorologicos
historitcos

Incluye funcionalidades avanzadas como:

Carga de datos desde CSV
Normalización automática de atributos (Min-Max)
Clasificación con K=1 y K=N.
Matriz de Confusión Accuracy Sensibilidad Especificidad
Limpieza de datos mediante el Algoritmo de Wilson
Estudio evolutivo de la precisión segun el valor de K

ARCHIVOS NECESARIOS
main.c               
listaDinamicaDeDias.c 
listaDinamicaDeDias.h
fragmenta.c          
fragmenta.h
normalizacion.c       
normalizacion.h
distancia.c           
distancia.h
listaKDistancias.c    
listaKDistancias.h
dataset_final.csv    
MAKEFILE

INSTRUCCIONES DE COMPILACIÓN
Abrir TERMINAL en la carpeta del proyecto y ejecutar:

>>make all
>>make ejec

o sino:

>>gcc -g -o main main.c normalizacion.o fragmenta.o listaDinamicaDeDias.o listaKDistancias.o distancia.o -lm -Wall
>>./main


EXPLICACIÓN DE LAS OPCIONES DEL MENÚ

[1] Introducir un dia nuevo y clasificarlo (K=1)
- Te permite introducir manualmente los datos de un dia
- El programa busca al vecino más cercano en la base de datos
- Predice si lloverá mañana basandose en ese unico vecino

[2] Comparar todos con todos (K=1)
- Intenta predecir la lluvia para CADA dia del dataset usando al resto como entrenamiento
- Muestra la Matriz de Confusión y métricas de rendimiento

[3] Introducir un día nuevo y clasificarlo (K=N)
- Igual que la opción 1 pero te permite elegir el valor de K (ej: 5, 13, 20)

[4] Comparar todos con todos (K=N)
- Igual que la opción 2 pero usando el valor de K que se elija

[5] Aplicar Algoritmo de Wilson
- Ejecuta una limpieza del dataset para eliminar datos atípicos o mal clasificados
- Pide un valor de K (recomendado K=1 o K=2) para decidir qué datos borrar
- Muestra cuántos datos se conservan y cuántos se eliminan
- Importante no borra datos de la lista principal solo aporta datos 

[6] Evolución de Accuracy Sin Wilson
- Realiza un estudio automático probando desde K=1 hasta el K máximo que se indique
- Muestra una tabla con la precisión obtenida para cada K
- Te dice cual es el mejor valor de K

[7] Evolución de Accuracy Con Wilson
- Primero aplica Wilson con una k eligida (opcion5) para limpiar los datos
- Luego usa ese dataset limpio para clasificar los datos originales probando diferentes K.
- Permite ver si la limpieza mejora el rendimiento del modelo.