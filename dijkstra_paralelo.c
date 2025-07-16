#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define INFINITO 1000000

int encontrarNodoMinimo(int *distancias, int *visitado, int cantidad) {
    int minimo = INFINITO;
    int indice = -1;

    #pragma omp parallel
    {
        int minimoLocal = INFINITO;
        int indiceLocal = -1;

        #pragma omp for nowait
        for (int i = 0; i < cantidad; i++) {
            if (!visitado[i]) {
                if (distancias[i] <= minimoLocal) {
                    minimoLocal = distancias[i];
                    indiceLocal = i;
                }
            }
        }

        #pragma omp critical
        {
            if (minimoLocal < minimo) {
                minimo = minimoLocal;
                indice = indiceLocal;
            }
        }
    }

    return indice;
}

void actualizarVecinos(int **grafo, int nodoActual, int *distancias, int *visitado, int cantidad) {
    #pragma omp parallel for
    for (int i = 0; i < cantidad; i++) {
        if (!visitado[i]) {
            if (grafo[nodoActual][i] > 0) {
                int nuevaDistancia = distancias[nodoActual] + grafo[nodoActual][i];
                if (nuevaDistancia < distancias[i]) {
                    distancias[i] = nuevaDistancia;
                }
            }
        }
    }
}

void guardarArchivoSalida(const char *ruta, int *distancias, int cantidad) {
    FILE *archivo = fopen(ruta, "w");
    if (!archivo) {
        perror("Error al abrir archivo");
        exit(EXIT_FAILURE);
    }

    fprintf(archivo, "Nodo\tDistancia desde origen\n");
    for (int i = 0; i < cantidad; i++) {
        if (distancias[i] == INFINITO) {
            fprintf(archivo, "%d\t9999\n", i);
        } else {
            fprintf(archivo, "%d\t%d\n", i, distancias[i]);
        }
    }

    fclose(archivo);
}

void algoritmoDijkstra(int **grafo, int cantidad, int origen, const char *archivoSalida) {
    int *distancias = (int *)malloc(cantidad * sizeof(int));
    int *visitado = (int *)calloc(cantidad, sizeof(int));

    for (int i = 0; i < cantidad; i++) {
        distancias[i] = INFINITO;
    }
    distancias[origen] = 0;

    for (int repeticiones = 0; repeticiones < cantidad - 1; repeticiones++) {
        int nodoMinimo = encontrarNodoMinimo(distancias, visitado, cantidad);
        if (nodoMinimo == -1) {
            break;
        }
        visitado[nodoMinimo] = 1;
        actualizarVecinos(grafo, nodoMinimo, distancias, visitado, cantidad);
    }

    guardarArchivoSalida(archivoSalida, distancias, cantidad);
    free(distancias);
    free(visitado);
}

int **leerMatriz(const char *texto, int *tamanoMatriz) {
    int filasContadas = 0;
    for (const char *p = texto; *p; p++) {
        if (*p == '[') {
            filasContadas++;
        }
    }
    *tamanoMatriz = filasContadas - 1;

    int **matrizCreada = (int **)malloc((*tamanoMatriz) * sizeof(int *));
    for (int i = 0; i < *tamanoMatriz; i++) {
        matrizCreada[i] = (int *)malloc((*tamanoMatriz) * sizeof(int));
    }

    char *copiaCadena = strdup(texto);
    char *pedazo = strtok(copiaCadena, "[], ");
    int filaActual = 0;
    int columnaActual = 0;

    while (pedazo != NULL) {
        int numero = atoi(pedazo);
        matrizCreada[filaActual][columnaActual] = numero;
        columnaActual++;
        if (columnaActual == *tamanoMatriz) {
            columnaActual = 0;
            filaActual++;
        }
        pedazo = strtok(NULL, "[], ");
    }

    free(copiaCadena);
    return matrizCreada;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Modo de uso correcto:\n%s \"[[matriz]]\" \"origen\" \"archivo_salida\"\n", argv[0]);
        return 1;
    }

    int tamanoMatriz;
    int **grafoFinal = leerMatriz(argv[1], &tamanoMatriz);
    int nodoInicio = atoi(argv[2]);
    const char *rutaArchivoSalida = argv[3];

    if (nodoInicio < 0) {
        if (nodoInicio >= tamanoMatriz) {
            printf("El nodo origen debe estar entre 0 y %d\n", tamanoMatriz - 1);
            return 1;
        }
    }

    algoritmoDijkstra(grafoFinal, tamanoMatriz, nodoInicio, rutaArchivoSalida);

    for (int i = 0; i < tamanoMatriz; i++) {
        free(grafoFinal[i]);
    }
    free(grafoFinal);

    printf("Los resultados fueron guardados en: %s\n", rutaArchivoSalida);

    return 0;
}
