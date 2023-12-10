//
// Created by varev on 09.12.2023.
//

#include <malloc.h>
#include "move.h"

char* moveToString(pawn_move move) {
    char* output = malloc(20 * sizeof(char));

    output[0] = move.color;
    output[1] = ' ';

    output[2] = move.type;
    output[3] = ' ';
    int c = 4;
    if (move.type == FINISH_SIGN || move.type != INIT_BAR_SIGN) {
        output[c++] = move.initial/10 + '0';
        output[c++] = move.initial%10 + '0';
    }
    output[c++] = ' ';
    if (move.type == INIT_BAR_SIGN || move.type != FINISH_SIGN) {
        output[c++] = move.final/10 + '0';
        output[c++] = move.final%10 + '0';
    }

    output[c++] = '\n';
    output[c++] = '\0';

    return output;
}