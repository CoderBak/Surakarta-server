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
              const std::shared_ptr<RuleManager> &ruleManager) :
            boardSize(board->boardSize), board(std::const_pointer_cast<const Board>(board)),
            gameInfo(std::const_pointer_cast<const GameInfo>(game_info)), ruleManager(ruleManager) {}

    virtual Move CalculateMove() {
        return Move{Position(0, 0), Position(0, 0), Player::NONE};
    }


protected:
    unsigned int boardSize;
    std::shared_ptr<const Board> board;
    std::shared_ptr<const GameInfo> gameInfo;
    std::shared_ptr<RuleManager> ruleManager;
};

#endif //AGENT_BASE_H
