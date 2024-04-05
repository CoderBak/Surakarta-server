#include "reason.h"
#include <iostream>

bool IsEndReason(EndReason reason) {
    return reason != EndReason::NONE;
}

std::ostream &operator<<(std::ostream &os, const MoveReason &reason) {
    switch (reason) {
        case MoveReason::LEGAL:
            os << "LEGAL";
            break;
        case MoveReason::LEGAL_CAPTURE_MOVE:
            os << "LEGAL_CAPTURE_MOVE";
            break;
        case MoveReason::LEGAL_NON_CAPTURE_MOVE:
            os << "LEGAL_NON_CAPTURE_MOVE";
            break;
        case MoveReason::ILLEGAL:
            os << "ILLEGAL";
            break;
        case MoveReason::NOT_PLAYER_TURN:
            os << "NOT_PLAYER_TURN";
            break;
        case MoveReason::OUT_OF_BOARD:
            os << "OUT_OF_BOARD";
            break;
        case MoveReason::NOT_PIECE:
            os << "NOT_PIECE";
            break;
        case MoveReason::NOT_PLAYER_PIECE:
            os << "NOT_PLAYER_PIECE";
            break;
        case MoveReason::ILLEGAL_CAPTURE_MOVE:
            os << "ILLEGAL_CAPTURE_MOVE";
            break;
        case MoveReason::ILLEGAL_NON_CAPTURE_MOVE:
            os << "ILLEGAL_NON_CAPTURE_MOVE";
            break;
        case MoveReason::GAME_ALREADY_END:
            os << "GAME_ALREADY_END";
            break;
        case MoveReason::GAME_NOT_START:
            os << "GAME_NOT_START";
            break;
        default:
            os << "UNKNOWN";
            break;
    }
    return os;
}

std::istream &operator>>(std::istream &is, MoveReason &reason) {
    std::string str;
    is >> str;
    if (str == "LEGAL") {
        reason = MoveReason::LEGAL;
    } else if (str == "LEGAL_CAPTURE_MOVE") {
        reason = MoveReason::LEGAL_CAPTURE_MOVE;
    } else if (str == "LEGAL_NON_CAPTURE_MOVE") {
        reason = MoveReason::LEGAL_NON_CAPTURE_MOVE;
    } else if (str == "ILLEGAL") {
        reason = MoveReason::ILLEGAL;
    } else if (str == "NOT_PLAYER_TURN") {
        reason = MoveReason::NOT_PLAYER_TURN;
    } else if (str == "OUT_OF_BOARD") {
        reason = MoveReason::OUT_OF_BOARD;
    } else if (str == "NOT_PIECE") {
        reason = MoveReason::NOT_PIECE;
    } else if (str == "NOT_PLAYER_PIECE") {
        reason = MoveReason::NOT_PLAYER_PIECE;
    } else if (str == "ILLEGAL_CAPTURE_MOVE") {
        reason = MoveReason::ILLEGAL_CAPTURE_MOVE;
    } else if (str == "ILLEGAL_NON_CAPTURE_MOVE") {
        reason = MoveReason::ILLEGAL_NON_CAPTURE_MOVE;
    } else if (str == "GAME_ALREADY_END") {
        reason = MoveReason::GAME_ALREADY_END;
    } else if (str == "GAME_NOT_START") {
        reason = MoveReason::GAME_NOT_START;
    } else {
        reason = MoveReason::ILLEGAL;
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
        case EndReason::ILLEGAL_MOVE:
            os << "ILLEGAL_MOVE";
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
    if (str == "STALEMATE") {
        reason = EndReason::STALEMATE;
    } else if (str == "CHECKMATE") {
        reason = EndReason::CHECKMATE;
    } else if (str == "TRAPPED") {
        reason = EndReason::TRAPPED;
    } else if (str == "RESIGN") {
        reason = EndReason::RESIGN;
    } else if (str == "TIMEOUT") {
        reason = EndReason::TIMEOUT;
    } else if (str == "ILLEGAL_MOVE") {
        reason = EndReason::ILLEGAL_MOVE;
    } else {
        reason = EndReason::NONE;
    }
    return is;
}
