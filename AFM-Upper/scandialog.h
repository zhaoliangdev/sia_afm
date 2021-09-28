#ifndef SCANDIALOG_H
#define SCANDIALOG_H

#include <QDialog>

#include <QImage>
#include <QPixmap>
#include <QVector>
#include <QFile>
#include <QTimer>
#include <QTimerEvent>
#include <QTextStream>
#include <QIODevice>
#include <QDebug>
#include "math.h"
#include <QSpinBox>
#include <QPainter>
#include <QPaintEvent>

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>
#include <QMessageBox>

#include <QMouseEvent>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_draw.h>
#include <qwt_series_data.h>

#include "transdata.h"
#include "psdcontrol.h"

namespace Ui {
class ScanDialog;
}

class ScanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScanDialog(QWidget *parent = nullptr);
    ~ScanDialog();

//声明数据转换类
public:
    //四种数据类型
    TRANSDATA *HgtTrace;
    TRANSDATA *HgtRetrace;
    TRANSDATA *DftTrace;
    TRANSDATA *DftRetrace;

    //四幅图像 //两个展示窗口
    //高度模式展示图像
    QImage m_hgtShowImage;

    //偏差模式展示图像
    QImage m_dftShowImage;

//槽函数
public slots:

    //高度模式
    void setHgtDataType();

    //偏差模式
    void setDftDataType();

public:

    //绘制四幅mini图像
    void drawMiniImage();

public:

    //四条曲线数据
    //高度模式
    //正向轨迹Trace
    //反向轨迹Retrace
    QVector<double> m_vHgtTraData;
    QVector<double> m_vHgtRetraData;

    QwtPlotCurve *m_HgtTraCurve;
    QwtPlotCurve *m_HgtRetraCurve;

    void drawHgtTraCurve();
    void drawHgtRetraCurve();
    void updateHgtCurve();

    //偏差模式
    QVector<double> m_vDftTraData;
    QVector<double> m_vDftRetraData;

    QwtPlotCurve *m_DftTraCurve;
    QwtPlotCurve *m_DftRetraCurve;

    void drawDftTraCurve();
    void drawDftRetraCurve();
    void updateDftCurve();

    //图像更新
    //mini图像
    void updateMiniHgtTraImg();
    void updateMiniHgtRetraImg();
    void updateMiniDftTraImg();
    void updateMiniDftRetraImg();

    //大图像
    void updateHgtImg();
    void updateDftImg();

    //数据显示更新
    void updateHgtDisplay();
    void updateDftDisplay();

    //第一行数据
    QVector<double> m_vHgtTraFirstLine;
    QVector<double> m_vHgtRetraFirstLine;
    QVector<double> m_vDftTraFirstLine;
    QVector<double> m_vDftRetraFirstLine;

    //行数据
    QVector<int> m_vHgtTraImgLine;
    QVector<int> m_vHgtRetraImgLine;
    QVector<int> m_vDftTraImgLine;
    QVector<int> m_vDftRetraImgLine;


public:
    void setX(int dataLength); //设置曲线的X轴坐标
    QVector<double> xPos; //x轴数据 = RESOLUTION
    int RESOLUTION; //定义分辨率 128 256 512

    int rowCount = 0;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::ScanDialog *ui;
};

#endif // SCANDIALOG_H
