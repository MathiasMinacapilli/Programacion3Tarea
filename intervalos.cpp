
#include "include/intervalos.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <climits>
#include <time.h>

struct intervalos_pos {
    intervalo_t inter;
    int pos; //Me dice la posicion dentro del array 'intervalos'
};

typedef intervalos_pos *intervalos_posicion;

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
    /*Agrego el primer intervalo (el de menor valor de fin)*/
    int min_f = minimo 
    

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

/*  Se considera que dos pedidos i, j ∈ {1, . . . , n} son compatibles 
    si y solo si no se superponen en el tiempo */
bool son_compatibles(intervalo_t i, intervalo_t j) {
    return i.fin <= j.inicio || i.inicio >= j.fin;
}



