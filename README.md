# Dijkstra_paralelo
Entrega de Trabajo recuperativo, Gabriel Fuentes Bahamondes, Algoritmo de Dijkstra con uso de Openmp

Requisito principal:
Tener instalado GCC con soporte para OpenMP 

Desde el terminal ejecutamos una de estas 2 opciones, desde mi Windows solo me funciona 1:

gcc dijkstra_paralelo.c -o programa.exe -fopenmp -mconsole

gcc -fopenmp -o programa dijkstra_paralelo.c

luego:
./programa.exe "[[MATRIZ DE ADYACENCIA]]" vertice_inicial nombre_del_archivo_salida.txt

por ejemplo:

./programa.exe "[[0, 10, 0, 0, 5],[0, 0, 1, 0, 2],[0, 0, 0, 4, 0],[7, 0, 6, 0, 0], [0, 3, 9, 2, 0]]" 0 resultados.txt

de esta manera escribirá los resultados en el .txt direccionado
