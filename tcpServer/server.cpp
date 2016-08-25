#include "server.h"
#include "ui_server.h"
#include <QtNetwork>

Server::Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer(this);

    if (!tcpServer->listen(QHostAddress::Any, 6666)) {
        qDebug() << tcpServer->errorString();
        close();
    }
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(new_connect()));

    this->xPressed = 0;
    this->yPressed = 0;
    this->numCounts = 0;
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            checkBoard[i][j] = 0;
        }
    }


}

void Server::new_connect()
{
    clientConnection = tcpServer->nextPendingConnection(); //获取已经建立的连接套接字
    connect(clientConnection,SIGNAL(readyRead()),this,SLOT(readMessageFromClient()));
}

void Server::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::black);
    painter.setPen(pen);
    //画线
    for(int i=60;i<=600;)
    {
        QPoint pointOne(0,i);
        QPoint pointTwo(600,i);
        painter.drawLine(pointOne,pointTwo);
        i = i + 60;
    }
    for(int i=60;i<=600;)
    {
        QPoint pointOne(i,0);
        QPoint pointTwo(i,600);
        painter.drawLine(pointOne,pointTwo);
        i = i + 60;
    }
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    for(int i=0;i<this->numCounts;i++)
    {
        if(i%2 == 0)
        {
            brush.setColor(Qt::black);
        }
        else
        {
            brush.setColor(Qt::white);
        }
        painter.setBrush(brush);
        painter.drawEllipse(this->counts[i][0]-20,this->counts[i][1]-20,40,40);
    }
}

void Server::mousePressEvent(QMouseEvent *mouseEvent)
{
    int x = int(float(mouseEvent->x())/60+0.5)*60;
    int y = int(float(mouseEvent->y())/60+0.5)*60;

    if(this->numCounts%2 == 0){

    }else{
        if(x>0&&x<600&&y>0&&y<600&&this->checkBoard[y/60-1][x/60-1]==0)
        {
            this->numCounts += 1;//棋子数目加一
            if(this->numCounts%2 != 0)//棋局矩阵构造
            {
                this->checkBoard[y/60-1][x/60-1] = 1; //1表示黑色
            }
            else
            {
                this->checkBoard[y/60-1][x/60-1] = -1;//-1表示白色
            }
            this->counts[this->numCounts-1][0] = x;
            this->counts[this->numCounts-1][1] = y;
            this->counts[this->numCounts-1][2] = this->numCounts%2;

            judgeWin(x,y);
            this->update();   //用于绘图
            sendMessageStr(QString("location:%1:%2:%3").arg(x).arg(y).arg(numCounts%2));
        }
    }
}

