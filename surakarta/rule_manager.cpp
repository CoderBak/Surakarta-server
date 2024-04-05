#include "rule_manager.h"
#include <iostream>
#include <memory>
#include <vector>

// Get the next postition and direction of a move.
std::pair<Position, Direction> get_next_pos(const Position from, const Direction dir, const int n) {
    int new_x = static_cast<int>(from.x);
    int new_y = static_cast<int>(from.y);
    if (dir == Direction::NORTH) { new_y -= 1; }
    if (dir == Direction::SOUTH) { new_y += 1; }
    if (dir == Direction::EAST) { new_x += 1; }
    if (dir == Direction::WEST) { new_x -= 1; }
    // gen packs the parameters together.
    auto gen = [](auto a, auto b, auto c) { return std::make_pair(Position(a, b), c); };
    // if we don't need to consider corner cases.
    if (0 <= new_x && new_x < n && 0 <= new_y && new_y < n) {
        return gen(new_x, new_y, dir);
    }
    const int x = static_cast<int>(from.x);
    const int y = static_cast<int>(from.y);
    const auto idx = (x > y) ? x - y : y - x;
    // calculate the next position when we are reaching the edge.
    if (x == 0 || x == n - 1) {
        return (2 * y < n) ? gen(idx, 0, Direction::SOUTH) : gen(n - 1 - idx, n - 1, Direction::NORTH);
    }
    return (2 * x < n) ? gen(0, idx, Direction::EAST) : gen(n - 1, n - 1 - idx, Direction::WEST);
}

// Search for all the reachable points.
bool search(const Position from, const Position to,
            const std::shared_ptr<const Board> &board, const unsigned int n) {
    std::vector<Direction> valid_directions;
    // construct all the valid directions.
    if (from.x != 0 && from.x != n - 1) {
        valid_directions.push_back(Direction::NORTH);
        valid_directions.push_back(Direction::SOUTH);
    }
    if (from.y != 0 && from.y != n - 1) {
        valid_directions.push_back(Direction::EAST);
        valid_directions.push_back(Direction::WEST);
    }
    for (const auto val_dir: valid_directions) {
        Position cur = from;
        Direction cur_dir = val_dir;
        // flag: if a capture can be made.
        // rotated: if we have performed a rotate.
        bool flag = false, rotated = false;
        for (unsigned int k = 1; k < 4 * n; k += 1) {
            const auto [new_pos, new_dir] = get_next_pos(cur, cur_dir, static_cast<int>(n));
            if (cur_dir != new_dir) { rotated = true; }
            cur_dir = new_dir;
            cur.x = new_pos.x;
            cur.y = new_pos.y;
            if (cur == to) { flag = rotated; }
            if ((*board)[cur.x][cur.y]->GetColor() != PieceColor::NONE) { break; }
        }
        if (flag) { return true; }
    }
    return false;
}

// check if a capture move is valid.
bool check_valid_move(const Position from, const Position to,
                      const std::shared_ptr<const Board> &board, const unsigned int n) {
    const Position cornerA(0, 0), cornerB(n - 1, n - 1);
    const Position cornerC(0, n - 1), cornerD(n - 1, 0);
    if (from == cornerA || from == cornerB || from == cornerC || from == cornerD) { return false; }
    const auto origin_color = (*board)[from.x][from.y]->GetColor();
    (*board)[from.x][from.y]->SetColor(PieceColor::NONE);
    const bool result = search(from, to, board, n);
    (*board)[from.x][from.y]->SetColor(origin_color);
    return result;
}

void RuleManager::OnUpdateBoard() {
    // TODO:
    // Every time the board and game_info is updated to the next round version, this function will be called.
    // You don't need to implement this function if you don't need it.
    // A more delicate way is to use Qt's signal and slot mechanism, but that's the advanced part.
}

