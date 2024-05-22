// In this file, we define Piece and Position.
#ifndef PIECE_H
#define PIECE_H

#pragma once

#include <iostream>

using PieceColorMemoryType = int;
enum class PieceColor : PieceColorMemoryType {
    BLACK, WHITE, NONE
};
using Player = PieceColor;

inline PieceColor ReverseColor(PieceColor color) {
    switch (color) {
        case PieceColor::BLACK:
            return PieceColor::WHITE;
        case PieceColor::WHITE:
            return PieceColor::BLACK;
        default:
            return color;
    }
}

inline std::ostream &operator<<(std::ostream &os, const PieceColor &color) {
    switch (color) {
        case PieceColor::NONE:
            os << ".";
            break;
        case PieceColor::BLACK:
            os << "B";
            break;
        case PieceColor::WHITE:
            os << "W";
            break;
        default:
            os << "?";
            break;
    }
    return os;
}

struct Position {
    unsigned int x, y;

    Position(unsigned int x, unsigned int y) : x(x), y(y) {}
    bool operator==(const Position &rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const Position &rhs) const { return !(*this == rhs); }
};

class Piece {
public:
    explicit Piece(PieceColor color) : color_(color) {}
    void SetColor(PieceColor color) { color_ = color; }
    [[nodiscard]] PieceColor GetColor() const { return color_; }
    friend std::ostream &operator<<(std::ostream &os, const Piece &piece) {
        os << piece.color_;
        return os;
    }

private:
    PieceColor color_;
};

#endif //PIECE_H
