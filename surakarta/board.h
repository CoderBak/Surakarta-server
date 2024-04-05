#ifndef BOARD_H
#define BOARD_H

#pragma once

#include <memory>
#include <vector>
#include <iostream>
#include "piece.h"

class Board : public std::vector<std::vector<std::shared_ptr<Piece>>> {
public:
    unsigned int boardSize;

    explicit Board(unsigned int boardSize) : boardSize(boardSize) {
        for (unsigned int i = 0; i < boardSize; i++) {
            this->emplace_back(boardSize);
        }
    }

    friend inline std::ostream &operator<<(std::ostream &os, const Board &board) {
        for (unsigned int y = 0; y < board.boardSize; y++) {
            for (unsigned int x = 0; x < board.boardSize; x++) {
                os << (*board[x][y]) << " ";
            }
            os << std::endl;
        }
        return os;
    }

    friend inline std::istream &operator>>(std::istream &is, Board &board) {
        for (unsigned int y = 0; y < board.boardSize; y++) {
            for (unsigned int x = 0; x < board.boardSize; x++) {
                char ch;
                is >> ch;
                PieceColor color;
                switch (ch) {
                    case 'B':
                        color = PieceColor::BLACK;
                        break;
                    case 'W':
                        color = PieceColor::WHITE;
                        break;
                    default:
                        color = PieceColor::NONE;
                        break;
                }
                board[x][y] = std::make_shared<Piece>(x, y, color);
            }
        }
        return is;
    }
};


#endif //BOARD_H
