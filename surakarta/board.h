#ifndef BOARD_H
#define BOARD_H

#pragma once

#include <memory>
#include <vector>
#include <iostream>
#include "piece.h"

class Row : public std::vector<std::shared_ptr<Piece>> {
public:
    Row(unsigned int board_size) : std::vector<std::shared_ptr<Piece>>(board_size) {}
};

class Board : public std::vector<Row> {
public:
    unsigned int board_size_;

    Board(unsigned int board_size) : board_size_(board_size) {
        for (unsigned int i = 0; i < board_size_; i++) {
            this->push_back(Row(board_size_));
        }
    }

    bool IsInside(const Position &position) const {
        return position.x < board_size_ && position.y < board_size_;
    }

    friend inline std::ostream &operator<<(std::ostream &os, const Board &board) {
        for (unsigned int y = 0; y < board.board_size_; y++) {
            for (unsigned int x = 0; x < board.board_size_; x++) {
                os << (*board[x][y]) << " ";
            }
            os << std::endl;
        }
        return os;
    }

    friend inline std::istream &operator>>(std::istream &is, Board &board) {
        for (unsigned int y = 0; y < board.board_size_; y++) {
            for (unsigned int x = 0; x < board.board_size_; x++) {
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


#endif //_SERVER_BOARD_H
