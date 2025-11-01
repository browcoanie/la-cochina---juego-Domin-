#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <string>
#include <cstdlib>


// =====================================================
// ESTRUCTURAS DE DATOS
// =====================================================

// La ficha, simple
struct Ficha {
    int lado1;
    int lado2;
};

// El Nodo (El "vagón" de nuestra Pila)
struct NodoFicha {
    Ficha ficha;
    NodoFicha *siguiente; // El puntero al vagón de abajo
};


// La Pila (El contenedor)
//Solo necesita saber dónde está el tope
struct pilasFicha {
    NodoFicha *tope;
};

// datos del jugador
struct Jugador {
    std::string nombre; 
    pilasFicha mano;    // Un jugador tiene una pila que es su mano
    int puntos;         // Los puntos que saca el jugador
    bool paso;          // por si tranca la partida
};

// el "cerebro" del juego, guarda todo
struct Juego {
    Jugador jugadores[4]; // Un arreglo para jugardar los jugadores 
    int numJugadores;      // Para saber cuantos son (2,3 o 4)
    pilasFicha pozo;      // el pozo es otra pila
    int turnoActual;       //  Para saber a quien le toca
};



#endif