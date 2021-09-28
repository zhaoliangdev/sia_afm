#ifndef PSDDIALOG_H
#define PSDDIALOG_H

//PSD数据转换类
//PZT数据转换类
#include "psdcontrol.h"

#include <QDialog>

namespace Ui {
class PsdDialog;
}

class PsdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PsdDialog(QWidget *parent = nullptr);
    ~PsdDialog();

public:

    //数据转化类
    PSDControl *psdControl;

    /////PSD数据相关/////
    //PSD电压数据显示
    void showPsdSignal(int revVerPsd,int revHorPsd,int revSumPsd);

    //PSD光标坐标
    int m_xPos;
    int m_yPos;

    //绘制光标
    void paintPsdPoint(int xPos,int yPos);

    /////PZT数据相关/////
    void showPztSignal(int revPzt);

private:
    Ui::PsdDialog *ui;
};

#endif // PSDDIALOG_H
