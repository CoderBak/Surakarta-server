#ifndef AGENT_RANDOM_H
#define AGENT_RANDOM_H

#include <memory>
//#include "agent_base.h"
#include <QThread>
#include <QDebug>
#include "../basic.h"
#include "../rule_manager.h"

class AgentRandom : public QObject {
    Q_OBJECT
public:
    //using AgentBase::AgentBase;
    AgentRandom(const std::shared_ptr<Board> &board, const std::shared_ptr<GameInfo> &game_info,
              const std::shared_ptr<RuleManager> &ruleManager) :
        boardSize(board->boardSize), board(std::const_pointer_cast<const Board>(board)),
        gameInfo(std::const_pointer_cast<const GameInfo>(game_info)), ruleManager(ruleManager),running(false) {}

    // ~AgentRandom(){
    //     stop();
    //     wait();
    // }

    Move CalculateMove();

signals:
    void moveCalculated(const Move &move);

protected:
    unsigned int boardSize;
    std::shared_ptr<const Board> board;
    std::shared_ptr<const GameInfo> gameInfo;
    std::shared_ptr<RuleManager> ruleManager;
    std::atomic<bool> running;
};

#endif //AGENT_RANDOM_H

