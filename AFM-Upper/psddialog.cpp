#include "psddialog.h"
#include "ui_psddialog.h"

PsdDialog::PsdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsdDialog)
{
    ui->setupUi(this);

    //实例化数据转换类
    psdControl = new PSDControl;
}

/////TCP相关/////
//tcp数据触发函数

/////PSD数据相关/////
//PSD电压数据显示
void PsdDialog::showPsdSignal(int revVerPsd, int revHorPsd, int revSumPsd)
{
    double verPsdData; //水平电压
    double horPsdData; //垂直电压
    double sumPsdData; //总电压

    verPsdData = psdControl->transVerPsdData(revVerPsd);
    horPsdData = psdControl->transHorPsdData(revHorPsd);
    sumPsdData = psdControl->transSumSignalData(revSumPsd);

    //实时显示更新
    ui->label_26->setNum(verPsdData);
    ui->label_27->setNum(horPsdData);
    ui->label_28->setNum(sumPsdData);

    //PSD bar实时变化
    int psdBarLength;
    psdBarLength = int((sumPsdData/10.0)*ui->frame_10->width());

    ui->label_32->setNum(sumPsdData);
    ui->frame_10->resize(psdBarLength,ui->frame_10->height());
}

//绘制PSD模拟光标

/////PZT数据相关/////
//PZT电压显示
void PsdDialog::showPztSignal(int revPzt)
{
    double pztSignal; //PZT电压

    pztSignal = psdControl->transPztData(revPzt);

    //实时显示更新
    //PZT bar实时更新
    int pztBarLength;
    pztBarLength = int((pztSignal/10.0)*ui->frame_14->height());

    ui->label_34->setNum(pztSignal);
    ui->frame_14->resize(ui->frame_14->width(),pztBarLength);
}

PsdDialog::~PsdDialog()
{
    delete ui;
}
