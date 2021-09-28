#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /////实例化界面类/////
    //Scan页面 扫描图像页面
    scanDlg = new ScanDialog;

    //Setup页面
    setupDlg = new SetupDialog;

    //PMC400 纳米平台页面
    platDlg = new PlatformDialog;

    //PSD页面
    psdDlg = new PsdDialog;

    //stacked Widget窗体添加界面
    ui->stackedWidget->addWidget(scanDlg); //Scan页面 扫描图像页面 1
    ui->stackedWidget->addWidget(platDlg); //Setup页面 暂时为空 2
    ui->stackedWidget->addWidget(psdDlg); //PMC400 纳米平台页面 3
    ui->stackedWidget->addWidget(setupDlg); //PSD页面 4

    //stacked Widget窗体默认是 scan页面
    ui->stackedWidget->setCurrentIndex(2);

    /////TCP默认连接/////
    connectServer();
}

/////TCP通讯相关/////
//获取本地ip地址 返回值是本地IP地址
QString MainWindow::getLocalIp()
{
    QString hostName=QHostInfo::localHostName();//本地主机名
    QHostInfo  hostInfo=QHostInfo::fromName(hostName);
    QString   localIP="";

    QList<QHostAddress> addList=hostInfo.addresses();

    if (!addList.isEmpty())
    for (int i=0;i<addList.count();i++)
    {
        QHostAddress aHost=addList.at(i);
        if (QAbstractSocket::IPv4Protocol==aHost.protocol())
        {
            localIP=aHost.toString();
            break;
        }
    }

    return localIP;
}

//连接tcp
void MainWindow::connectServer()
{
    tcpClient = new QTcpSocket();

   // QString ip = getLocalIp(); //获取本地IP地址
    QString ip ="192.168.1.2";
    quint16 port = 1026;

    tcpClient->connectToHost(ip,port);
    if(!tcpClient->waitForConnected(30000))
    {
        QMessageBox::information(this, "QT网络通信", "连接服务端失败！");
        return;
    }

    //连接信号与槽
    //TCPClint有readyRead，即触发槽函数
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(receiveData()));

}

