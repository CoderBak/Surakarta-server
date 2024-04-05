#ifndef PIECE_H
#define PIECE_H

#pragma once

#include <iostream>

using PieceColorMemoryType = int;
enum class PieceColor : PieceColorMemoryType {
    BLACK, WHITE, NONE, UNKNOWN
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

inline std::istream &operator>>(std::istream &is, PieceColor &color) {
    char ch;
    is >> ch;
    switch (ch) {
        case '.':
            color = PieceColor::NONE;
            break;
        case 'B':
            color = PieceColor::BLACK;
            break;
        case 'W':
            color = PieceColor::WHITE;
            break;
        default:
            color = PieceColor::UNKNOWN;
            break;
    }
    return is;
}

struct Position {
    unsigned int x;
    unsigned int y;

    Position(unsigned int x = 0, unsigned int y = 0) : x(x), y(y) {}

    friend std::ostream &operator<<(std::ostream &os, const Position &pos) {
        os << "(" << pos.x << ", " << pos.y << ")";
        return os;
    }

    bool operator==(const Position &rhs) const { return x == rhs.x && y == rhs.y; }

    bool operator!=(const Position &rhs) const { return !(*this == rhs); }
};

class Piece {
public:
    Piece() : position_({0, 0}), color_(PieceColor::NONE) {}

    Piece(unsigned int x, unsigned int y, PieceColor color) : position_({x, y}), color_(color) {}

    Piece(Position position, PieceColor color) : position_(position), color_(color) {}

    void SetPosition(Position position) { position_ = position; }

    void SetColor(PieceColor color) { color_ = color; }

    void Set(Position position, PieceColor color) {
        position_ = position;
        color_ = color;
    }

    Position GetPosition() const { return position_; }

    PieceColor GetColor() const { return color_; }

    friend std::ostream &operator<<(std::ostream &os, const Piece &piece) {
        os << piece.color_;
        return os;
    }

    Position position_;
    PieceColor color_;
};

#endif //PIECE_H
