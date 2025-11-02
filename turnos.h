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



#endif