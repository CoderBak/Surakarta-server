//
// Created by CoderBak on 2024/4/5.
//

#ifndef AGENT_BASE_H
#define AGENT_BASE_H

#pragma once

#include <memory>
#include <QDebug>
#include "../basic.h"
#include "../rule_manager.h"

class AgentBase {
public:
    AgentBase(const std::shared_ptr<Board> &board, const std::shared_ptr<GameInfo> &game_info,
              const std::shared_ptr<RuleManager> &rule_manager) :
            board_size_(board->board_size_), board_(std::const_pointer_cast<const Board>(board)),
            game_info_(std::const_pointer_cast<const GameInfo>(game_info)), rule_manager_(rule_manager) {}

    virtual Move CalculateMove() {
        return Move{Position(0, 0), Position(0, 0), Player::NONE};
    }


protected:
    unsigned int board_size_;
    std::shared_ptr<const Board> board_;
    std::shared_ptr<const GameInfo> game_info_;
    std::shared_ptr<RuleManager> rule_manager_;
};

#endif //AGENT_BASE_H
