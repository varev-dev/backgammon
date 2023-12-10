//
// Created by varev on 09.12.2023.
//

#include <malloc.h>
#include "move.h"

char* moveAsString(pawn_move move) {
    char* output = malloc(10 * sizeof(char));

    output[0] = move.color;
    output[1] = ' ';

    output[2] = move.type;
    output[3] = ' ';

    if (move.type == FINISH_SIGN) {
        output[4] = move.initial + '0';
        output[5] = '\n';
        output[6] = '\0';
    } else if (move.type == INIT_BAR_SIGN) {
        output[4] = move.final + '0';
        output[5] = '\n';
        output[6] = '\0';
    } else {
        output[4] = move.initial + '0';
        output[5] = ' ';
        output[6] = move.final + '0';
        output[7] = '\n';
        output[8] = '\0';
    }

    return output;
}