// In this file, we define the Game and MoveResponse class.
#ifndef GAME_H
#define GAME_H

#pragma once

#include <sstream>
#include "basic.h"
#include "rule_manager.h"

class MoveResponse {
public:
    explicit MoveResponse(EndReason endReason, Player winner) :
            endReason(endReason), winner(winner) {}

    EndReason endReason;
    Player winner;
};

class Game : public QObject {
Q_OBJECT

public:
    explicit Game(const unsigned boardSize = BOARD_SIZE, const unsigned int maxNoCaptureRound = maxNoCapture) :
            boardSize(boardSize), board(std::make_shared<Board>(boardSize)),
            gameInfo(std::make_shared<GameInfo>(maxNoCaptureRound)),
            ruleManager(std::make_shared<RuleManager>(board, gameInfo)) {}

    void updateBoard() {
        std::ostringstream oss;
        oss << (*board);
        emit boardUpdated(QString::fromStdString(oss.str()));
    }

    void StartGame();
    void UpdateGameInfo(MoveReason moveReason, EndReason endReason, Player winner) const;
    MoveResponse Move(const Move &move);
    [[nodiscard]] bool isEnd() const { return gameInfo->endReason != EndReason::NONE; }
    [[nodiscard]] std::vector<pair> searchEatable(const Position &from) const;
    [[nodiscard]] std::vector<Position> searchMovable(const Position &from) const;

    unsigned int boardSize;
    std::shared_ptr<Board> board;
    std::shared_ptr<GameInfo> gameInfo;
    std::shared_ptr<RuleManager> ruleManager;

signals:
    void boardUpdated(const QString &boardInfo);
};

#endif //GAME_H
