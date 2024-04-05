//
// Created by CoderBak on 2024/4/5.
//

#ifndef REASON_H
#define REASON_H

#pragma once

#include <iostream>

enum class IllegalMoveReason {
    LEGAL, // unused
    LEGAL_CAPTURE_MOVE, // capture a opponent's piece, and the move consists at least one corner loop
    LEGAL_NON_CAPTURE_MOVE, // just as the name
    ILLIGAL, // unused
    NOT_PLAYER_TURN, // move when it's not the player's turn.
    OUT_OF_BOARD, // from or to position is out of board
    NOT_PIECE, // move a position that is not a piece
    NOT_PLAYER_PIECE, // move a piece that is not the player's
    ILLIGAL_CAPTURE_MOVE, // try to capture a opponent's piece, but the move can't consist any corner loop
    ILLIGAL_NON_CAPTURE_MOVE, // otherwise
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
    ILLIGAL_MOVE, // one player makes an illegal move
};

bool IsLegalMoveReason(IllegalMoveReason reason);

bool IsEndReason(EndReason reason);

std::ostream &operator<<(std::ostream &os, const IllegalMoveReason &reason);

std::istream &operator>>(std::istream &is, IllegalMoveReason &reason);

std::ostream &operator<<(std::ostream &os, const EndReason &reason);

std::istream &operator>>(std::istream &is, EndReason &reason);


#endif //REASON_H
