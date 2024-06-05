#include "agent_ai.h"
#include <algorithm>
#include <vector>
#include "../../utils/global_random_generator.h"
#include <cmath>
#include <memory>
#include <queue>

using pairs = std::pair<Position, Position>;

// The same from rule_manager.cpp
// But add all the valid points into a list.
std::vector<Position> search_pos(const Position from, const std::shared_ptr<const Board> &board,
                                 const unsigned int n) {
    const auto origin_color = (*board)[from.x][from.y]->GetColor();
    (*board)[from.x][from.y]->SetColor(PieceColor::NONE);
    std::vector<Direction> valid_directions;
    std::vector<Position> valid_eat;
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
        bool rotated = false;
        for (unsigned int k = 1; k < 4 * n; k += 1) {
            const auto [new_pos, new_dir] = get_next_pos(cur, cur_dir, static_cast<int>(n));
            if (cur_dir != new_dir) {
                rotated = true;
            }
            cur_dir = new_dir;
            cur.x = new_pos.x;
            cur.y = new_pos.y;
            if (const auto cur_color = (*board)[cur.x][cur.y]->GetColor(); cur_color != PieceColor::NONE) {
                if (cur_color != origin_color && rotated) {
                    valid_eat.push_back(cur);
                }
                break;
            }
        }
    }
    (*board)[from.x][from.y]->SetColor(origin_color);
    return valid_eat;
}

Move AgentAI::CalculateMove() {
    const auto self = gameInfo->currentPlayer;
    const int n = static_cast<int>(boardSize);
    auto strategy = [=](const pairs &x, const pairs &y) -> bool {
        // Move x and y, which is better?
        auto func = [=](const pairs &a) {
            const auto x_coord = static_cast<double>(a.second.x);
            const auto y_coord = static_cast<double>(a.second.y);
            return std::abs(x_coord - (n - 1) * 1.0 / 2) + std::abs(y_coord - (n - 1) * 1.0 / 2);
        };
        return func(x) > func(y);
    };
    std::priority_queue<pairs, std::vector<pairs>, decltype(strategy)> all_valid_moves(strategy);
    std::vector<Position> from;
    for (int i = 0; i < n; i += 1) {
        for (int j = 0; j < n; j += 1) {
            if ((*board)[i][j]->GetColor() == self) {
                const auto current_pos = Position(i, j);
                auto eatable = search_pos(current_pos, board, n);
                for (auto obj: eatable) {
                    all_valid_moves.emplace(current_pos, obj);
                }
                from.push_back(current_pos);
            }
        }
    }
    if (!all_valid_moves.empty()) {
        const auto best = all_valid_moves.top();
        return {best.first, best.second, self};
    }
    std::vector<std::pair<Position, Position>> valid_noncapture;
    for (const auto from_pos: from) {
        const int x = static_cast<int>(from_pos.x);
        const int y = static_cast<int>(from_pos.y);
        for (int dx = -1; dx <= 1; dx += 1) {
            for (int dy = -1; dy <= 1; dy += 1) {
                const int new_x = x + dx, new_y = y + dy;
                if (0 <= new_x && new_x < n && 0 <= new_y && new_y < n) {
                    if ((*board)[new_x][new_y]->GetColor() == PieceColor::NONE) {
                        valid_noncapture.emplace_back(from_pos, Position(new_x, new_y));
                    }
                }
            }
        }
    }
    if (valid_noncapture.empty()) {
        // No move is valid. return invalid one.
        return {{0, 0}, {0, 0}, self};
    }
    std::shuffle(valid_noncapture.begin(), valid_noncapture.end(), GlobalRandomGenerator::getInstance());
    const auto chosen = valid_noncapture.front();
    return {chosen.first, chosen.second, self};
}