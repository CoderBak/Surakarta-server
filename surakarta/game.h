#ifndef GAME_H
#define GAME_H

#pragma once

#include "agent/agent_base.h"
#include "basic.h"
#include "rule_manager.h"

class MoveResponse {
public:
    MoveResponse(IllegalMoveReason move_reason) :
            move_reason_(move_reason), end_reason_(EndReason::NONE), winner_(Player::NONE) {}

    MoveResponse(IllegalMoveReason move_reason, EndReason end_reason,
                 Player winner) :
            move_reason_(move_reason), end_reason_(end_reason), winner_(winner) {}

    bool IsLegal() const { return IsLegalMoveReason(move_reason_); }

    bool IsEnd() const { return end_reason_ != EndReason::NONE; }

    IllegalMoveReason GetMoveReason() const { return move_reason_; }

    EndReason GetEndReason() const { return end_reason_; }

    Player GetWinner() const { return winner_; }

    //    private:
    IllegalMoveReason move_reason_;
    EndReason end_reason_;
    Player winner_;
};

class Game {
public:
    explicit Game(unsigned board_size = BOARD_SIZE, unsigned int max_no_capture_round = 40) :
            board_size_(board_size), board_(std::make_shared<Board>(board_size)),
            game_info_(std::make_shared<GameInfo>(max_no_capture_round)),
            rule_manager_(std::make_shared<RuleManager>(board_, game_info_)),
            agent_(std::make_shared<AgentBase>(board_, game_info_, rule_manager_)) {}

    /**
     * @brief Start the game. If file_name is empty, the board will be initialized to the initial state. Otherwise, the
     * board will be initialized to the state in the file.
     */
    void StartGame(std::string file_name = "");

    /**
     * @brief Save the game to a file.
     * @param file_name The file name.
     */
    void SaveGame(std::string file_name) const;

    /**
     * @brief Update game info, this function should be called after each move.
     * @param reason IllegalMoveReason.
     */
    void UpdateGameInfo(IllegalMoveReason move_reason, EndReason end_reason, Player winner);

    /**
     * @brief Move a piece, and update game info.
     * @param move The move to be made.
     * @return See MoveResponse.
     */
    MoveResponse Move(const Move &move);

    std::shared_ptr<Board> GetBoard() const { return board_; }

    std::shared_ptr<GameInfo> GetGameInfo() const { return game_info_; }

    bool IsEnd() const { return game_info_->IsEnd(); }

    unsigned int board_size_;
    std::shared_ptr<Board> board_;
    std::shared_ptr<GameInfo> game_info_;
    std::shared_ptr<RuleManager> rule_manager_;
    std::shared_ptr<AgentBase> agent_;
};


#endif //_SERVER_GAME_H
