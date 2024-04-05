#ifndef RULE_MANAGER_H
#define RULE_MANAGER_H

#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "basic.h"

class RuleManager {
public:
    RuleManager(const std::shared_ptr<Board> &board, const std::shared_ptr<GameInfo> &gameInfo) :
            boardSize(board->boardSize), board(std::const_pointer_cast<const Board>(board)),
            gameInfo(std::const_pointer_cast<const GameInfo>(gameInfo)) {}

    // Judge whether a move is legal.
    virtual MoveReason JudgeMove(const Move &move);

    // Judge whether the game is end.
    virtual std::pair<EndReason, Player> JudgeEnd(const MoveReason &reason);

    unsigned int boardSize;
    std::shared_ptr<const Board> board;
    std::shared_ptr<const GameInfo> gameInfo;
};

// Tool functions
enum class Direction {
    NORTH, SOUTH, WEST, EAST
};

// Get the next position and direction of a move.
std::pair<Position, Direction> get_next_pos(Position from, Direction dir, int n);

// Search for all the reachable points.
bool search(Position from, Position to,
            const std::shared_ptr<const Board> &board, unsigned int n);

// check if a capture move is valid.
bool check_valid_move(Position from, Position to,
                      const std::shared_ptr<const Board> &board, unsigned int n);

#endif //RULE_MANAGER_H
