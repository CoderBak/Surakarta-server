#ifndef RULE_MANAGER_H
#define RULE_MANAGER_H

#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "basic.h"

class RuleManager {
public:
    RuleManager() = default;

    RuleManager(const std::shared_ptr<Board> &board, const std::shared_ptr<GameInfo> &game_info) :
            board_size_(board->board_size_), board_(std::const_pointer_cast<const Board>(board)),
            game_info_(std::const_pointer_cast<const GameInfo>(game_info)) {}

    /**
     * @brief Judge whether a move is legal.
     * @param move The move to be judged.
     */
    virtual IllegalMoveReason JudgeMove(const Move &move);

    /**
     * @brief Judge whether the game is end.
     * @param reason IllegalMoveReason of the last move.
     */
    virtual std::pair<EndReason, Player> JudgeEnd(const IllegalMoveReason &reason);

    unsigned int board_size_{};
    std::shared_ptr<const Board> board_;
    std::shared_ptr<const GameInfo> game_info_;
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
