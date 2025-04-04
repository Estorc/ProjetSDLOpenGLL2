#pragma once
#include <raptiquax.h>
#include <math/math_util.h>
#include <storage/node.h>
#include <scripts/scripts.h>

#include <io/socket/socket.h>
#include <classes/classes.h>

#include <storage/hash.h>
#include <storage/raw.h>


#include <time.h>
#include "client.h"


typedef enum ChessPiece {
    FC_EMPTY,
    FC_PAWN,
    FC_ROOK,
    FC_KNIGHT,
    FC_BISHOP,
    FC_QUEEN,
    FC_KING
} ChessPiece;

typedef enum ChessPieceColor {
    FC_UNKNOWN = 0,
    FC_WHITE = 1 << 4,
    FC_BLACK = 2 << 4
} ChessPieceColor;

typedef enum ChessPieceFlags {
    FC_HOVERED = 1 << 6,
    FC_SELECTED = 1 << 7,
    FC_ATTACKED = 1 << 8,
    FC_MOVED = 1 << 9,
} ChessPieceFlags;

typedef enum MovementState {
    FC_FORBIDDEN,
    FC_VALID_MOVE,
    FC_ATTACKING,
    FC_CASTLING,
} MovementState;

void set_piece_skin(Node *piece, int flags);