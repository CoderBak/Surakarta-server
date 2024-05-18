#ifndef BOARDUI_H
#define BOARDUI_H

#include <QWidget>
#include <QHostAddress>
#include "common.h"
// using posType = std::pair<int, int>;
// constexpr unsigned int BOARD_SIZE = 6;
// constexpr unsigned int maxNoCapture = 40;
constexpr int WIDTH0 = 1180/5*3;
constexpr int HEIGHT0 = 820/5*3;
constexpr int BOARD_HEIGHT0 = 720 / 2/5*3;
constexpr int DELTA_Y0 = HEIGHT / 2 - BOARD_HEIGHT / 2 - 10/5*3;
constexpr int DELTA_X0 = 400/5*3;
constexpr int cellSize0 = BOARD_HEIGHT0 / BOARD_SIZE;
constexpr int PEN_WIDTH0 = 2/5*3;
// constexpr auto PATH_COLOR = QColor(173, 216, 230);
// constexpr auto DEFAULT_COLOR = Qt::black;
// constexpr auto BACK_COLOR = QColor(255, 255, 240);
constexpr int chessRadius0 = static_cast<int>(0.35 * cellSize0);
// constexpr auto BLACK_COLOR = Qt::black;
// constexpr auto WHITE_COLOR = Qt::white;
// constexpr auto CHESS_BORDER = Qt::black;
// constexpr auto SELECTED_COLOR = Qt::red;
// constexpr Qt::GlobalColor EATABLE_COLOR = Qt::green;
// constexpr auto MOVABLE_COLOR = Qt::green;
// constexpr auto ANIMATION_PER_TIME = 500;
// constexpr auto ANIMATION_STEP = 100;
// // constexpr auto EATABLE_PATH_COLOR = Qt::yellow;
// constexpr int selectedSize = 5;
// constexpr int emphasizeSize = 3;
// constexpr Qt::PenStyle selectedLine = Qt::SolidLine;
// constexpr Qt::PenStyle emphasizeLine = Qt::DotLine;
// constexpr int maxSecond = 1000;
// const QHostAddress serverIP = QHostAddress("20.212.82.172");
// constexpr int PORT = 1233;
// // const QString STYLE = "background-color: white;";
// enum ChessColor {
//     WHITE,
//     BLACK,
//     NONE
// };

class boardUi:public QWidget{
    Q_OBJECT

public:
    explicit boardUi(QWidget *parent);

    ~boardUi();

    std::pair<int, int> translateIdx1(const unsigned int x, const unsigned int y) {
        return std::make_pair(DELTA_X0 + x * cellSize0 + cellSize0 / 2, DELTA_Y0 + y * cellSize0 + cellSize0 / 2);
    }


    void paintEvent(QPaintEvent *) override;

    //void mousePressEvent(QMouseEvent *event) override;

    //void mouseMoveEvent(QMouseEvent *event) override;

    void processBoardInfo(const QByteArray &boardInfo);

    void drawChess();

    //bool eventFilter(QObject *obj, QEvent *event) override;

    bool rotated;

    void animationStep();

    void highlightPath(QPainter &painter);


private:
    //Settings settings;
    ChessColor chessColor0[BOARD_SIZE][BOARD_SIZE];
    ChessColor current_player;
    int selectedPieceRow = -1;
    int selectedPieceCol = -1;
    std::vector<posType> movable;
    std::vector<std::pair<posType, std::vector<posType>>> eatable;
    QTimer *animationTimer;
    int currentPathIndex;
    int lastRow, lastCol;
    int fromRow, fromCol;
    const std::vector<posType> *currentPath;
    bool handledByAI = false;
    bool inAnimation = false;
    bool shouldCheckAnimation = false;
    int extraX = -1, extraY = -1;
    int currentStep;
    int gameCount = 0;
};


#endif // BOARDUI_H
