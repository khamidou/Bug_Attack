#ifndef TYPES_H
#define TYPES_H

namespace TYPE {

// Utilisation du bit 2^7 pour coder une information supplémentaire
// concernant les diagonales (cf doc)

typedef enum DIRECTION { NORTH = 1 , SOUTH = 2 , EAST = 4 , WEST = 8, UP = 128 } DIRECTION;
typedef enum ENTITY { T_RAMPANT = 1, T_VOLANT = 2 } ENTITY;

}

#endif // TYPES_H
