#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit->setText("1026");

    //getDataFromTxt();
    getDataFromTxt2();

//    getDataFromTxt3();

    initSocket();

    //连接信号与槽
    //connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(receiveResolution()));

}

void MainWindow::getDataFromTxt()
{
//    QFile file("14950.txt");

//    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
//    {
//        qDebug()<<"Can not open the file!"<<endl;
//    }

//    QByteArray lineData;
//    float fData;

//    while(!file.atEnd())
//    {
//        lineData = file.readLine();
//        QString str(lineData);
//        fData = str.toFloat();
//        testData.append(fData);
//    }
//    qDebug()<<testData[65535];

    QFile file_1("14950.txt");
        QVector<short int> array;

        if(file_1.open(QIODevice::ReadOnly))
        {

            QTextStream stream(&file_1);
            while(!file_1.atEnd())
            {
                int buf;
                QStringList list = stream.readAll().split(" ");
                QListIterator<QString> li(list);

                while(li.hasNext())
                {
                    buf=li.next().toInt();
                    array.append(buf);
                }
            }
        }


       qDebug()<<array.count();
}

void MainWindow::getDataFromTxt2()
{
    QFile file("C:\\Users\\Administrator\\Desktop\\AFM\\AFM-tcpServer\\14950.txt");

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"Can not open the file!"<<endl;
    }

    QByteArray lineData;
    float fData;

    while(!file.atEnd())
    {
        lineData = file.readLine();
        QString str(lineData);
        fData = str.toFloat();
        testData2.append(fData);
    }

    qDebug()<<testData2.count();

}

void MainWindow::getDataFromTxt3()
{
    QFile file("‪C:/Users/dell/Desktop/testData/130.txt");

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"Can not open the file!"<<endl;
    }

    QByteArray lineData;
    short int fData;

    while(!file.atEnd())
    {
        lineData = file.readLine();
        QString str(lineData);
        fData = str.toShort();
        testData3.append(fData);
    }


}

//图像曲线数据timer
void MainWindow::timerEvent(QTimerEvent *event)
{

    testSendData.append(testData2[count]);
    testSendData2.append(testData2[count]);
    testSendData3.append(testData2[count]);
    testSendData4.append(testData2[count]);

    int iCtrCmd1 = 0x86;
    int iCtrCmd2 = 0x01;
    int iCtrCmd3 = 0x02;

//    //psd
//    int sendX = qrand()%(32767-(-32768)) + (-32768);
//    int sendY = qrand()%(32767-(-32768)) + (-32768);
//    int sendZ = qrand()%(32767-(-32768)) + (-32768);

    QByteArray buffer;
    QDataStream out(&buffer,QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_5_14);
//    out<<sendX;
//    out<<sendY;

    //tcpSocket->write(buffer);

    if(testSendData.count()==256)
    {
        out<<iCtrCmd1;
        //out<<iCtrCmd2;
        out<<iCtrCmd3;

        out<<testSendData; //trace数据
        out<<testSendData2; //reTrace数据
        out<<testSendData3;
        out<<testSendData4;

//        out<<sendX;
//        out<<sendY;
//        out<<sendZ;

        //out << (quint32)(buffer.size()- sizeof(quint32));

        tcpSocket->write(buffer);

        //qDebug()<<testSendData.at(0);
        testSendData.clear();
        testSendData2.clear();
        testSendData3.clear();
        testSendData4.clear();
    }

    count++;

    if(count==65536)
    {
        killTimer(timer);
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

//初始化tcpSocket
void MainWindow::initSocket()
{
    tcpServer = new QTcpServer();

    int port = ui->lineEdit->text().toInt();
    if(!tcpServer->listen(QHostAddress::Any,port))
    {
        QMessageBox::information(this,"QT网络通信","服务器监听失败");
    }
    else
    {
        QMessageBox::information(this,"QT网络通信","服务器监听成功");
    }

    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(onNewConnection()));


}

void MainWindow::onNewConnection()
{
    //获取客户端连接
    tcpSocket = tcpServer->nextPendingConnection();

    if(!tcpSocket)
    {
        QMessageBox::information(this,"Qt网络通信","未正确获取客户端连接");
    }
    else
    {
        QMessageBox::information(this,"Qt网络通信","成功获取客户端连接");

        //有数据
        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(serverReadData()));

        //没有数据
        connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(serverDisConnection()));
    }
}

void MainWindow::serverReadData()
{
//    QByteArray buffer;
//    buffer = tcpSocket->read(1024);

//    if(buffer.isEmpty())
//    {
//        QMessageBox::information(this,"Qt网络通信","未正确接收数据");
//    }
//    else
//    {
//        QString showMsg = buffer;
//        ui->textEdit_2->append(showMsg);
//    }

    QDataStream in; //接收数据流in

    int revResolution;

    in.setDevice(tcpSocket); //数据流in连接tcp
    in.setVersion(QDataStream::Qt_5_9); //数据流in设置版本 需客户端和服务器版本相同

    in>>revResolution;

    QString str = QString::number(revResolution);

    ui->textEdit_2->setText(str);
}

void MainWindow::receiveResolution()
{
    QDataStream in; //接收数据流in

    int revResolution;

    in.setDevice(tcpSocket); //数据流in连接tcp
    in.setVersion(QDataStream::Qt_5_14); //数据流in设置版本 需客户端和服务器版本相同

    in>>revResolution;

    QString str = QString::number(revResolution);

    ui->textEdit_2->setText(str);

}

void MainWindow::serverDisConnection()
{
    QMessageBox::information(this, "QT网络通信", "与客户端的连接断开");
}

void MainWindow::on_pushButton_clicked()
{
    //initSocket();
}

void MainWindow::on_pushButton_2_clicked()
{
    //sendData();
    timer = startTimer(5);

}

void MainWindow::on_pushButton_3_clicked()
{

}
