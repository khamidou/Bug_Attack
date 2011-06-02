/**
* @file types.h
* @brief Ce fichier contient les constantes et types prédéfinis du jeu, utilisés
* à des fins de simplification du code. On notera que les valeurs de type DIRECTION
* utilisent chacune un bit différent, utile pour les tests à base de masques de bits.
*/

#ifndef TYPES_H
#define TYPES_H

namespace TYPE {

// Utilisation du bit 2^7 pour coder une information supplémentaire
// concernant les diagonales (cf doc)

typedef enum { NORTH = 1 , SOUTH = 2 , EAST = 4 , WEST = 8, GOAL = 32, UP = 128 } DIRECTION;
typedef enum { T_RAMPANT = 1, T_VOLANT = 2 } ENTITY;
typedef enum { CAFARD , FOURMI , GUEPE , MOUSTIQUE } ENEMY;
typedef enum { NONE, PISTOLET_A_EAU, LANCE_PIERRES, PAINTBALL, PETANQUE, MUSICIEN} TURRET;


};

namespace GAME {

typedef enum { EASY = 1, HARD = 2 } DIFFICULTY;
static const short FPS = 30;

}

#endif // TYPES_H
