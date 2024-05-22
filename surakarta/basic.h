// In this file, we define basic concepts,
// including Move, GameInfo.
#ifndef BASIC_H
#define BASIC_H

#pragma once

#include "board.h"
#include "reason.h"
#include "piece.h"
#include "../common.h"

using pair = std::pair<Position, std::vector<Position>>;

class Move {
public:
    Move(const Position &from, const Position &to, const Player &player) : from(from), to(to), player(player) {}
    friend bool operator==(const Move &lhs, const Move &rhs) = default;
    friend bool operator!=(const Move &lhs, const Move &rhs) = default;

    Position from, to;
    Player player;
};

struct GameInfo {
    Player currentPlayer;
    unsigned int numRound;
    unsigned int lastCapturedRound;
    EndReason endReason;
    Player winner;
    unsigned int maxNoCaptureRound;

    explicit GameInfo(const unsigned int maxNoCaptureRound = maxNoCapture) :
            currentPlayer(Player::BLACK), numRound(1), lastCapturedRound(0),
            endReason(EndReason::NONE), winner(Player::NONE),
            maxNoCaptureRound(maxNoCaptureRound) {}

    void reset() {
        currentPlayer = Player::BLACK;
        numRound = 1;
        lastCapturedRound = 0;
        endReason = EndReason::NONE;
        winner = Player::NONE;
    }
};

#endif //BASIC_H
