//
// Created by CoderBak on 2024/4/5.
//

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

    RuleManager(std::shared_ptr<Board> board, std::shared_ptr<GameInfo> game_info) :
            board_size_(board->board_size_), board_(std::const_pointer_cast<const Board>(board)),
            game_info_(std::const_pointer_cast<const GameInfo>(game_info)) {}

    unsigned int GetBoardSize() { return board_size_; }

    virtual void OnUpdateBoard();

    /**
     * @brief Judge whether a move is legal.
     * @param move The move to be judged.
     */
    virtual IllegalMoveReason JudgeMove(const Move &move);

    /**
     * @brief Judge whether the game is end.
     * @param reason IllegalMoveReason of the last move.
     */
    virtual std::pair<EndReason, Player> JudgeEnd(const IllegalMoveReason reason);

    /**
     * @brief Get all legal target positions of a piece.
     * @param postion The position of the piece.
     */
    virtual std::unique_ptr<std::vector<Position>> GetAllLegalTarget(const Position postion);

    //    protected:
    unsigned int board_size_;
    std::shared_ptr<const Board> board_;
    std::shared_ptr<const GameInfo> game_info_;

public:
    // TODO:
    //  define your own functions/variables here
    static void HelloWorld();

    static void InlineHelloWorld() { std::cout << "Hello World!" << std::endl; }

    int bye_world_ = 0;
};

// Tool functions
enum class Direction { NORTH, SOUTH, WEST, EAST };

// Get the next postition and direction of a move.
std::pair<Position, Direction> get_next_pos(Position from, Direction dir, int n);

// Search for all the reachable points.
bool search(Position from, Position to,
            const std::shared_ptr<const Board> &board, unsigned int n);

// check if a capture move is valid.
bool check_valid_move(Position from, Position to,
                      const std::shared_ptr<const Board> &board, unsigned int n);

#endif //_SERVER_RULE_MANAGER_H
