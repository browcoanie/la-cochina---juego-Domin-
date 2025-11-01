#ifndef UTILITY_H
#define UTILITY_H

#include "funciones.h"


// ESTRUCTURAS PARA LA MESA (LISTA DOBLEMENTE ENLAZADA)


struct mesaNode {
    Ficha ficha;
    mesaNode* prev;
    mesaNode* next;
    mesaNode(const Ficha& f) : ficha(f), prev(nullptr), next(nullptr) {}
};

struct Mesa {
    mesaNode* left;
    mesaNode* right;
    Mesa() : left(nullptr), right(nullptr) {}
};


// FUNCIONES PARA LA MESA


// Inicializa una mesa vacía
inline void initMesa(Mesa &mesa) {
    mesa.left = nullptr;
    mesa.right = nullptr;
}

// Retorna el valor del extremo izquierdo de la mesa
inline int valueLeft(const Mesa &mesa) {
    if (mesa.left == nullptr) return -1; // Mesa vacía
    return mesa.left->ficha.lado1;
}

// Retorna el valor del extremo derecho de la mesa
inline int valueRight(const Mesa &mesa) {
    if (mesa.right == nullptr) return -1; // Mesa vacía
    return mesa.right->ficha.lado2;
}

// Voltea los lados de una ficha
inline void swapSides(Ficha &ficha) {
    int temp = ficha.lado1;
    ficha.lado1 = ficha.lado2;
    ficha.lado2 = temp;
}

// Verifica si una ficha es válida (valores entre 0 y 6)
inline bool isValidFicha(const Ficha& ficha) {
    return (ficha.lado1 >= 0 && ficha.lado1 <= 6) && 
           (ficha.lado2 >= 0 && ficha.lado2 <= 6);
}

// Coloca una ficha a la izquierda de la mesa
inline bool placeLeft(Mesa &mesa, Ficha ficha) {
    if (!isValidFicha(ficha)) {
        return false;
    }

    // Si la mesa está vacía, simplemente añadimos la ficha
    if (mesa.left == nullptr) {
        mesaNode* newNode = new mesaNode(ficha);
        if (newNode == nullptr) {
            return false;
        }
        mesa.left = newNode;
        mesa.right = newNode;
        return true;
    }

    // Verificamos si la ficha coincide con el lado izquierdo
    int valorIzquierdo = valueLeft(mesa);

    // Si lado2 coincide, la ficha va en la orientación correcta
    if (ficha.lado2 == valorIzquierdo) {
        mesaNode* newNode = new mesaNode(ficha);
        if (newNode == nullptr) {
            return false;
        }
        newNode->next = mesa.left;
        mesa.left->prev = newNode;
        mesa.left = newNode;
        return true;
    }

    // Si lado1 coincide, volteamos la ficha
    if (ficha.lado1 == valorIzquierdo) {
        swapSides(ficha);
        mesaNode* newNode = new mesaNode(ficha);
        if (newNode == nullptr) {
            return false;
        }
        newNode->next = mesa.left;
        mesa.left->prev = newNode;
        mesa.left = newNode;
        return true;
    }

    // La ficha no coincide
    return false;
}

// Coloca una ficha a la derecha de la mesa
inline bool placeRight(Mesa &mesa, Ficha ficha) {
    if (!isValidFicha(ficha)) {
        return false;
    }

    // Si la mesa está vacía, simplemente añadimos la ficha
    if (mesa.right == nullptr) {
        mesaNode* newNode = new mesaNode(ficha);
        if (newNode == nullptr) {
            return false;
        }
        mesa.left = newNode;
        mesa.right = newNode;
        return true;
    }

    // Verificamos si la ficha coincide con el lado derecho
    int valorDerecho = valueRight(mesa);

    // Si lado1 coincide, la ficha va en la orientación correcta
    if (ficha.lado1 == valorDerecho) {
        mesaNode* newNode = new mesaNode(ficha);
        if (newNode == nullptr) {
            return false;
        }
        newNode->prev = mesa.right;
        mesa.right->next = newNode;
        mesa.right = newNode;
        return true;
    }

    // Si lado2 coincide, volteamos la ficha
    if (ficha.lado2 == valorDerecho) {
        swapSides(ficha);
        mesaNode* newNode = new mesaNode(ficha);
        if (newNode == nullptr) {
            return false;
        }
        newNode->prev = mesa.right;
        mesa.right->next = newNode;
        mesa.right = newNode;
        return true;
    }

    // La ficha no coincide
    return false;
}

// Limpia toda la mesa y libera la memoria
inline void clearMesa(Mesa &mesa) {
    mesaNode* current = mesa.left;
    while (current != nullptr) {
        mesaNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    mesa.left = nullptr;
    mesa.right = nullptr;
}

#endif 