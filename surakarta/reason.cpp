#include "reason.h"
#include <iostream>

bool IsLegalMoveReason(IllegalMoveReason reason) {
    switch (reason) {
        case IllegalMoveReason::LEGAL:
        case IllegalMoveReason::LEGAL_CAPTURE_MOVE:
        case IllegalMoveReason::LEGAL_NON_CAPTURE_MOVE:
            return true;
        default:
            return false;
    }
}

bool IsEndReason(EndReason reason) {
    switch (reason) {
        case EndReason::NONE:
            return false;
        default:
            return true;
    }
}

std::ostream &operator<<(std::ostream &os, const IllegalMoveReason &reason) {
    switch (reason) {
        case IllegalMoveReason::LEGAL:
            os << "LEGAL";
            break;
        case IllegalMoveReason::LEGAL_CAPTURE_MOVE:
            os << "LEGAL_CAPTURE_MOVE";
            break;
        case IllegalMoveReason::LEGAL_NON_CAPTURE_MOVE:
            os << "LEGAL_NON_CAPTURE_MOVE";
            break;
        case IllegalMoveReason::ILLIGAL:
            os << "ILLIGAL";
            break;
        case IllegalMoveReason::NOT_PLAYER_TURN:
            os << "NOT_PLAYER_TURN";
            break;
        case IllegalMoveReason::OUT_OF_BOARD:
            os << "OUT_OF_BOARD";
            break;
        case IllegalMoveReason::NOT_PIECE:
            os << "NOT_PIECE";
            break;
        case IllegalMoveReason::NOT_PLAYER_PIECE:
            os << "NOT_PLAYER_PIECE";
            break;
        case IllegalMoveReason::ILLIGAL_CAPTURE_MOVE:
            os << "ILLIGAL_CAPTURE_MOVE";
            break;
        case IllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE:
            os << "ILLIGAL_NON_CAPTURE_MOVE";
            break;
        case IllegalMoveReason::GAME_ALREADY_END:
            os << "GAME_ALREADY_END";
            break;
        case IllegalMoveReason::GAME_NOT_START:
            os << "GAME_NOT_START";
            break;
        default:
            os << "UNKNOWN";
            break;
    }
    return os;
}

std::istream &operator>>(std::istream &is, IllegalMoveReason &reason) {
    std::string str;
    is >> str;
    if (str == "LEGAL") {
        reason = IllegalMoveReason::LEGAL;
    } else if (str == "LEGAL_CAPTURE_MOVE") {
        reason = IllegalMoveReason::LEGAL_CAPTURE_MOVE;
    } else if (str == "LEGAL_NON_CAPTURE_MOVE") {
        reason = IllegalMoveReason::LEGAL_NON_CAPTURE_MOVE;
    } else if (str == "ILLIGAL") {
        reason = IllegalMoveReason::ILLIGAL;
    } else if (str == "NOT_PLAYER_TURN") {
        reason = IllegalMoveReason::NOT_PLAYER_TURN;
    } else if (str == "OUT_OF_BOARD") {
        reason = IllegalMoveReason::OUT_OF_BOARD;
    } else if (str == "NOT_PIECE") {
        reason = IllegalMoveReason::NOT_PIECE;
    } else if (str == "NOT_PLAYER_PIECE") {
        reason = IllegalMoveReason::NOT_PLAYER_PIECE;
    } else if (str == "ILLIGAL_CAPTURE_MOVE") {
        reason = IllegalMoveReason::ILLIGAL_CAPTURE_MOVE;
    } else if (str == "ILLIGAL_NON_CAPTURE_MOVE") {
        reason = IllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
    } else if (str == "GAME_ALREADY_END") {
        reason = IllegalMoveReason::GAME_ALREADY_END;
    } else if (str == "GAME_NOT_START") {
        reason = IllegalMoveReason::GAME_NOT_START;
    } else {
        reason = IllegalMoveReason::ILLIGAL;
    }
    return is;
}

std::ostream &operator<<(std::ostream &os, const EndReason &reason) {
    switch (reason) {
        case EndReason::NONE:
            os << "NONE";
            break;
        case EndReason::STALEMATE:
            os << "STALEMATE";
            break;
        case EndReason::CHECKMATE:
            os << "CHECKMATE";
            break;
        case EndReason::TRAPPED:
            os << "TRAPPED";
            break;
        case EndReason::RESIGN:
            os << "RESIGN";
            break;
        case EndReason::TIMEOUT:
            os << "TIMEOUT";
            break;
        case EndReason::ILLIGAL_MOVE:
            os << "ILLIGAL_MOVE";
            break;
        default:
            os << "UNKNOWN";
            break;
    }
    return os;
}

std::istream &operator>>(std::istream &is, EndReason &reason) {
    std::string str;
    is >> str;
    if (str == "NONE") {
        reason = EndReason::NONE;
    } else if (str == "STALEMATE") {
        reason = EndReason::STALEMATE;
    } else if (str == "CHECKMATE") {
        reason = EndReason::CHECKMATE;
    } else if (str == "TRAPPED") {
        reason = EndReason::TRAPPED;
    } else if (str == "RESIGN") {
        reason = EndReason::RESIGN;
    } else if (str == "TIMEOUT") {
        reason = EndReason::TIMEOUT;
    } else if (str == "ILLIGAL_MOVE") {
        reason = EndReason::ILLIGAL_MOVE;
    } else {
        reason = EndReason::NONE;
    }
    return is;
}
