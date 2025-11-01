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


// FUNCIONES PARA FICHAS


// solo crea la ficha
inline Ficha crearFicha(int num1, int num2) {
    Ficha nuevaFicha;
    nuevaFicha.lado1 = num1;
    nuevaFicha.lado2 = num2;
    return nuevaFicha;
}

// genera las 28 fichas
inline void generarTodasLasFichas(pilasFicha &pila) {
    for (int i = 0; i <= 6; i++) {
        for (int j = i; j <= 6; j++) { // j=i para no repetir (ej. [1|2] y [2|1])
            Ficha ficha = crearFicha(i, j);
            insertarFichaPila(pila, ficha);
        }
    }
}

// Barajar (Algoritmo Fisher-Yates)
inline void mezclarPozo(pilasFicha &pila) {
    int totalFichas = contarFichasEnPila(pila);
    if (totalFichas <= 1) return;

    // 1. crear array temp
    Ficha *fichas = new Ficha[totalFichas];
    // 2. vaciar pila al array
    for (int i = 0; i < totalFichas; i++) {
        fichas[i] = sacarFichaPila(pila);
    }
    // 3. el algoritmo de barajado
    for (int i = totalFichas - 1; i > 0; i--) {
        int j = rand() % (i + 1); // indice al azar
        // swap
        Ficha temp = fichas[i];
        fichas[i] = fichas[j];
        fichas[j] = temp;
    }
    // 4. devolver a la pila
    for (int i = 0; i < totalFichas; i++) {
        insertarFichaPila(pila, fichas[i]);
    }
    // 5. OJO: borrar el array
    delete[] fichas;
}


// FUNCIONES DE LÓGICA DE JUEGO

// revisa la mano por el doble mas alto
inline int buscarFichaDobleMasAlta(pilasFicha mano) {
    int fichaDobleMaxima = -1; // -1 = ninguna
    NodoFicha *actual = mano.tope;

    while (actual != nullptr) {
        if (actual->ficha.lado1 == actual->ficha.lado2) { // es doble?
            if (actual->ficha.lado1 > fichaDobleMaxima) {
                fichaDobleMaxima = actual->ficha.lado1;
            }
        }
        actual = actual->siguiente;
    }
    return fichaDobleMaxima;
}

// compara los dobles de todos los jugadores (SOLO PARA LA RONDA 1)
inline int determinarQuienEmpieza(Juego &juego, int &fichaEncontrada) {
    int jugadorInicial = 0;
    int fichaDobleMaximaGlobal = -1;

    for (int i = 0; i < juego.numJugadores; i++) {
        int fichaDobleJugador = buscarFichaDobleMasAlta(juego.jugadores[i].mano);
        if (fichaDobleJugador > fichaDobleMaximaGlobal) {
            fichaDobleMaximaGlobal = fichaDobleJugador;
            jugadorInicial = i;
        }
    }
    fichaEncontrada = fichaDobleMaximaGlobal;
    return jugadorInicial; // devuelve el INDICE del jugador
}

// el % hace que de la vuelta (ej. 3+1 % 4 = 0)
inline void siguienteTurno(Juego &juego) {
    juego.turnoActual = (juego.turnoActual + 1) % juego.numJugadores;
}

// revisa si la ficha se puede jugar
inline bool puedeJugarFicha(Ficha ficha, int valorIzq, int valorDer) {
    // si la mesa esta vacia, cualquiera sirve
    if (valorIzq == -1 && valorDer == -1) {
        return true;
    }
    // revisa las puntas
    return (ficha.lado1 == valorIzq || ficha.lado2 == valorIzq ||
            ficha.lado1 == valorDer || ficha.lado2 == valorDer);
}

// revisa toda la mano a ver si tiene AL MENOS una jugada
inline bool tieneJugadaValida(pilasFicha mano, int valorIzq, int valorDer) {
    NodoFicha *actual = mano.tope;
    while (actual != nullptr) {
        if (puedeJugarFicha(actual->ficha, valorIzq, valorDer)) {
            return true; // encontramos una!
        }
        actual = actual->siguiente;
    }
    return false; // no tiene nada
}

// pop del pozo, push a la mano
inline bool robarDelPozo(Juego &juego, int indiceJugador) {
    if (pilaVacia(juego.pozo)) {
        return false;
    }
    Ficha fichaRobada = sacarFichaPila(juego.pozo);
    insertarFichaPila(juego.jugadores[indiceJugador].mano, fichaRobada);
    return true;
}


// FUNCIONES PARA PUNTUAR

// suma los puntos de una mano (ESTA ES CLAVE AHORA)
int calcularPuntosManos(pilasFicha mano){
    int puntosTotales = 0;
    NodoFicha *actual = mano.tope;
    while (actual != nullptr)
    {
        puntosTotales += actual -> ficha.lado1 + actual -> ficha.lado2;
        actual = actual -> siguiente; 
    }
    return puntosTotales;
}


// si hay tranca, gana el que tenga MENOS puntos
int encontrarGanadorPorTranca(Juego &juego){
    int jugadorConMenosPuntos = 0;
    int menosPuntos = calcularPuntosManos(juego.jugadores[0].mano);
    for (int i = 1; i < juego.numJugadores;    i++)
    {
        int puntosJugadores = calcularPuntosManos(juego.jugadores[i].mano);
        if (puntosJugadores < menosPuntos)
        {
            menosPuntos = puntosJugadores;
            jugadorConMenosPuntos = i;
        }
    }
    return jugadorConMenosPuntos;
}

// solo imprime la tabla de puntos
inline void mostrarPuntosJugadores(Juego &juego) {
    std::cout << "\n📊 PUNTUACIÓN ACUMULADA (Menos es mejor):" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    for (int i = 0; i < juego.numJugadores; i++) {
        std::cout << juego.jugadores[i].nombre << ": " << juego.jugadores[i].puntos << " puntos" << std::endl;
    }
    std::cout << std::string(40, '-') << std::endl;
}

#endif