IllegalMoveReason RuleManager::JudgeMove(const Move &move) {
    auto [from, to, player] = move;
    const Player current_player = game_info_->currentPlayer;
    auto is_owner = [](auto &_player, auto &_piece) {
        return (_player == Player::BLACK && _piece->GetColor() == PieceColor::BLACK) ||
               (_player == Player::WHITE && _piece->GetColor() == PieceColor::WHITE);
    };
    // check if the move is not player turn
    if (current_player != player) {
        return IllegalMoveReason::NOT_PLAYER_TURN;
    }
    // check if the pos is valid -> OUT_OF_BOARD
    auto check_pos_valid = [=](auto &pos) {
        return 0 <= pos.x && pos.x < board_size_ && 0 <= pos.y && pos.y < board_size_;
    };
    if (!check_pos_valid(from) || !check_pos_valid(to)) {
        return IllegalMoveReason::OUT_OF_BOARD;
    }
    // check if the pos is able to move -> NOT_PIECE
    const auto to_move = (*board_)[from.x][from.y];
    if (to_move == nullptr) {
        return IllegalMoveReason::NOT_PIECE;
    }
    const PieceColor color = to_move->GetColor();
    if (color != PieceColor::BLACK && color != PieceColor::WHITE) {
        return IllegalMoveReason::NOT_PIECE;
    }
    // check if current player can move the pos -> NOT_PLAYER_PIECE
    if (!is_owner(current_player, to_move)) {
        return IllegalMoveReason::NOT_PLAYER_PIECE;
    }
    // check other situations
    const auto target = (*board_)[to.x][to.y];
    if (is_owner(current_player, target)) {
        return IllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
    }
    if (target->GetColor() == PieceColor::NONE) {
        // this is a non_capture move
        if (std::abs(static_cast<int>(to.x) - static_cast<int>(from.x)) <= 1 &&
            std::abs(static_cast<int>(to.y) - static_cast<int>(from.y)) <= 1) {
            return IllegalMoveReason::LEGAL_NON_CAPTURE_MOVE;
        }
        return IllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
    }
    // this a capture move
    if (check_valid_move(from, to, board_, board_size_)) {
        return IllegalMoveReason::LEGAL_CAPTURE_MOVE;
    }
    return IllegalMoveReason::ILLIGAL_CAPTURE_MOVE;
}

std::pair<EndReason, Player> RuleManager::JudgeEnd(const IllegalMoveReason reason) {
    // Note that at this point, the board and game_info have not been updated yet.
    // Return the end reason and the winner in this function.
    const auto player = game_info_->currentPlayer;
    if (reason != IllegalMoveReason::LEGAL_CAPTURE_MOVE &&
        reason != IllegalMoveReason::LEGAL_NON_CAPTURE_MOVE) {
        return std::make_pair(EndReason::ILLIGAL_MOVE, ReverseColor(player));
    }
    // count the black and white piece on the board.
    int white_cnt = 0, black_cnt = 0;
    for (unsigned int i = 0; i < board_size_; i += 1) {
        for (unsigned int j = 0; j < board_size_; j += 1) {
            const auto cur = (*board_)[i][j]->GetColor();
            black_cnt += cur == PieceColor::BLACK ? 1 : 0;
            white_cnt += cur == PieceColor::WHITE ? 1 : 0;
        }
    }
    if (reason == IllegalMoveReason::LEGAL_CAPTURE_MOVE) {
        // check if CHECKMATE happens
        if (white_cnt == 1 && player == Player::BLACK) {
            return std::make_pair(EndReason::CHECKMATE, Player::BLACK);
        }
        if (black_cnt == 1 && player == Player::WHITE) {
            return std::make_pair(EndReason::CHECKMATE, Player::WHITE);
        }
    } else {
        // check if STALEMATE happens
        if (game_info_->numRound - game_info_->lastCapturedRound >= game_info_->maxNoCaptureRound) {
            auto winner = Player::NONE;
            if (white_cnt > black_cnt) {
                winner = Player::WHITE;
            }
            if (white_cnt < black_cnt) {
                winner = Player::BLACK;
            }
            return std::make_pair(EndReason::STALEMATE, winner);
        }
    }
    return std::make_pair(EndReason::NONE, Player::NONE);
}

std::unique_ptr<std::vector<Position>>
RuleManager::GetAllLegalTarget(const Position postion [[maybe_unused]]) {
    // We don't test this function, you don't need to implement this function if you don't need it.
    return std::make_unique<std::vector<Position>>();
}

void RuleManager::HelloWorld() { std::cout << "Hello World!" << std::endl; }
