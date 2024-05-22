#include "game.h"

void Game::StartGame() {
    for (unsigned int y = 0; y < boardSize; y += 1) {
        for (unsigned int x = 0; x < boardSize; x += 1) {
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
    updateBoard();
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
    MoveResponse response(endReason, winner);
    return response;
}

// Checks if the coordinates are in the boundary.
bool checkBound(const int x, const int y, const int boundMin, const int boundMax) {
    return boundMin <= x && x < boundMax && boundMin <= y && y < boundMax;
}

// Get the next position and direction of a move.
std::pair<Position, Direction> getNextPos(const Position from, const Direction dir, const int n) {
    int new_x = static_cast<int>(from.x);
    int new_y = static_cast<int>(from.y);
    if (dir == Direction::NORTH) { new_y -= 1; }
    if (dir == Direction::SOUTH) { new_y += 1; }
    if (dir == Direction::EAST) { new_x += 1; }
    if (dir == Direction::WEST) { new_x -= 1; }
    // gen packs the parameters together.
    auto gen = [](auto a, auto b, auto c) { return std::make_pair(Position(a, b), c); };
    // if we don't need to consider corner cases.
    if (0 <= new_x && new_x < n && 0 <= new_y && new_y < n) {
        return gen(new_x, new_y, dir);
    }
    const int x = static_cast<int>(from.x);
    const int y = static_cast<int>(from.y);
    const auto idx = (x > y) ? x - y : y - x;
    // calculate the next position when we are reaching the edge.
    if (x == 0 || x == n - 1) {
        return (2 * y < n) ? gen(idx, 0, Direction::SOUTH) : gen(n - 1 - idx, n - 1, Direction::NORTH);
    }
    return (2 * x < n) ? gen(0, idx, Direction::EAST) : gen(n - 1, n - 1 - idx, Direction::WEST);
}

// Find all eatable places and their path.
std::vector<pair> Game::searchEatable(const Position &from) const {
    const auto originColor = (*board)[from.x][from.y]->GetColor();
    const unsigned int n = board->boardSize;
    (*board)[from.x][from.y]->SetColor(PieceColor::NONE);
    std::vector<Direction> validDirections;
    std::vector<pair> validEat;
    // find all valid search directions
    if (from.x != 0 && from.x != n - 1) {
        validDirections.push_back(Direction::NORTH);
        validDirections.push_back(Direction::SOUTH);
    }
    if (from.y != 0 && from.y != n - 1) {
        validDirections.push_back(Direction::EAST);
        validDirections.push_back(Direction::WEST);
    }
    for (const auto valDir: validDirections) {
        Position cur = from;
        Direction curDir = valDir;
        std::vector<Position> path;
        bool rotated = false;
        for (unsigned int k = 1; k < 4 * n; k += 1) {
            const auto [newPos, newDir] = getNextPos(cur, curDir, static_cast<int>(n));
            if (curDir != newDir) { rotated = true; }
            curDir = newDir;
            cur = newPos;
            path.push_back(cur);
            if ((*board)[cur.x][cur.y]->GetColor() != PieceColor::NONE) {
                if ((*board)[cur.x][cur.y]->GetColor() != originColor && rotated) {
                    validEat.emplace_back(cur, path);
                }
                break;
            }
        }
    }
    (*board)[from.x][from.y]->SetColor(originColor);
    return validEat;
}

// Find all movable places. (non-capture)
std::vector<Position> Game::searchMovable(const Position &from) const {
    std::vector<Position> validNonCapture;
    const int x = static_cast<int>(from.x);
    const int y = static_cast<int>(from.y);
    for (int dx = -1; dx <= 1; dx += 1) {
        for (int dy = -1; dy <= 1; dy += 1) {
            const int new_x = x + dx, new_y = y + dy;
            if (checkBound(new_x, new_y, 0, static_cast<int>(BOARD_SIZE))) {
                if ((*board)[new_x][new_y]->GetColor() == PieceColor::NONE) {
                    validNonCapture.emplace_back(new_x, new_y);
                }
            }
        }
    }
    return validNonCapture;
}