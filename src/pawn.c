#include "pawn.h"

void InitPawn(pawn * pawn, char color) {
    pawn->color = color;
}

void InitManyPawns(pawn * pawn, char color, int amount) {
    while (amount--) {
        (pawn+amount)->color = color;
    }
}