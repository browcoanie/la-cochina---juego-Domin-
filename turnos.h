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
// 5. BUCLE DE ACCION (para no perder turno si se equivoca)
    jugadorActual.paso = false; 
    
    while (true) {
        std::cout << "\n¿Qué quieres hacer?" << std::endl;
        std::cout << "1. Jugar una ficha" << std::endl;
        std::cout << "2. Robar del pozo (si hay)" << std::endl;
        
        int opcion;
        if (!(std::cin >> opcion)) { // Validacion de numero
             std::cout << "❌ Error: Debes ingresar un NÚMERO." << std::endl;
             std::cin.clear();
             std::cin.ignore(10000, '\n');
             continue; // repite
        }

        switch (opcion) {
            
            // --- CASO 1: Jugar ---
            case 1: {
                std::cout << "\n¿Qué ficha quieres jugar? (número): ";
                int numFicha;
                
                if (!(std::cin >> numFicha)) { // Validacion
                     std::cout << "❌ Error: Debes ingresar un NÚMERO." << std::endl;
                     std::cin.clear();
                     std::cin.ignore(10000, '\n');
                     continue; 
                }
                
                // 1. Usamos la función "espía" (VER). La mano NO se modifica.
                Ficha fichaAJugar = verFichaPorPosicion(jugadorActual.mano, numFicha);
                
                // si puso un num invalido
                if (fichaAJugar.lado1 == -1) {
                    std::cout << "❌ Ficha inválida. Introduce un número de tu mano." << std::endl;
                    std::cout << "Presiona ENTER para continuar...";
                    std::cin.ignore(); std::cin.get();
                    continue; // no pierde turno
                }
                
                // si la ficha no se puede jugar
                if (!puedeJugarFicha(fichaAJugar, valorIzq, valorDer)) {
                    std::cout << "❌ Esa ficha no se puede jugar. Intenta con otra." << std::endl;
                    // Ya no hay que devolverla, porque nunca la sacamos
                    std::cout << "Presiona ENTER para continuar...";
                    std::cin.ignore(); std::cin.get();
                    continue; // no pierde turno
                }
                
                // Preguntar lado
                char lado;
                bool colocada = false;
                
                if (valorIzq == -1 && valorDer == -1) {
                    // si es la primera ficha, se pone sola
                    colocada = placeLeft(mesa, fichaAJugar);
                } else {
                    std::cout << "¿Dónde quieres colocarla? (I)zquierda o (D)erecha: ";
                    std::cin >> lado;
                    
                    if (lado == 'I' || lado == 'i') {
                        colocada = placeLeft(mesa, fichaAJugar);
                    } else if (lado == 'D' || lado == 'd') {
                        colocada = placeRight(mesa, fichaAJugar);
                    } else {
                        std::cout << "❌ Lado inválido. Intenta de nuevo." << std::endl;
                        std::cout << "Presiona ENTER para continuar...";
                        std::cin.ignore(); std::cin.get();
                        continue; 
                    }

                    if (!colocada) {
                        std::cout << "❌ Esa ficha no va en ese lado. Intenta de nuevo." << std::endl;
                        std::cout << "Presiona ENTER para continuar...";
                        std::cin.ignore(); std::cin.get();
                        continue;
                    }
                }

    
                std::cout << "✅ ¡Ficha colocada!" << std::endl;
                
                // 2. AHORA SÍ, sacamos la ficha de la mano
                Ficha fichaTemp; // variable temporal, no importa
                sacarFichaEspecifica(jugadorActual.mano, fichaAJugar.lado1, fichaAJugar.lado2, fichaTemp);
                
                // 3. GANO!
                if (pilaVacia(jugadorActual.mano)) {
                    std::cout << "\n🎉🎉🎉 ¡" << jugadorActual.nombre << " GANÓ LA RONDA! 🎉🎉🎉" << std::endl;
                    return true; // Termina el turno (GANA)
                }
                
                return false; // Jugada valida, termina turno

            } // Fin Case 1
            
            // --- CASO 2: Robar ---
            case 2: {
                if (robarDelPozo(juego, juego.turnoActual)) {
                    std::cout << "✅ Robaste: [" << verTopePila(jugadorActual.mano).lado1 
                         << "|" << verTopePila(jugadorActual.mano).lado2 << "]" << std::endl;
                    
                    std::cout << "💼 Tu mano actualizada: ";
                    mostrarFichasPila(jugadorActual.mano);
                    std::cout << std::endl;
                    std::cout << "Presiona ENTER para continuar...";
                    std::cin.ignore(); std::cin.get();
                    
                    // mostramos el estado actualizado
                    limpiarConsola();
                    std::cout << std::string(60, '=') << std::endl;
                    std::cout << "🎯 TURNO DE: " << jugadorActual.nombre << std::endl;
                    std::cout << std::string(60, '=') << std::endl;
                    mostrarMesa(mesa);
                    std::cout << "\n💼 Tu mano: ";
                    mostrarFichasPila(jugadorActual.mano);
                    std::cout << "\n🎲 Fichas en el pozo: " << contarFichasEnPila(juego.pozo) << std::endl;
                    
                    continue; // vuelve al menu (1. Jugar 2. Robar)

                } else {
                    std::cout << "❌ No hay fichas en el pozo." << std::endl;
                    std::cout << "Presiona ENTER para continuar...";
                    std::cin.ignore(); std::cin.get();
                    continue; // vuelve al menu
                }
            } // Fin Case 2
            
            // --- Default: opcion invalida ---
            default: {
                std::cout << "❌ Opción inválida. Elige 1 o 2." << std::endl;
                std::cout << "Presiona ENTER para continuar...";
                std::cin.ignore(); std::cin.get();
                continue; // vuelve al menu
            }
        } // Fin Switch
    } // Fin while(true)
}

// FUNCIONES DE CONFIGURACIÓN DE JUEGO

// Pide los datos de los jugadores
void configurarJugadores(Juego &juego) {
    // Bucle para validar el numero de jugadores
    while (true) {
        std::cout << "¿Cuántos jugadores van a jugar? (2-4): ";
        if (!(std::cin >> juego.numJugadores)) {
            std::cout << "❌ Error: Debes ingresar un NÚMERO." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        if (juego.numJugadores >= 2 && juego.numJugadores <= 4) {
            break; // Numero valido, salimos
        } else {
            std::cout << "❌ Opción inválida. Tiene que ser entre 2 y 4 jugadores." << std::endl;
        }
    }

    // Pide nombres y crea sus pilas vacias
    for(int i = 0; i < juego.numJugadores; i++) {
        std::cout << "Nombre del jugador " << (i + 1) << ": ";
        std::cin >> juego.jugadores[i].nombre;
        crearPila(juego.jugadores[i].mano);
        juego.jugadores[i].puntos = 0; // Puntos en contra empiezan en 0
        juego.jugadores[i].paso = false;
    }
}

// reparte 7 a cada uno
void repartirFichas(Juego &juego) {
    std::cout << "\n🎴 Repartiendo fichas..." << std::endl;
    for(int jugador = 0; jugador < juego.numJugadores; jugador++) {
        for(int ficha = 0; ficha < 7; ficha++) {
            Ficha fichaRobada = sacarFichaPila(juego.pozo);
            insertarFichaPila(juego.jugadores[jugador].mano, fichaRobada);
        }
    }
    std::cout << "✅ ¡Fichas repartidas! Cada jugador tiene 7 fichas." << std::endl;
}

// FUNCIONES DE CONTROL DE FLUJO

#endif