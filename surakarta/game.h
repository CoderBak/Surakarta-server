#ifndef GAME_H
#define GAME_H

#pragma once

#include "basic.h"
#include "rule_manager.h"

using pair = std::pair<Position, std::vector<Position>>;

class MoveResponse {
public:
    explicit MoveResponse(MoveReason moveReason, EndReason endReason,
                          Player winner) :
            moveReason(moveReason), endReason(endReason), winner(winner) {}

    MoveReason moveReason;
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

    void StartGame(const std::string &file_name = "");

    void SaveGame(const std::string &file_name) const;

    // Update game info, this function should be called after each move.
    void UpdateGameInfo(MoveReason moveReason, EndReason endReason, Player winner) const;

    // Move a piece, and update game info.
    MoveResponse Move(const Move &move);

    std::shared_ptr<Board> GetBoard() const { return board; }

    std::shared_ptr<GameInfo> GetGameInfo() const { return gameInfo; }

    bool IsEnd() const { return gameInfo->IsEnd(); }

    unsigned int boardSize;
    std::shared_ptr<Board> board;
    std::shared_ptr<GameInfo> gameInfo;
    std::shared_ptr<RuleManager> ruleManager;

    void updateBoard() {
        std::ostringstream oss;
        oss << (*board);
        emit boardUpdated(QString::fromStdString(oss.str()));
    }

    std::vector<pair> searchEatable(const Position &from) const;

    std::vector<Position> searchMovable(const Position &from) const;

signals:

    void boardUpdated(const QString &boardInfo);
};


#endif //GAME_H
