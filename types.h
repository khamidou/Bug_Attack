#ifndef TYPES_H
#define TYPES_H

namespace TYPE {

// Utilisation du bit 2^7 pour coder une information supplémentaire
// concernant les diagonales (cf doc)

typedef enum { NORTH = 1 , SOUTH = 2 , EAST = 4 , WEST = 8, UP = 128 } DIRECTION;
typedef enum { T_RAMPANT = 1, T_VOLANT = 2 } ENTITY;
typedef enum { CAFARD } ENEMY;

}

#endif // TYPES_H