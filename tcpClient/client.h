#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>
#include <QMessageBox>

#include <QAbstractSocket>
class QTcpSocket;

namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *mouseEvent);
    void paintEvent(QPaintEvent *);
    void judgeWin(int x, int y);
    void winShow(int colorCenter);
    void newStart();

    ~Client();

private:
    Ui::Client *ui;
    QTcpSocket *tcpSocket;
    QString message;
    // 用来存放数据的大小信息
    quint16 blockSize;

    int xPressed;
    int yPressed;
    int numCounts;
    int counts[81][3];
    int checkBoard[9][9];//模拟棋盘，1表示黑色，-1表示白色

private slots:
    void newConnect();
    void readMessage();
    void sendMessage();
    void sendMessageStr(QString str);
    void displayError(QAbstractSocket::SocketError);

    void on_connectButton_clicked();
    void on_pushButton_clicked();


};

#endif // CLIENT_H
