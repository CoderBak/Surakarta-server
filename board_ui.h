// In this file, we draw the board ui for server.
#ifndef BOARD_UI_H
#define BOARD_UI_H

#include <QWidget>
#include <QHostAddress>
#include <QPainter>
#include "common.h"
#include "server.h"

constexpr int sub_WIDTH = 378;
constexpr int sub_HEIGHT = 400;
constexpr int sub_BOARD_HEIGHT = 300;
constexpr int sub_DELTA_Y = 39;
constexpr int sub_DELTA_X = 28;
constexpr int sub_cellSize = sub_BOARD_HEIGHT / BOARD_SIZE;
constexpr int sub_PEN_WIDTH = 2;
constexpr int sub_chessRadius = static_cast<int>(0.35 * sub_cellSize);

class board_ui : public QWidget {
Q_OBJECT

public:
    explicit board_ui(QWidget *parent) {
        for (int i = 0; i < BOARD_SIZE; i += 1) {
            for (int j = 0; j < BOARD_SIZE; j += 1) {
                _board[i][j] = NONE;
            }
        }
    }

    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.translate(0, -181);
        painter.setPen(QPen(DEFAULT_COLOR, sub_PEN_WIDTH));
        for (int i = 0; i < BOARD_SIZE; i += 1) {
            for (int j = 0; j < BOARD_SIZE; j += 1) {
                painter.save();
                painter.drawRect(sub_DELTA_X + i * sub_cellSize, DELTA_Y + j * sub_cellSize,
                                 sub_cellSize, sub_cellSize);
                painter.restore();
            }
        }
        drawChess();
    }

    void drawChess() {
        auto translateIdx = [=](auto x, auto y) {
            return std::make_pair(sub_DELTA_X + x * sub_cellSize + sub_cellSize / 2,
                                  sub_DELTA_Y + y * sub_cellSize + sub_cellSize / 2);
        };
        QPainter painter(this);
        for (int i = 0; i < BOARD_SIZE; i += 1) {
            for (int j = 0; j < BOARD_SIZE; j += 1) {
                if (_board[i][j] != NONE) {
                    const auto currentColor = (_board[i][j] == BLACK) ? Qt::black : Qt::white;
                    painter.setPen(QPen(CHESS_BORDER, PEN_WIDTH));
                    painter.setBrush(QBrush(currentColor, Qt::SolidPattern));
                    const auto [centerX, centerY] = translateIdx(j, i);
                    const QRect rect(centerX - sub_chessRadius, centerY - sub_chessRadius,
                                     2 * sub_chessRadius, 2 * sub_chessRadius);
                    painter.drawEllipse(rect);
                }
            }
        }
    }

    void connectWith(Server *server) const {
        connect(server, &Server::boardUpdated, this, &board_ui::updateBoard);
    }

private:
    ChessColor _board[BOARD_SIZE][BOARD_SIZE]{};

private slots:

    void updateBoard(const QString &boardInfo) {
        QStringList rows = boardInfo.split('|');
        for (int row = 0; row < BOARD_SIZE; row += 1) {
            QString rowString = rows.value(row).trimmed();
            rowString.remove(' ');
            QTextStream stream(&rowString);
            for (int col = 0; col < BOARD_SIZE; col += 1) {
                QChar pieceChar;
                stream >> pieceChar;
                switch (pieceChar.toLatin1()) {
                    case 'B':
                        _board[row][col] = BLACK;
                        break;
                    case 'W':
                        _board[row][col] = WHITE;
                        break;
                    case '.':
                        _board[row][col] = NONE;
                        break;
                    default:
                        continue;
                }
            }
        }
        repaint();
    }
};

#endif // BOARD_UI_H
