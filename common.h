/**
 * In this file, we define the constants in backend, and basic classes.
 */
#pragma once

#include <QtWidgets>

constexpr unsigned int BOARD_SIZE = 6;
constexpr unsigned int maxNoCapture = 40;
constexpr int WIDTH = 1180;
constexpr int HEIGHT = 820;
constexpr int BOARD_HEIGHT = 720 / 2;
constexpr int DELTA_Y = HEIGHT / 2 - BOARD_HEIGHT / 2;
constexpr int DELTA_X = 560;
constexpr int cellSize = BOARD_HEIGHT / BOARD_SIZE;
constexpr int PEN_WIDTH = 2;
constexpr auto PATH_COLOR = QColor(173, 216, 230);
constexpr auto DEFAULT_COLOR = Qt::black;
constexpr auto BACK_COLOR = Qt::yellow;
constexpr int chessRadius = static_cast<int>(0.35 * cellSize);
constexpr auto BLACK_COLOR = Qt::black;
constexpr auto WHITE_COLOR = Qt::white;
constexpr auto CHESS_BORDER = Qt::black;