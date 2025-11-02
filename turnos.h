#ifndef TURNOS_H
#define TURNOS_H

#include "funciones.h"
#include "mesa.h"
#include <iostream>
#include <string>

// FUNCIONES DE VISUALIZACIÓN

// muestra la mano numerada
void mostrarFichasPila(pilasFicha pila) {
    NodoFicha *actual = pila.tope;
    int contador = 1;
    
    if (actual == nullptr) {
        std::cout << "No hay fichas";
        return;
    }
    
    while (actual != nullptr) {
        std::cout << contador << ".[" << actual->ficha.lado1 << "|" << actual->ficha.lado2 << "] ";
        actual = actual->siguiente;
        contador++;
    }
}

// muestra la mesa y las puntas
void mostrarMesa(const Mesa &mesa) {
    if (mesa.left == nullptr) {
        std::cout << "[MESA VACÍA]" << std::endl;
        return;
    }
    
    std::cout << "MESA: ";
    mesaNode* current = mesa.left;
    while (current != nullptr) {
        std::cout << "[" << current->ficha.lado1 << "|" << current->ficha.lado2 << "]";
        if (current->next != nullptr) std::cout << "-";
        current = current->next;
    }
    std::cout << "  (Izq:" << valueLeft(mesa) << " | Der:" << valueRight(mesa) << ")" << std::endl;
}

//FUNCIONES AUXILIARES DE JUEGO
Ficha verFichaPorPosicion(pilasFicha pila, int posicion) {
    NodoFicha* actual = pila.tope;
    int contador = 1;
    
    while (actual != nullptr) {
        if (contador == posicion) {
            return actual->ficha;
        }
        actual = actual->siguiente;
        contador++;
    }
    
    Ficha fichaVacia = {-1, -1}; // Indicador de ficha no encontrada
    return fichaVacia;
}

// FUNCIÓN PRINCIPAL DE TURNO
bool jugarTurno(Juego &juego, Mesa &mesa) {
    // 1. Limpiamos consola
    limpiarConsola(); 
    
    Jugador &jugadorActual = juego.jugadores[juego.turnoActual];
    
    // 2. Mostramos info
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "🎯 TURNO DE: " << jugadorActual.nombre << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    mostrarMesa(mesa);
    std::cout << "\n💼 Tu mano: ";
    mostrarFichasPila(jugadorActual.mano);
    std::cout << "\n🎲 Fichas en el pozo: " << contarFichasEnPila(juego.pozo) << std::endl;
    
    int valorIzq = valueLeft(mesa);
    int valorDer = valueRight(mesa);
    
    // 3. Revisamos si puede jugar
    bool tieneJugadas = tieneJugadaValida(jugadorActual.mano, valorIzq, valorDer);
    
    if (!tieneJugadas) {
        std::cout << "\n❌ No tienes jugadas válidas." << std::endl;
        
        // 4. Si no puede, roba
        if (!pilaVacia(juego.pozo)) {
            std::cout << "🎴 Robando del pozo..." << std::endl;
            robarDelPozo(juego, juego.turnoActual);
            
            std::cout << "💼 Tu mano actualizada: ";
            mostrarFichasPila(jugadorActual.mano);
            std::cout << std::endl;
            
            // si sigue sin poder, pasa
            tieneJugadas = tieneJugadaValida(jugadorActual.mano, valorIzq, valorDer);
            if (!tieneJugadas) {
                std::cout << "❌ Aún no puedes jugar. Pasas turno." << std::endl;
                jugadorActual.paso = true;
                return false; // Termina el turno
            }
            std::cout << "✅ ¡Ahora puedes jugar!" << std::endl;
        } else {
            // no hay pozo, pasa
            std::cout << "🚫 No hay fichas en el pozo. Pasas turno." << std::endl;
            jugadorActual.paso = true;
            return false; // Termina el turno
        }
    }

#endif