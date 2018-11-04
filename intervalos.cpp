/* 4976552 */

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
typedef intervalos_pos *intervalos_pos_t;

struct max_vol {
    intervalos_pos intervalo;
    uint ant_compatible,volumen;
};
typedef max_vol *max_vol_t;


/* ------------- FUNCIONES AUXILIARES ------------- */

/*  Se considera que dos pedidos i, j ∈ {1, . . . , n} son compatibles 
    si y solo si no se superponen en el tiempo */
bool son_compatibles(intervalo_t i, intervalo_t j) {
    return i.fin <= j.inicio || i.inicio >= j.fin;
}

/* Retorna el maximo de los dos valores */
uint max(uint a, uint b) {
    if(a<=b)
        return b;
    else //a>b
        return a;
}

/*  Busca el mayor de los intervalos compatibles en O(n.logn) pues va siempre a la mitad del array 
    Devuelve la posicion en el arreglo ''*/
uint mayor_inter_compatible(uint desde, uint hasta, max_vol_t maxVol, uint i) {
    uint j = ((hasta-desde)/2) + desde; //La mitad del intervalo
    if(desde == hasta) //Estoy en una sola casilla del array
        return desde; //Posicion en la cual esta mi mayor intervalo compatible
    else if(maxVol[j].intervalo.inter.fin>maxVol[i].intervalo.inter.inicio) //No es compatible
        return mayor_inter_compatible(desde, j-1, maxVol, i); //Voy a buscar a la mitad anterior a j
    else if(maxVol[j].intervalo.inter.fin<=maxVol[i].intervalo.inter.inicio && !(maxVol[j+1].intervalo.inter.fin<=maxVol[i].intervalo.inter.inicio)) //j es el mayor intervalo compatible, pues estan ordenados segun su fin
        return j;
    else if(maxVol[j].intervalo.inter.fin<=maxVol[i].intervalo.inter.inicio && (maxVol[j+1].intervalo.inter.fin<=maxVol[i].intervalo.inter.inicio)) //j NO es el mayor intervalo, pues j+1 tambien cumple la condicion
        return mayor_inter_compatible(j+1, hasta, maxVol, i); //Voy a buscar a la mitad luego a j
    return j;
}

/* ------------------------------------------------ */


/* ------------------------------------------------------------------- */
/* ------------------------------------------------------------------- */
/* Se crea el TAD 'Heap' en el cual siempre en la posicion h[1] tengo
el minimo intervalo segun su finalizacion.
El criterio de ordenacion es que el .fin del padre siempre es menor que el de sus hijos.*/

struct heap {
    intervalos_pos_t inter_pos;
    uint last, tamanio;
};
typedef heap *heap_t;

heap_t crear_heap(const intervalo_t *intervalos, uint n) {
    heap_t h = new heap;
    h->inter_pos = new intervalos_pos[n+1]; //Se crea hasta n+1 pues el lugar 0 no se usa y se necesitan n espacios
    h->last = 0; //Apunta al ultimo nodo con informacion (Si es 0 entonces el heap esta vacio)
    //Cargo datos en el heap
    uint i;
    for(i=1; i<n+1; i++) {
        (h->last)++;
        (h->inter_pos[h->last]).inter = intervalos[i-1]; //i-1 pues intervalos si utiliza el lugar 0
        (h->inter_pos[h->last]).pos = i-1;
        //Actualizo heap
        uint pos_actual = h->last;
        uint padre = pos_actual/2;
        while(pos_actual>1 && ((h->inter_pos[padre]).inter.fin > h->inter_pos[pos_actual].inter.fin)) {
            //Intercambio el hijo con el padre
            intervalos_pos aux = h->inter_pos[pos_actual];
            h->inter_pos[pos_actual] = h->inter_pos[padre];
            h->inter_pos[padre] = aux;
            //Actualizo posiciones
            pos_actual = padre;
            padre = pos_actual/2;
        }
    }
    h->tamanio = n;
    return h;
}

bool es_vacio_heap(heap_t h) { return h->last == 0; }

bool esta_lleno_heap(heap_t h) { return h->last == h->tamanio + 1; }

/* void insertar_en_heap(heap_t h, intervalo_t inter, uint pos) {
    if(!esta_lleno_heap(h)) {
        //Cargo datos
        (h->last)++;
        (h->inter_pos[h->last]).inter = inter;
        (h->inter_pos[h->last]).pos = pos;
        //Actualizo heap
        uint pos_actual = h->last;
        uint padre = pos_actual/2;
        while(pos_actual>1 && ((h->inter_pos[padre]).inter.fin > h->inter_pos[pos_actual].inter.fin)) {
            //Intercambio el hijo con el padre
            intervalos_pos aux = h->inter_pos[pos_actual];
            h->inter_pos[pos_actual] = h->inter_pos[padre];
            h->inter_pos[padre] = aux;

            pos_actual = padre;
            padre = pos_actual/2;
        }
    }
} */

