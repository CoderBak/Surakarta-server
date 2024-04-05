#include "game.h"
#include <fstream>
#include <iostream>

void Game::StartGame(const std::string &file_name) {
    if (file_name.empty()) {
        for (unsigned int y = 0; y < boardSize; y++) {
            for (unsigned int x = 0; x < boardSize; x++) {
                if (y < 2) {
                    (*board)[x][y] = std::make_shared<Piece>(x, y, PieceColor::BLACK);
                } else if (y >= boardSize - 2) {
                    (*board)[x][y] = std::make_shared<Piece>(x, y, PieceColor::WHITE);
                } else {
                    (*board)[x][y] = std::make_shared<Piece>(x, y, PieceColor::NONE);
                }
            }
        }
        gameInfo->reset();
    } else {
        std::ifstream fin(file_name);
        fin >> (*board);
        fin >> (*gameInfo);
        fin.close();
    }
    updateBoard();
}

void Game::SaveGame(const std::string &file_name) const {
    std::ofstream fout(file_name);
    fout << (*board);
    fout << (*gameInfo);
    fout.close();
}

void Game::UpdateGameInfo(MoveReason moveReason, EndReason endReason, Player winner) const {
    if (moveReason == MoveReason::LEGAL_CAPTURE_MOVE) {
        gameInfo->lastCapturedRound = gameInfo->numRound;
    }
    if (!IsEndReason(endReason)) {
        gameInfo->currentPlayer = ReverseColor(gameInfo->currentPlayer);
        gameInfo->numRound++;
    } else {
        gameInfo->endReason = endReason;
        gameInfo->winner = winner;
    }
}

MoveResponse Game::Move(const class Move &move) {
    std::cerr << move.player << " " << move.from << " " << move.to << std::endl;
    MoveReason moveReason = ruleManager->JudgeMove(move);
    auto [endReason, winner] = ruleManager->JudgeEnd(moveReason);
    UpdateGameInfo(moveReason, endReason, winner);
    if (moveReason == MoveReason::LEGAL_NON_CAPTURE_MOVE) {
        std::swap((*board)[move.to.x][move.to.y], (*board)[move.from.x][move.from.y]);
        (*board)[move.to.x][move.to.y]->SetPosition(move.to);
        (*board)[move.from.x][move.from.y]->SetPosition(move.from);
        updateBoard();
    } else if (moveReason == MoveReason::LEGAL_CAPTURE_MOVE) {
        (*board)[move.to.x][move.to.y] = (*board)[move.from.x][move.from.y];
        (*board)[move.to.x][move.to.y]->SetPosition(move.to);
        (*board)[move.from.x][move.from.y] =
                std::make_shared<Piece>(move.from.x, move.from.y, PieceColor::NONE);
        updateBoard();
    }
    MoveResponse response(moveReason, endReason, winner);
    return response;
}