void Server::judgeWin(int x, int y)
{
    int row = y/60-1; //显示第几行
    int col = x/60-1; //显示第几列
    int colorCenter = this->checkBoard[row][col];//中间点的颜色
    ///////////列
    if(row>=4&&this->checkBoard[row-4][col]==colorCenter&&this->checkBoard[row-3][col]==colorCenter&&this->checkBoard[row-2][col]==colorCenter&&this->checkBoard[row-1][col]==colorCenter)
    {//该点上面
        winShow(colorCenter);
    }
    else if(row>=3&&row<=7&&this->checkBoard[row+1][col]==colorCenter&&this->checkBoard[row-3][col]==colorCenter&&this->checkBoard[row-2][col]==colorCenter&&this->checkBoard[row-1][col]==colorCenter)
    {
        winShow(colorCenter);
    }
    else if(row>=2&&row<=6&&this->checkBoard[row+2][col]==colorCenter&&this->checkBoard[row+1][col]==colorCenter&&this->checkBoard[row-2][col]==colorCenter&&this->checkBoard[row-1][col]==colorCenter)
    {
        winShow(colorCenter);
    }
    else if(row>=1&&row<=5&&this->checkBoard[row+3][col]==colorCenter&&this->checkBoard[row+2][col]==colorCenter&&this->checkBoard[row+1][col]==colorCenter&&this->checkBoard[row-1][col]==colorCenter)
    {
        winShow(colorCenter);
    }
    else if(row<=4&&this->checkBoard[row+4][col]==colorCenter&&this->checkBoard[row+3][col]==colorCenter&&this->checkBoard[row+2][col]==colorCenter&&this->checkBoard[row+1][col]==colorCenter)
    {//该点下面
        winShow(colorCenter);
    }
    //////////行
    else if(col>=4&&this->checkBoard[row][col-4]==colorCenter&&this->checkBoard[row][col-3]==colorCenter&&this->checkBoard[row][col-2]==colorCenter&&this->checkBoard[row][col-1]==colorCenter)
    {//该点左边
        winShow(colorCenter);
    }
    else if(col>=3&&col<=7&&this->checkBoard[row][col+1]==colorCenter&&this->checkBoard[row][col-3]==colorCenter&&this->checkBoard[row][col-2]==colorCenter&&this->checkBoard[row][col-1]==colorCenter)
    {
        winShow(colorCenter);
    }
    else if(col>=2&&col<=6&&this->checkBoard[row][col+2]==colorCenter&&this->checkBoard[row][col+1]==colorCenter&&this->checkBoard[row][col-2]==colorCenter&&this->checkBoard[row][col-1]==colorCenter)
    {
        winShow(colorCenter);
    }
    else if(col>=1&&col<=5&&this->checkBoard[row][col+3]==colorCenter&&this->checkBoard[row][col+2]==colorCenter&&this->checkBoard[row][col+1]==colorCenter&&this->checkBoard[row][col-1]==colorCenter)
    {
        winShow(colorCenter);
    }
    else if(col<=4&&this->checkBoard[row][col+4]==colorCenter&&this->checkBoard[row][col+3]==colorCenter&&this->checkBoard[row][col+2]==colorCenter&&this->checkBoard[row][col+1]==colorCenter)
    {//该点右边
        winShow(colorCenter);
    }
    //左上到右下
    else if(row>=4&&col>=4&&this->checkBoard[row-4][col-4]==colorCenter&&this->checkBoard[row-3][col-3]==colorCenter&&this->checkBoard[row-2][col-2]==colorCenter&&this->checkBoard[row-1][col-1]==colorCenter)
    {//该点左上
        winShow(colorCenter);
    }
    else if(row>=3&&col>=3&&row<=7&&col<=7&&this->checkBoard[row+1][col+1]==colorCenter&&this->checkBoard[row-3][col-3]==colorCenter&&this->checkBoard[row-2][col-2]==colorCenter&&this->checkBoard[row-1][col-1]==colorCenter)
    {
        winShow(colorCenter);
    }
    else if(row>=2&&col>=2&&row<=6&&col<=6&&this->checkBoard[row+2][col+2]==colorCenter&&this->checkBoard[row+1][col+1]==colorCenter&&this->checkBoard[row-2][col-2]==colorCenter&&this->checkBoard[row-1][col-1]==colorCenter)
    {
        winShow(colorCenter);
    }
    else if(row>=1&&col>=1&&row<=5&&col<=5&&this->checkBoard[row+3][col+3]==colorCenter&&this->checkBoard[row+2][col+2]==colorCenter&&this->checkBoard[row+1][col+1]==colorCenter&&this->checkBoard[row-1][col-1]==colorCenter)
    {
        winShow(colorCenter);
    }
    else if(row<=4&&col<=4&&this->checkBoard[row+4][col+4]==colorCenter&&this->checkBoard[row+3][col+3]==colorCenter&&this->checkBoard[row+2][col+2]==colorCenter&&this->checkBoard[row+1][col+1]==colorCenter)
    {//该点右下
        winShow(colorCenter);
    }
    //右上到左下
    else if(row>=4&&col<=4&&this->checkBoard[row-4][col+4]==colorCenter&&this->checkBoard[row-3][col+3]==colorCenter&&this->checkBoard[row-2][col+2]==colorCenter&&this->checkBoard[row-1][col+1]==colorCenter)
    {//该点左下
        winShow(colorCenter);
    }
    else if(row>=3&&col<=5&&row<=7&&col>=1&&this->checkBoard[row+1][col-1]==colorCenter&&this->checkBoard[row-3][col+3]==colorCenter&&this->checkBoard[row-2][col+2]==colorCenter&&this->checkBoard[row-1][col+1]==colorCenter)
    {
        winShow(colorCenter);
    }
    else if(row>=2&&col<=6&&row<=6&&col>=2&&this->checkBoard[row+2][col-2]==colorCenter&&this->checkBoard[row+1][col-1]==colorCenter&&this->checkBoard[row-2][col+2]==colorCenter&&this->checkBoard[row-1][col+1]==colorCenter)
    {
        winShow(colorCenter);
    }
    else if(row>=1&&col<=7&&row<=5&&col>=3&&this->checkBoard[row+3][col-3]==colorCenter&&this->checkBoard[row+2][col-2]==colorCenter&&this->checkBoard[row+1][col-1]==colorCenter&&this->checkBoard[row-1][col+1]==colorCenter)
    {
        winShow(colorCenter);
    }
    else if(row<=4&&col>=4&&this->checkBoard[row+4][col-4]==colorCenter&&this->checkBoard[row+3][col-3]==colorCenter&&this->checkBoard[row+2][col-2]==colorCenter&&this->checkBoard[row+1][col-1]==colorCenter)
    {//该点右上
        winShow(colorCenter);
    }
    else if(numCounts==81)
    {
        newStart();
    }
}

void Server::winShow(int colorCenter)
{
    QString infoShow;
    if(colorCenter == 1)
    {
        infoShow = "Black Win!";
        sendMessageStr("black");
    }
    else if(colorCenter == -1)
    {
        infoShow = "White Win!";
        sendMessageStr("white");
    }
    if(QMessageBox::information(NULL,"Game Over",infoShow,"Again","Exit")==1)
    {
        close();
    }
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            this->checkBoard[i][j]=0;
        }
    }
    this->numCounts = 0;
}

void Server::newStart()
{
    if(QMessageBox::information(NULL,"Game Over","newStart","Again","Exit")==1)
    {
        close();
    }
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            this->checkBoard[i][j]=0;
        }
    }
    this->numCounts = 0;
}

Server::~Server()
{
    delete ui;
}

void Server::sendMessage()
{
//    QString str = ui->lineEdit->text();
//    clientConnection->write(str.toLatin1());
//    ui->label->setText("you will be OK");
//    qDebug()<<"server send message to client:"+str;
//    ui->lineEdit->clear();
}

void Server::sendMessageStr(QString str)
{
    clientConnection->write(str.toLatin1());
}

void Server::readMessageFromClient()
{
    QByteArray qba = clientConnection->readAll();
    QString ss = QVariant(qba).toString();
    ui->label_3->setText(ss);
    qDebug()<<"server recived message from client:"+ss;
    if(ss == "black"){
        winShow(1);
    }else if(ss == "white"){
        winShow(-1);
    }
    else{
        QString string_x = ss.split(":")[1];
        QString string_y = ss.split(":")[2];
        QString string_m = ss.split(":")[3];
        int x = string_x.toInt();
        int y = string_y.toInt();
        int m = string_m.toInt();
        this->numCounts += 1;
        this->counts[this->numCounts-1][0] = x;
        this->counts[this->numCounts-1][1] = y;
        this->counts[this->numCounts-1][2] = m;
        this->update();
    }
}

void Server::on_pushButton_clicked()
{
    sendMessage();
}