intervalos_pos obtener_minimo(heap_t h) {
    intervalos_pos inter_min = h->inter_pos[1]; //El minimo elemento, para devolver
    //Traigo el ultimo elemento a la primera posicion
    h->inter_pos[1] = h->inter_pos[h->last];
    h->last--;
    //Actualizo heap
    uint pos_actual = 1;
    uint hijo1 = pos_actual*2;
    uint hijo2 = (pos_actual*2) + 1;
    while(pos_actual < h->last && ((hijo1 <= h->last && h->inter_pos[hijo1].inter.fin < h->inter_pos[pos_actual].inter.fin)
    || (hijo2 <= h->last && h->inter_pos[hijo2].inter.fin < h->inter_pos[pos_actual].inter.fin))) {
        if(!(h->inter_pos[hijo2].inter.fin < h->inter_pos[hijo1].inter.fin) && 
        h->inter_pos[hijo1].inter.fin < h->inter_pos[pos_actual].inter.fin) {
            //Intercambio el hijo con el padre
            intervalos_pos aux = h->inter_pos[pos_actual];
            h->inter_pos[pos_actual] = h->inter_pos[hijo1];
            h->inter_pos[hijo1] = aux;
            //Actualizo posiciones
            pos_actual = hijo1;
            hijo1 = pos_actual*2;
            hijo2 = (pos_actual*2) + 1;
        }
        else { //h->inter_pos[hijo2].inter.fin < h->inter_pos[pos_actual].inter.fin
            //Intercambio el hijo con el padre
            intervalos_pos aux = h->inter_pos[pos_actual];
            h->inter_pos[pos_actual] = h->inter_pos[hijo2];
            h->inter_pos[hijo2] = aux;
            //Actualizo posiciones
            pos_actual = hijo2;
            hijo1 = pos_actual*2;
            hijo2 = (pos_actual*2) + 1;
        }
    } //while
    return inter_min;
}

void liberar_heap(heap_t h) {
    if(h->inter_pos != NULL)
        delete[] h->inter_pos;
    delete h; 
}

/* ------------------------------------------------------------------- */
/* ------------------------------------------------------------------- */

/* Devuelve un arreglo de booleanos de 'n' con TRUE en los intervalos asignados,
   los cuales no se superponen.
   La cantidad de intervalos asignados debe ser la máxima posible.
   'intervalos' es un arreglo con 'n' intervalo_t.
   El tiempo de ejecucion de peor caso debe ser O(n*log(n)).
*/
bool *max_cantidad(const intervalo_t *intervalos, uint n) {
    //Inicializo mi arreglo en false, luego voy agregando
    bool *ab = new bool[n];
    uint i;
    for (i = 0; i<n; i++) {
        ab[i] = false;
    }
    /*Ordeno en un nuevo array 'int_pos' mis intervalos en orden ascendiente 
    de finalizacion (Heap Sort ~ O(nlogn))*/
    /*Voy tomando los menores intervalos segun el tiempo de finalizacion
    y voy eliminando los que NO son compatibles*/
    heap_t heap = crear_heap(intervalos,n);
    intervalos_pos_t int_pos = new intervalos_pos[n];
    //Paso base
    int_pos[0] = obtener_minimo(heap);
    intervalos_pos ant_compatible = int_pos[0];
    ab[int_pos[0].pos] = true;
    //Paso inductivo
    for(i=1; i<n; i++) {
        int_pos[i] = obtener_minimo(heap);
        if(son_compatibles(int_pos[i].inter,ant_compatible.inter)) { //Son compatibles
            ab[int_pos[i].pos] = true;
            ant_compatible = int_pos[i];
        }
        else { // NO son compatibles
            ab[int_pos[i].pos] = false;
        }
    }
    delete[] int_pos;
    liberar_heap(heap);
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
    //Inicializo mi arreglo en false, luego voy agregando
    bool *ab = new bool[n];
    uint i;
    for (i = 0; i<n; i++) {
        ab[i] = false;
    }
    /*Ordeno en un nuevo array 'maxVol' mis intervalos en orden ascendiente 
    de finalizacion (Heap Sort ~ O(nlogn))
    Luego, en cada posicion de este arreglo ordenado voy calculando el valor maximo*/
    heap_t heap = crear_heap(intervalos,n);
    max_vol_t maxVol = new max_vol[n+1];//Creo de n+1 pues el lugar 0 lo uso para setear el valor 0
    maxVol[0].volumen = 0;
    for(i=1; i<n+1; i++) {
        maxVol[i].intervalo = obtener_minimo(heap);
        maxVol[i].ant_compatible = 0;
        //Seteo el valor de 'ant_compatible'
        if(maxVol[1].intervalo.inter.fin>maxVol[i].intervalo.inter.inicio) //No es compatible con niguno anterior
            maxVol[i].ant_compatible = 0;
        else
            maxVol[i].ant_compatible = mayor_inter_compatible(1, i, maxVol, i);
        /*Seteo el valor maxVol[j].volumen = max { intervalos[maxVol[j].intervalo.pos].volumen + 
                                            maxVol[maxVol[j].ant_compatible] , maxVol[j-1]  } */
        maxVol[i].volumen = max(intervalos[maxVol[i].intervalo.pos].volumen + maxVol[maxVol[i].ant_compatible].volumen, maxVol[i-1].volumen);
        
    }
    //Agrego los elementos a mi solucion
    i = n;
    while(i>0) {
        if(intervalos[maxVol[i].intervalo.pos].volumen + maxVol[maxVol[i].ant_compatible].volumen > maxVol[i-1].volumen) {//Agrego el intervalo i
            ab[maxVol[i].intervalo.pos] = true;
            i = maxVol[i].ant_compatible;
        }
        else { //NO agrego el intervalo i
            i--;
        }
    }
    delete[] maxVol;
    liberar_heap(heap);
    return ab;
}



