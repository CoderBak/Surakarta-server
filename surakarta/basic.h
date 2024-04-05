#ifndef BASIC_H
#define BASIC_H

#pragma once

#include "board.h"
#include "reason.h"
#include "piece.h"
#include "../common.h"

class Move {
public:
    Move(const Position from, const Position to, const Player player) : from(from), to(to), player(player) {}

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

    bool IsEnd() const { return endReason != EndReason::NONE; }

    friend std::ostream &operator<<(std::ostream &os, const GameInfo &game_info) {
        os << "current_player: " << game_info.currentPlayer << std::endl;
        os << "num_round: " << game_info.numRound << std::endl;
        os << "last_captured_round: " << game_info.lastCapturedRound << std::endl;
        os << "end_reason: " << game_info.endReason << std::endl;
        os << "winner: " << game_info.winner << std::endl;
        os << "max_no_capture_round: " << game_info.maxNoCaptureRound << std::endl;
        return os;
    }

    friend std::istream &operator>>(std::istream &is, GameInfo &game_info) {
        std::string str;
        is >> str >> game_info.currentPlayer;
        is >> str >> game_info.numRound;
        is >> str >> game_info.lastCapturedRound;
        is >> str >> game_info.endReason;
        is >> str >> game_info.winner;
        is >> str >> game_info.maxNoCaptureRound;
        return is;
    }
};

#endif //BASIC_H
