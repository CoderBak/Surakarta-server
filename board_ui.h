#ifndef BOARD_UI_H
#define BOARD_UI_H

#include <QWidget>
#include <QHostAddress>
#include "common.h"
#include "server.h"

constexpr int _WIDTH = 378;
constexpr int _HEIGHT = 640;
constexpr int _BOARD_HEIGHT = 300;
constexpr int _DELTA_Y = 220;
constexpr int _DELTA_X = 28;
constexpr int _cellSize = _BOARD_HEIGHT / BOARD_SIZE;
constexpr int _PEN_WIDTH = 2;
constexpr int _chessRadius = static_cast<int>(0.35 * _cellSize);

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
        painter.setPen(QPen(DEFAULT_COLOR, _PEN_WIDTH));
        for (int i = 0; i < BOARD_SIZE; i += 1) {
            for (int j = 0; j < BOARD_SIZE; j += 1) {
                painter.save();
                painter.drawRect(_DELTA_X + i * _cellSize, DELTA_Y + j * _cellSize, _cellSize, _cellSize);
                painter.restore();
            }
        }
        drawChess();
    }

    void drawChess() {
        auto translateIdx = [=](auto x, auto y) {
            return std::make_pair(_DELTA_X + x * _cellSize + _cellSize / 2, _DELTA_Y + y * _cellSize + _cellSize / 2);
        };
        QPainter painter(this);
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (_board[i][j] != NONE) {
                    const auto currentColor = (_board[i][j] == BLACK) ? Qt::black : Qt::white;
                    painter.setPen(QPen(CHESS_BORDER, PEN_WIDTH));
                    painter.setBrush(QBrush(currentColor, Qt::SolidPattern));
                    const auto [centerX, centerY] = translateIdx(j, i);
                    const QRect rect(centerX - _chessRadius, centerY - _chessRadius, 2 * _chessRadius,
                                     2 * _chessRadius);
                    painter.drawEllipse(rect);
                }
            }
        }
    }

    void connectWith(Server *server) const {
        connect(server, &Server::boardUpdated, this, &board_ui::updateBoard);
    }

private:
    ChessColor _board[BOARD_SIZE][BOARD_SIZE];

private slots:

    void updateBoard(const QString &boardInfo) {
        QStringList rows = boardInfo.split('\n');
        for (int row = 0; row < BOARD_SIZE; row += 1) {
            QString rowString = rows.value(row).trimmed();
            rowString.remove(' ');
            QTextStream stream(&rowString);
            for (int col = 0; col < BOARD_SIZE; col += 1) {
                QChar pieceChar;
                stream >> pieceChar;
                // qDebug() << pieceChar;
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
