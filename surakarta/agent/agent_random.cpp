#include "agent_random.h"
#include <algorithm>
#include <vector>
#include "../../utils/global_random_generator.h"

Move AgentRandom::CalculateMove() {
    std::vector<Position> from;
    std::vector<Position> to;
    for (unsigned int i = 0; i < board_size_; i++) {
        for (unsigned int j = 0; j < board_size_; j++) {
            Position position = {i, j};
            if ((*board_)[i][j]->GetColor() == game_info_->currentPlayer) {
                from.push_back(position);
            }
            else {
                to.push_back(position);
            }
        }
    }
    std::shuffle(from.begin(), from.end(), GlobalRandomGenerator::getInstance());
    std::shuffle(to.begin(), to.end(), GlobalRandomGenerator::getInstance());
    Move rd_move({0, 0}, {0, 0}, game_info_->currentPlayer);
    for (auto &p1: from) {
        for (auto &p2: to) {
            Move move = {p1, p2, game_info_->currentPlayer};
            IllegalMoveReason reason = rule_manager_->JudgeMove(move);
            if (reason == IllegalMoveReason::LEGAL_CAPTURE_MOVE) {
                return move;
            }
            else if (reason == IllegalMoveReason::LEGAL_NON_CAPTURE_MOVE) {
                rd_move = move;
            }
        }
    }
    return rd_move;
}