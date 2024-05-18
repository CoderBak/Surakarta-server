#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        serverBoard(new boardUi(this))
    {
    ui->setupUi(this);
   auto *layout = new QHBoxLayout;
    layout->addWidget(serverBoard);
   auto *centralWidget = new QWidget;
   centralWidget->setLayout(layout);
   //centralWidget->setFixedSize(WIDTH0, HEIGHT0);
   setCentralWidget(centralWidget);


    server = new NetworkServer(this); // 创建一个服务器对象，其父对象为当前窗口

    ui->ShowClient1->setReadOnly(true);
    ui->ShowClient2->setReadOnly(true);

    connect(ui->PortButton, &QPushButton::clicked, this, &MainWindow::listen_port); // 开启服务器监听
    connect(ui->restart, &QPushButton::clicked, this, &MainWindow::restart_server); // 重启服务器
    connect(server, &NetworkServer::receive, this, &MainWindow::receive_from_client);
    // receive 是服务端收到消息后发出的信号，receive_from_client 是处理这个信号的槽函数
}


void MainWindow::listen_port() {
    this->port = ui->PortEdit->text().toInt();
    server->listen(QHostAddress::Any, this->port);

    ui->PortButton->setEnabled(false);
    ui->PortEdit->setReadOnly(true);
    ui->PortEdit->setText("Listening...");
}

void MainWindow::remove_client(QTcpSocket *client) {
    if (client == client1) {
        client1 = nullptr;
        ui->ShowClient1->setText("");
    } else if (client == client2) {
        client2 = nullptr;
        ui->ShowClient2->setText("");
    }
    clients.remove(client);
}

void MainWindow::receive_from_client(QTcpSocket *client, NetworkData data) {
    // client 是发送消息的客户端，data 是消息内容
    // 以下代码首先处理客户端的连接和断开，然后处理从客户端收到的消息，显示在界面上并转发给另一个客户端
    // 你们在游戏里要做类似的事情，只不过处理不仅仅是显示，而是对应的游戏逻辑
    if (data.op == OPCODE::LEAVE_OP) {
        remove_client(client);
        return;
    }
    if (!clients.contains(client)) {
        if (clients.size() >= max_clients) {
            QMessageBox::warning(this, "Warning", "The server is full!");
            return;
        }
        clients.insert(client);
        if (!client1)
            client1 = client;
        else if (!client2)
            client2 = client;
    }
    if (client == client1) {
        this->ui->ShowClient1->setText(data.data2);
        if (client2 && data.op == OPCODE::CHAT_OP)
            send_to_another_client(client2, data);
    } else if (client == client2) {
        this->ui->ShowClient2->setText(data.data2);
        if (client1 && data.op == OPCODE::CHAT_OP)
            send_to_another_client(client1, data);
    } else
        QMessageBox::warning(this, "Warning", "Unknown client!");
}

void MainWindow::send_to_another_client(QTcpSocket *another, NetworkData data) {
    // 发送消息给一个客户端，这个程序里实际上做的事情是转发消息
    this->server->send(another, data);
}

void MainWindow::restart_server() {
    // 如果你要重启服务器，就像其他 QObject 一样，你需要 disconnect 再 connect 回来一些信号槽
    server->close();
    clients.clear();
    client1 = nullptr;
    client2 = nullptr;
    disconnect(server, &NetworkServer::receive, this, &MainWindow::receive_from_client);
    delete server;
    server = new NetworkServer(this);
    ui->PortButton->setEnabled(true);
    ui->PortEdit->setReadOnly(false);
    ui->PortEdit->setText(QString::number(this->port));
    ui->ShowClient1->setText("");
    ui->ShowClient2->setText("");
    connect(server, &NetworkServer::receive, this, &MainWindow::receive_from_client);
}

MainWindow::~MainWindow() {
    delete ui;
}

boardUi::boardUi(QWidget *parent) : QWidget(parent) {
    this->installEventFilter(this);
    setMouseTracking(true);
    for (unsigned int i = 0; i < BOARD_SIZE; i += 1) {
        for (unsigned int j = 0; j < BOARD_SIZE; j += 1) {
            chessColor0[i][j] = NONE;
        }
    }
    // connect(&settings, &Settings::settingsApplied, this, &QtBoard::receiveDataFromUser);
}

boardUi::~boardUi()
{

}

void boardUi::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.fillRect(rect(), BACK_COLOR);

    // no need for arc
    // // Draw k = size / 2 layers of arc, centered at centerX, centerY.
    painter.setPen(QPen(DEFAULT_COLOR, PEN_WIDTH0));
    for (int i = 0; i < BOARD_SIZE; i += 1) {
        for (int j = 0; j < BOARD_SIZE; j += 1) {
            painter.save();
            painter.drawRect(DELTA_X0 + i * cellSize0, DELTA_Y0 + j * cellSize0, cellSize0, cellSize0);
            painter.restore();
        }
    }
    // drawArcs(painter, DELTA_X, DELTA_Y, 0, BOARD_SIZE / 2);
    // drawArcs(painter, DELTA_X + BOARD_HEIGHT, DELTA_Y, 270 * 16, BOARD_SIZE / 2);
    // drawArcs(painter, DELTA_X, DELTA_Y + BOARD_HEIGHT, 90 * 16, BOARD_SIZE / 2);
    // drawArcs(painter, DELTA_X + BOARD_HEIGHT, DELTA_Y + BOARD_HEIGHT, 180 * 16, BOARD_SIZE / 2);

    auto emphasize = [&](const int col, const int row, auto color, auto line, auto size) {
        const auto [centerX, centerY] = translateIdx1(col, row);
        const QRect rect(centerX - chessRadius0, centerY - chessRadius0, 2 * chessRadius0, 2 * chessRadius0);
        painter.setPen(QPen(color, size, line));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(rect);
    };

    drawChess();
}

void boardUi::drawChess()
{
    QPainter painter(this);
    for (unsigned int i = 0; i < BOARD_SIZE; i++) {
        for (unsigned int j = 0; j < BOARD_SIZE; j++) {
            if (chessColor0[i][j] != NONE) {
                const auto currentColor = (chessColor0[i][j] == BLACK) ? Qt::black : Qt::white;
                painter.setPen(QPen(CHESS_BORDER, PEN_WIDTH0));
                painter.setBrush(QBrush(currentColor, Qt::SolidPattern));
                const auto [centerX, centerY] = translateIdx1(j, i);
                const QRect rect(centerX - chessRadius0, centerY - chessRadius0, 2 * chessRadius0, 2 * chessRadius0);
                painter.drawEllipse(rect);
            }
        }
    }
    if (extraX != -1 && extraY != -1) {
        const auto currentColor = (current_player == BLACK) ? Qt::black : Qt::white;
        painter.setPen(QPen(CHESS_BORDER, PEN_WIDTH0));
        painter.setBrush(QBrush(currentColor, Qt::SolidPattern));
        const QRect rect(extraX - chessRadius0, extraY - chessRadius0, 2 * chessRadius0, 2 * chessRadius0);
        painter.drawEllipse(rect);
    }
}
