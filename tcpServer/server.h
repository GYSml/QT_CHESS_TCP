#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include<QtNetwork>

#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>
#include <QMessageBox>

class QTcpServer;

namespace Ui {
    class Server;
}

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void judgeWin(int x,int y);
    void winShow(int colorCenter);
    void newStart();


private:
    Ui::Server *ui;
    QTcpServer *tcpServer;
    QTcpSocket *clientConnection;

    int xPressed;
    int yPressed;
    int numCounts;
    int counts[81][3];
    int checkBoard[9][9];//模拟棋盘，1表示黑色，-1表示白色

private slots:
    void sendMessage();
    void sendMessageStr(QString str);
    void readMessageFromClient();
    void new_connect();
    void on_pushButton_clicked();
};

#endif // SERVER_H
