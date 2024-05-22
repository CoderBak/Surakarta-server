// In this file, we define MoveReason and EndReason.
#ifndef REASON_H
#define REASON_H

#pragma once

enum class MoveReason {
    LEGAL, // unused
    LEGAL_CAPTURE_MOVE, // capture an opponent's piece, and the move consists at least one corner loop
    LEGAL_NON_CAPTURE_MOVE, // just as the name
    ILLEGAL, // unused
    NOT_PLAYER_TURN, // move when it's not the player's turn.
    OUT_OF_BOARD, // from or to position is out of board
    NOT_PIECE, // move a position that is not a piece
    NOT_PLAYER_PIECE, // move a piece that is not the player's
    ILLEGAL_CAPTURE_MOVE, // try to capture an opponent's piece, but the move can't consist any corner loop
    ILLEGAL_NON_CAPTURE_MOVE, // otherwise
    GAME_ALREADY_END, // unused
    GAME_NOT_START // unused
};

enum class EndReason {
    NONE, // not end
    STALEMATE, // both players can't make more move
    CHECKMATE, // one player's all pieces are captured
    TRAPPED, // unused, one player's pieces are all trapped, no legal move can be made.
    RESIGN, // one player resigns.
    TIMEOUT, // one player's time is out.
    ILLEGAL_MOVE, // one player makes an illegal move
};

inline bool IsEndReason(EndReason reason) {
    return reason != EndReason::NONE;
}

#endif //REASON_H
