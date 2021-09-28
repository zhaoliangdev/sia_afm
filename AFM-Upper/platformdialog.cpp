#include "platformdialog.h"
#include "ui_platformdialog.h"

PlatformDialog::PlatformDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlatformDialog)
{
    ui->setupUi(this);

    //实例化类
    //PMC400控制类
    pmcControl = new PMCCONTROL;

    //图像标定类
    transMarkTarget = new markTarget;

    //pmc400运动控制器初始化
    pmcControl->iniPMC400();

    //设定初始步长
    m_moveStep = 10.00;

    //设定运动初速度
    m_moveSpeed = 1.00;
    pmcControl->setXMoveSpeed(m_moveSpeed*10000); //单位是脉冲，mm转换成脉冲
    pmcControl->setYMoveSpeed(m_moveSpeed*10000);

    //ui界面初始化显示
    ui->lineEdit->setText(QString::number(m_moveStep)); //显示步长
    ui->lineEdit_2->setText(QString::number(m_moveSpeed)); //显示速度

    //设置与显示槽函数
    //ui界面更改参数后 点击回车 参数立即更改
    //设置步长 默认是10微米
    connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(setMoveStep()));

    //设置运动速度 默认1mm/毫秒
    connect(ui->lineEdit_2,SIGNAL(returnPressed()),this,SLOT(setMoveSpeed()));

}

//槽函数
//设定运动步长
void PlatformDialog::setMoveStep()
{
    double displayData; //界面显示 微米
    double stepCts;

    displayData = ui->lineEdit->text().toDouble();

    //定义取值范围10-10000微米
    if(displayData<=10)
    {
        stepCts = 10;
        ui->lineEdit->setText(QString::number(10)); //界面显示
    }
    else if(displayData>10000)
    {
        stepCts = 10000;
        ui->lineEdit->setText(QString::number(10000)); //界面显示
    }
    else
    {
        stepCts = displayData; //输入即是
    }

    //微米转换成移动距离毫米
    m_moveStep = 0.001 * stepCts;

}

//设定运动速度
void PlatformDialog::setMoveSpeed()
{
    //运动速度 mm/毫秒
    double displayData;
    double speedCts;
    displayData = ui->lineEdit_2->text().toDouble();

    //定义取值范围0.1-1.58
    if(displayData<=0)
    {
        speedCts = 0.1; //最小速度
        ui->lineEdit_2->setText(QString::number(0.10));
    }
    else if(displayData >= 1.58)
    {
        speedCts = 1.58;
        ui->lineEdit_2->setText(QString::number(1.58));
    }
    else
    {
        speedCts = displayData;
    }

    //速度转换成脉冲/毫秒
    m_moveSpeed = 10000 *speedCts;

    //XY轴设定运动速度
    pmcControl->setXMoveSpeed(m_moveSpeed);
    pmcControl->setYMoveSpeed(m_moveSpeed);
}

PlatformDialog::~PlatformDialog()
{
    delete ui;
}


//界面按钮槽函数
//////按钮点击一次 运动一个步长/////
//向前
void PlatformDialog::on_pushButton_clicked()
{
    pmcControl->setFrontOnceMove(m_moveStep); //每次移动一个步长
    pmcControl->setMoveStart();
}

//向后
void PlatformDialog::on_pushButton_3_clicked()
{
    pmcControl->setBackOnceMove(m_moveStep);
    pmcControl->setMoveStart();
}

//向左
void PlatformDialog::on_pushButton_2_clicked()
{
    pmcControl->setLeftOnceMove(m_moveStep);
    pmcControl->setMoveStart();
}

//向右
void PlatformDialog::on_pushButton_4_clicked()
{
    pmcControl->setRightOnceMove(m_moveStep);
    pmcControl->setMoveStart();
}

/////按住按钮持续运动/////
//向前
void PlatformDialog::on_pushButton_pressed()
{
    pmcControl->setFrontLastMove(); //持续运动
    pmcControl->pmc400->Command_Start(); //开始运动 不返回参数
}

void PlatformDialog::on_pushButton_3_pressed()
{
    pmcControl->setBackLastMove();
    pmcControl->pmc400->Command_Start(); //开始运动 不返回参数
}

void PlatformDialog::on_pushButton_2_pressed()
{
    pmcControl->setLeftLastMove();
    pmcControl->pmc400->Command_Start(); //开始运动 不返回参数
}

void PlatformDialog::on_pushButton_4_pressed()
{
    pmcControl->setRightLastMove();
    pmcControl->pmc400->Command_Start(); //开始运动 不返回参数
}

/////松开按钮 运动停止/////
//前
void PlatformDialog::on_pushButton_released()
{
    pmcControl->setMoveStop();
}

//后
void PlatformDialog::on_pushButton_3_released()
{
    pmcControl->setMoveStop();
}

//左
void PlatformDialog::on_pushButton_2_released()
{
    pmcControl->setMoveStop();
}

//右
void PlatformDialog::on_pushButton_4_released()
{
    pmcControl->setMoveStop();
}

/////左侧图像标定区域/////
//从文件夹读取图像
void PlatformDialog::on_pushButton_6_clicked()
{
    QString fileName;
    //QImage imageShow;

    //选择图像对话框
    fileName = QFileDialog::getOpenFileName(this,tr("选择扫描图像"),"D:/",tr("images(*.png *.jpeg *.bmp)"));

    if(fileName.isEmpty())
    {
        QMessageBox::warning(this,"Warning!","Failed to open the video!");
    }
    else
    {
        //原始图像读取
        m_fileImage.load(fileName);
    }

    //改变显示图像大小适应图像框
    QImage showImg;
    showImg = transMarkTarget->getShowImg(m_fileImage,ui->label_6->width(),ui->label_6->height());

    //图像显示
    ui->label_6->setPixmap(QPixmap::fromImage(showImg));

}
