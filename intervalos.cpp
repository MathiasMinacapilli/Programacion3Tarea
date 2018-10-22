/* 4976552 */

#include "include/intervalos.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <climits>
#include <time.h>

/* ------------------------------------------------------------------- */
/* Se crea el TAD 'Heap' en el cual siempre en la posicion h[1] tengo
el minimo intervalo segun su finalizacion.
El criterio de ordenacion es que el padre siempre es menor que sus hijos.*/

struct intervalos_pos {
    intervalo_t inter;
    int pos; //Me dice la posicion dentro del array 'intervalos'
};

struct heap {
    intervalos_pos *inter_pos;
    uint last, tamanio;
};
typedef heap *heap_t;

heap_t crear_heap(const intervalo_t *intervalos, uint n) {
    heap_t h = new heap;
    h->inter_pos = new intervalos_pos[n+1]; //Se crea hasta n+1 pues el lugar 0 no se usa y se necesitan n espacios
    h->last = 0; //Apunta al ultimo nodo con informacion (Si es 0 entonces el heap esta vacio)
    h->tamanio = n;
    return h;
}

bool es_vacio_heap(heap_t h) { return h->last == 0; }
bool esta_lleno_heap(heap_t h) { return h->last == h->tamanio + 1; }

void insertar_en_heap(heap_t h, intervalo_t inter, uint pos) {
    if(!esta_lleno_heap(h)) {
        //Cargo datos
        (h->last)++;
        (h->inter_pos[h->last]).inter = inter;
        (h->inter_pos[h->last]).pos = pos;
        //Actualizo heap
        int pos_actual = h->last;
        int padre = pos_actual/2;
        while(pos_actual>1 && ((h->inter_pos[padre]).inter.fin > h->inter_pos[pos_actual].inter.fin)) {
            //Intercabmio el hijo con el padre
            intervalos_pos aux = h->inter_pos[pos_actual];
            h->inter_pos[pos_actual] = h->inter_pos[padre];
            h->inter_pos[padre] = aux;

            pos_actual = padre;
            padre = pos_actual/2;
        }
    }
}

/* intervalos_pos obtener_minimo(heap_t &h) {

} */


/* ------------------------------------------------------------------- */

/* Devuelve un arreglo de booleanos de 'n' con TRUE en los intervalos asignados,
   los cuales no se superponen.
   La cantidad de intervalos asignados debe ser la máxima posible.
   'intervalos' es un arreglo con 'n' intervalo_t.
   El tiempo de ejecucion de peor caso debe ser O(n*log(n)).
*/
bool *max_cantidad(const intervalo_t *intervalos, uint n) {
    //Inicializo mi arreglo en true, luego voy quitando
    bool *ab = new bool[n];
    uint i;
    for (i = 0; i<n; i++) {
        ab[i] = false;
    }
    /*Ordeno en un nuevo array 'int_pos' mis intervalos en orden ascendiente 
    de finalizacion*/
    

    /*Voy tomando los menores intervalos segun el tiempo de finalizacion
    y voy eliminando los que NO son compatibles*/
    return ab;
}


/* Devuelve un arreglo de booleanos de 'n' con TRUE en los intervalos asignados,
   los cuales no se superponen.
   La suma de los volumenes de los intervalos asignados debe ser la máxima
   posible.
   'intervalos' es un arreglo con 'n' intervalo_t.
    El tiempo de ejecucion de peor caso debe ser O(n*log(n)).
*/
bool *max_volumen(const intervalo_t *intervalos, uint n) {
    bool *ab[n];
    return ab[n];
}

/* ---------- FUNCIONES AUXILIARES ---------- */

/*  Se considera que dos pedidos i, j ∈ {1, . . . , n} son compatibles 
    si y solo si no se superponen en el tiempo */
bool son_compatibles(intervalo_t i, intervalo_t j) {
    return i.fin <= j.inicio || i.inicio >= j.fin;
}



