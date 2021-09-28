#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTcpServer>
#include <QTcpSocket>

#include <QMessageBox>

#include <QVector>
#include <QFile>
#include <QTimerEvent>

#include <QMetaType>
#include <QDataStream>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public:
    //txt获取整组 测试数据
    void getDataFromTxt();
    void getDataFromTxt2();
    void getDataFromTxt3();

    //测试数据数组
    QVector<double> testData;
    QVector<double> testData2;
    QVector<short int> testData3;

    void timerEvent(QTimerEvent *event);

    int timer;
    int count = 0;

    QVector<short int> testSendData;
    QVector<short int> testSendData2;
    QVector<short int> testSendData3;
    QVector<short int> testSendData4;

public:
    void initSocket();

public:
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void onNewConnection();
    void serverReadData();
    void serverDisConnection();


    void on_pushButton_3_clicked();

    void receiveResolution();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
