#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>
#include <QMessageBox>

/////控制页面头文件/////
#include "scandialog.h"
#include "platformdialog.h"
#include "psddialog.h"
#include "setupdialog.h"

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

    /////定义分辨率/////
    int RESOLUTION;

    /////声明界面类/////
    //scan页面 扫描图像页面
    ScanDialog *scanDlg;

    //setup页面 暂时为空
    SetupDialog *setupDlg;

    //PMC400 纳米平台页面
    PlatformDialog *platDlg;

    //PSD页面
    PsdDialog *psdDlg;

/////tcp通讯相关/////
public:
    QTcpSocket *tcpClient; //上位机客户端socket

    void connectServer(); //连接tcp
    QString getLocalIp(); //获取本地ip地址 返回值是本地IP地址

//TCP通讯接收数据槽函数
public slots:
    void receiveData(); //读取数据

/////槽函数/////
private slots:

    /////页面转换槽函数/////
    void on_pushButton_clicked(); //scan页面

    void on_pushButton_2_clicked(); //navigate页面

    void on_pushButton_3_clicked(); //PMC400页面

    void on_pushButton_4_clicked(); //PSD页面

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
