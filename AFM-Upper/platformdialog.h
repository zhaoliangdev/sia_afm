#ifndef PLATFORMDIALOG_H
#define PLATFORMDIALOG_H

#include <QDialog>
#include <QVector>
#include <QDebug>
#include <QImage>
#include <QMessageBox>
#include <QFileDialog>

//PMC400控制类的头文件
#include "pmccontrol.h"

//标定图像转换类的头文件
#include "marktarget.h"

namespace Ui {
class PlatformDialog;
}

class PlatformDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlatformDialog(QWidget *parent = nullptr);
    ~PlatformDialog();

/////右侧PMC400控制区域/////
public:

    //定义类
    PMCCONTROL *pmcControl;

public:

    //步长 初始值为1.00
    double m_moveStep;

    //运动速度 单位mm/s
    //初始值为1.00mm/s
    //最大值为1.58mm/s
    double m_moveSpeed;

//界面上的槽函数
//设定参数
public slots:

    //自定义步长
    void setMoveStep();

    //自定义速度
    void setMoveSpeed();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_pressed();

    void on_pushButton_3_pressed();

    void on_pushButton_2_pressed();

    void on_pushButton_4_pressed();

    void on_pushButton_released();

    void on_pushButton_3_released();

    void on_pushButton_2_released();

    void on_pushButton_4_released();

    /////左侧标定区域/////
    //读取图像按钮 从文件夹读取图像
    void on_pushButton_6_clicked();

public:

    //图像标定类
    markTarget *transMarkTarget;

    //从文件夹读取的原始图像
    QImage m_fileImage;

private:
    Ui::PlatformDialog *ui;
};

#endif // PLATFORMDIALOG_H