//读取数据
void MainWindow::receiveData()
{
    QDataStream in; //接收数据流in

    int iCtrCmd1; //控制字头 开始扫描0x86
    int iCtrCmd2; //控制字头 曲线图像数据4组0x01
    //int iCtrCmd3; //控制字头 PSD数据 ver hor sum 0x02
    //int iCtrCmd4; //控制字头 压电陶瓷 0x03

    //扫描数据
    QVector<short int> vHgtTraData; //in接收数组vHgtTraData
    QVector<short int> vHgtRetraData;
    QVector<short int> vDeflectTraData;//vDeflectTraData
    QVector<short int> vDeflectRetraData;//vDeflectRetraData

    //PSD数据
    int revPsdVerData;
    int revPsdHorData; //简写 revPsdData
    int revPsdSumData;

    //压电陶瓷压力数据
    int revZPiezoData;

    in.setDevice(tcpClient); //数据流in连接tcp
    in.setVersion(QDataStream::Qt_5_9); //数据流in设置版本 需客户端和服务器版本相同

    /////数据流in接收控制字头/////
    in>>iCtrCmd1; //in数据流读取控制字头0x86, iCtrCmd1 扫描开始
    in>>iCtrCmd2; //模式
    //in>>iCtrCmd3;
    //in>>iCtrCmd4;

    //数据转换
    //曲线数据double
    //图像数据unsigned char

    if(iCtrCmd1!=0x86) //扫描开始
    {
        return;
    }
    switch(iCtrCmd2) //判断扫描模式
    {
    case 0x01: //扫描数据0x01

        in>>vHgtTraData; //in输入数据流将数据存入临时数组
        in>>vHgtRetraData;
        in>>vDeflectTraData;
        in>>vDeflectRetraData;

//        //psd数据
//        in>>revPsdVerData; //vertical
//        in>>revPsdHorData; //horizonal
//        in>>revPsdSumData; //sum

        //scan页面数据转换
        scanDlg->m_vHgtTraData = scanDlg->HgtTrace->getDoubleADData(vHgtTraData);
        scanDlg->m_vHgtRetraData = scanDlg->HgtRetrace->getDoubleADData(vHgtRetraData);
        scanDlg->m_vDftTraData = scanDlg->DftTrace->getDoubleADData(vDeflectTraData);
        scanDlg->m_vDftRetraData = scanDlg->DftRetrace->getDoubleADData(vDeflectRetraData);

        //初始offset
        if(scanDlg->rowCount == 0)
        {
            scanDlg->m_vHgtTraFirstLine = scanDlg->HgtTrace->getHgtLineData(vHgtTraData);
            scanDlg->HgtTrace->m_iOffsetMin = scanDlg->HgtTrace->getAutoOffset(scanDlg->m_vHgtTraFirstLine).at(0);
            scanDlg->HgtTrace->m_iOffsetMax = scanDlg->HgtTrace->getAutoOffset(scanDlg->m_vHgtTraFirstLine).at(1);
            scanDlg->HgtTrace->m_iScaleData = scanDlg->HgtTrace->m_iOffsetMax - scanDlg->HgtTrace->m_iOffsetMin;

            scanDlg->m_vHgtRetraFirstLine = scanDlg->HgtRetrace->getHgtLineData(vHgtRetraData);
            scanDlg->HgtRetrace->m_iOffsetMin = scanDlg->HgtRetrace->getAutoOffset(scanDlg->m_vHgtRetraFirstLine).at(0);
            scanDlg->HgtRetrace->m_iOffsetMax = scanDlg->HgtRetrace->getAutoOffset(scanDlg->m_vHgtRetraFirstLine).at(1);
            scanDlg->HgtRetrace->m_iScaleData = scanDlg->HgtRetrace->m_iOffsetMax - scanDlg->HgtRetrace->m_iOffsetMin;

            scanDlg->m_vDftTraFirstLine = scanDlg->DftTrace->getHgtLineData(vDeflectTraData);
            scanDlg->DftTrace->m_iOffsetMin = scanDlg->DftTrace->getAutoOffset(scanDlg->m_vDftTraFirstLine).at(0);
            scanDlg->DftTrace->m_iOffsetMax = scanDlg->DftTrace->getAutoOffset(scanDlg->m_vDftTraFirstLine).at(1);
            scanDlg->DftTrace->m_iScaleData = scanDlg->DftTrace->m_iOffsetMax - scanDlg->DftTrace->m_iOffsetMin;

            scanDlg->m_vDftRetraFirstLine = scanDlg->DftRetrace->getHgtLineData(vDeflectRetraData);
            scanDlg->DftRetrace->m_iOffsetMin = scanDlg->DftRetrace->getAutoOffset(scanDlg->m_vDftRetraFirstLine).at(0);
            scanDlg->DftRetrace->m_iOffsetMax = scanDlg->DftRetrace->getAutoOffset(scanDlg->m_vDftRetraFirstLine).at(1);
            scanDlg->DftRetrace->m_iScaleData = scanDlg->DftRetrace->m_iOffsetMax - scanDlg->DftRetrace->m_iOffsetMin;

            //数据显示
            scanDlg->updateHgtDisplay();
            scanDlg->updateDftDisplay();
        }

        //求第i行图像数据
        scanDlg->m_vHgtTraImgLine = scanDlg->HgtTrace->processLine(vHgtTraData);
        scanDlg->m_vHgtRetraImgLine = scanDlg->HgtRetrace->processLine(vHgtRetraData);
        scanDlg->m_vDftTraImgLine = scanDlg->DftTrace->processLine(vDeflectTraData);
        scanDlg->m_vDftRetraImgLine = scanDlg->DftRetrace->processLine(vDeflectRetraData);

        scanDlg->m_hgtShowImage = scanDlg->HgtTrace->outputImg(scanDlg->m_vHgtTraImgLine,scanDlg->rowCount);
        scanDlg->m_dftShowImage = scanDlg->DftTrace->outputImg(scanDlg->m_vDftTraImgLine,scanDlg->rowCount);

        //高度模式
        if(scanDlg->HgtTrace->m_bAllUpdate == true)
        {
            //三种模式选择
            if(scanDlg->HgtTrace->m_bRaw)
            {
                scanDlg->m_hgtShowImage = scanDlg->HgtTrace->processAllLine(scanDlg->HgtTrace->m_vHgtData);
            }
            else if(scanDlg->HgtTrace->m_bFit)
            {
                scanDlg->m_hgtShowImage = scanDlg->HgtTrace->processAllLine(scanDlg->HgtTrace->m_vFitData);
            }
            else if(scanDlg->HgtTrace->m_bZAverage)
            {
                scanDlg->m_hgtShowImage = scanDlg->HgtTrace->processAllLine(scanDlg->HgtTrace->m_vZAverageData);
            }
        }

        if(scanDlg->HgtRetrace->m_bAllUpdate == true)
        {
            //三种模式选择
            if(scanDlg->HgtRetrace->m_bRaw)
            {
                scanDlg->m_hgtShowImage = scanDlg->HgtRetrace->processAllLine(scanDlg->HgtRetrace->m_vHgtData);
            }
            else if(scanDlg->HgtRetrace->m_bFit)
            {
                scanDlg->m_hgtShowImage = scanDlg->HgtRetrace->processAllLine(scanDlg->HgtRetrace->m_vFitData);
            }
            else if(scanDlg->HgtRetrace->m_bZAverage)
            {
                scanDlg->m_hgtShowImage = scanDlg->HgtRetrace->processAllLine(scanDlg->HgtRetrace->m_vZAverageData);
            }
        }

        //偏差模式
        if(scanDlg->DftTrace->m_bAllUpdate == true)
        {
            //三种模式选择
            if(scanDlg->DftTrace->m_bRaw)
            {
                scanDlg->m_dftShowImage = scanDlg->DftTrace->processAllLine(scanDlg->DftTrace->m_vHgtData);
            }
            else if(scanDlg->DftTrace->m_bFit)
            {
                scanDlg->m_dftShowImage = scanDlg->DftTrace->processAllLine(scanDlg->DftTrace->m_vFitData);
            }
            else if(scanDlg->DftTrace->m_bZAverage)
            {
                scanDlg->m_dftShowImage = scanDlg->DftTrace->processAllLine(scanDlg->DftTrace->m_vZAverageData);
            }
        }

        if(scanDlg->DftRetrace->m_bAllUpdate == true)
        {
            //三种模式选择
            if(scanDlg->DftRetrace->m_bRaw)
            {
                scanDlg->m_dftShowImage = scanDlg->DftRetrace->processAllLine(scanDlg->DftRetrace->m_vHgtData);
            }
            else if(scanDlg->DftRetrace->m_bFit)
            {
                scanDlg->m_dftShowImage = scanDlg->DftRetrace->processAllLine(scanDlg->DftRetrace->m_vFitData);
            }
            else if(scanDlg->DftRetrace->m_bZAverage)
            {
                scanDlg->m_dftShowImage = scanDlg->DftRetrace->processAllLine(scanDlg->DftRetrace->m_vZAverageData);
            }
        }

        break;

    /////PSD模式/////
    case 0x02: //PSD数据

        //psd数据
        in>>revPsdVerData; //vertical
        in>>revPsdHorData; //horizonal
        in>>revPsdSumData; //sum

        //scan页面psd数据转换/接收
        //scanDlg->setPsdData(receivePsdX,receivePsdY);
        //scanDlg->setPsdDeflectData(revPsdVerData,revPsdHorData,revPsdSumData);

        //psd页面数据接收/转换
        //navigateDlg->readData(receivePsdX,receivePsdY);
        //navigateDlg->setPsdDeflectData(revPsdVerData,revPsdHorData,revPsdSumData);

        break;

    case 0x03: //压电陶瓷

        //压电陶瓷压力值
        in>>revZPiezoData;

        //navigate页面压电陶瓷数据转换
        //navigateDlg->setZPiezoData(revZPiezoData);

        break;
    }

    //曲线绘制
    scanDlg->updateHgtCurve();
    scanDlg->updateDftCurve();

    //图像绘制
    scanDlg->updateHgtImg();
    scanDlg->updateDftImg();


    //曲线数据清空
    scanDlg->m_vHgtTraData.clear();
    scanDlg->m_vHgtRetraData.clear();
    scanDlg->m_vDftTraData.clear();
    scanDlg->m_vDftRetraData.clear();

    scanDlg->rowCount++; //行数

    //扫描结束
    if(scanDlg->rowCount == scanDlg->RESOLUTION)
    {
        //数据转换并保存
        //scanDlg->writeDataToMode(60000,256,scanDlg->m_chTraceData,scanDlg->m_chRetraceData,scanDlg->m_chDeflectionData);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

/////槽函数页面选择按钮/////
//Scan页面 扫描图像页面
void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2); //Scan页面 扫描图像页面 1+1
}

//PMC400 纳米平台页面
void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3); //PMC400纳米平台控制页面 2+1
}

//PSD页面
void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(4); //PSD控制页面 3+1
}

//setup页面 暂时为空
void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(5); //setup页面 暂时为空 4+1
}
