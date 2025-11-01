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

// =====================================================
// FUNCIONES PARA PILA (MANO DE JUGADORES)
// =====================================================

// Inicializa una pila vacía
inline void crearPila(pilasFicha &pila) {
    pila.tope = nullptr;
}

// Verifica si la pila está vacía
inline bool pilaVacia(pilasFicha pila) {
    return pila.tope == nullptr;
}

// Inserta una ficha en el tope de la pila
inline void insertarFichaPila(pilasFicha &pila, Ficha ficha) {
    NodoFicha *nuevaFicha = new NodoFicha;
    nuevaFicha->ficha = ficha;
    nuevaFicha->siguiente = pila.tope;
    pila.tope = nuevaFicha;
}

// Saca la ficha de arriba de la pila
inline Ficha sacarFichaPila(pilasFicha &pila) {
    if (pilaVacia(pila)) {
        Ficha fichaVacia = {-1, -1};
        return fichaVacia;
    }

    NodoFicha *fichaEliminar = pila.tope;
    Ficha fichaRobada = fichaEliminar->ficha;

    pila.tope = pila.tope->siguiente;

    delete fichaEliminar;

    return fichaRobada;
}

// Mira la ficha del tope sin sacarla
inline Ficha verTopePila(pilasFicha pila) {
    if (pilaVacia(pila)) {
        Ficha fichaVacia = {-1, -1};
        return fichaVacia;
    }
    return pila.tope->ficha;
}

// Cuenta cuántas fichas hay en una pila
inline int contarFichasEnPila(pilasFicha pila) {
    int total = 0;
    NodoFicha *actual = pila.tope;

    while (actual != nullptr) {
        total++;
        actual = actual->siguiente;
    }
    return total;
}

// Busca una ficha específica en la pila y la saca
inline bool sacarFichaEspecifica(pilasFicha &pila, int lado1, int lado2, Ficha &fichaEncontrada) {
    if (pilaVacia(pila)) {
        return false;
    }

    pilasFicha pilaAux;
    crearPila(pilaAux);
    bool encontrada = false;

    while (!pilaVacia(pila) && !encontrada) {
        Ficha actual = sacarFichaPila(pila);

        if ((actual.lado1 == lado1 && actual.lado2 == lado2) ||
            (actual.lado1 == lado2 && actual.lado2 == lado1)) {
            fichaEncontrada = actual;
            encontrada = true;
        } else {
            insertarFichaPila(pilaAux, actual);
        }
    }

    while (!pilaVacia(pilaAux)) {
        Ficha temp = sacarFichaPila(pilaAux);
        insertarFichaPila(pila, temp);
    }

    return encontrada;
}

// Libera toda la memoria de una pila
inline void limpiarPila(pilasFicha &pila) {
    while (!pilaVacia(pila)) {
        sacarFichaPila(pila);
    }
}


#endif