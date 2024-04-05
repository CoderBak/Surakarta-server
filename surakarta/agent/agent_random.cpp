#include "agent_random.h"
#include <algorithm>
#include <vector>
#include "../../utils/global_random_generator.h"

Move AgentRandom::CalculateMove() {
    std::vector<Position> from;
    std::vector<Position> to;
    for (unsigned int i = 0; i < boardSize; i++) {
        for (unsigned int j = 0; j < boardSize; j++) {
            Position position = {i, j};
            if ((*board)[i][j]->GetColor() == gameInfo->currentPlayer) {
                from.push_back(position);
            } else {
                to.push_back(position);
            }
        }
    }
    std::shuffle(from.begin(), from.end(), GlobalRandomGenerator::getInstance());
    std::shuffle(to.begin(), to.end(), GlobalRandomGenerator::getInstance());
    Move rd_move({0, 0}, {0, 0}, gameInfo->currentPlayer);
    for (auto &p1: from) {
        for (auto &p2: to) {
            Move move = {p1, p2, gameInfo->currentPlayer};
            MoveReason reason = ruleManager->JudgeMove(move);
            if (reason == MoveReason::LEGAL_CAPTURE_MOVE) {
                return move;
            } else if (reason == MoveReason::LEGAL_NON_CAPTURE_MOVE) {
                rd_move = move;
            }
        }
    }
    return rd_move;
}