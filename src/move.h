#ifndef BACKGAMMON_MOVE_H
#define BACKGAMMON_MOVE_H

#define NOT_SET 'N'
#define INIT_BAR_SIGN 'I'
#define BOARD_MOVE_SIGN 'B'
#define ATTACK_SIGN 'A'
#define END 'E'

typedef struct pawn_move {
    char type;
    char color;
    int initial;
    int final;
} pawn_move;

char* moveToString(pawn_move move);

#endif //BACKGAMMON_MOVE_H
