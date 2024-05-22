// In this file, we define the Board class.
#ifndef BOARD_H
#define BOARD_H

#pragma once

#include <memory>
#include <vector>
#include "piece.h"

class Board : public std::vector<std::vector<std::shared_ptr<Piece>>> {
public:
    unsigned int boardSize;

    explicit Board(unsigned int boardSize) : boardSize(boardSize) {
        for (unsigned int i = 0; i < boardSize; i += 1) {
            this->emplace_back(boardSize);
        }
    }

    friend inline std::ostream &operator<<(std::ostream &os, const Board &board) {
        for (unsigned int y = 0; y < board.boardSize; y += 1) {
            for (unsigned int x = 0; x < board.boardSize; x += 1) {
                os << (*board[x][y]) << " ";
            }
            os << "|";
        }
        return os;
    }
};

#endif //BOARD_H
