#include "scandialog.h"
#include "ui_scandialog.h"

ScanDialog::ScanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScanDialog)
{
    ui->setupUi(this);

    //类实例化
    HgtTrace = new TRANSDATA;
    HgtRetrace = new TRANSDATA;
    DftTrace = new TRANSDATA;
    DftRetrace = new TRANSDATA;

    //图像数据初始化
    HgtTrace->iniImgData();
    HgtRetrace->iniImgData();
    DftTrace->iniImgData();
    DftRetrace->iniImgData();

    //初始模式
    HgtTrace->m_bRaw = true;
    HgtTrace->m_bFit = false;
    HgtTrace->m_bZAverage = false;
    HgtTrace->m_bAllUpdate = true;

    HgtRetrace->m_bRaw = false;
    HgtRetrace->m_bFit = false;
    HgtRetrace->m_bZAverage = false;

    DftTrace->m_bRaw = true;
    DftTrace->m_bFit = false;
    DftTrace->m_bZAverage = false;
    DftTrace->m_bAllUpdate = true;

    DftRetrace->m_bRaw = false;
    DftRetrace->m_bFit = false;
    DftRetrace->m_bZAverage = false;

    setX(256); //设置横坐标

    /////信号与槽函数/////
    //三种模式
    //rawData
    //lineFit
    //zAverage
    connect(ui->comboBox_34,SIGNAL(currentIndexChanged(int)),this,SLOT(setHgtDataType()));
    connect(ui->comboBox_31,SIGNAL(currentIndexChanged(int)),this,SLOT(setHgtDataType()));

    connect(ui->comboBox_17,SIGNAL(currentIndexChanged(int)),this,SLOT(setDftDataType()));
    connect(ui->comboBox_15,SIGNAL(currentIndexChanged(int)),this,SLOT(setDftDataType()));
}

//曲线数据
//设置X轴数据
void ScanDialog::setX(int dataLength) //每行256个数据
{
    int i;
    for(i=0;i<dataLength;i++)
    {
        xPos.append(i);
    }
}

/////四条曲线/////
/////Height模式/////
////绘制正向轨迹曲线
void ScanDialog::drawHgtTraCurve()
{
    m_HgtTraCurve = new QwtPlotCurve;
    m_HgtTraCurve->setTitle("Trace"); //曲线标题
    m_HgtTraCurve->setPen(Qt::blue,1); //曲线颜色 粗细
    m_HgtTraCurve->setSamples(xPos,m_vHgtTraData); //数据传入曲线
    m_HgtTraCurve->attach(ui->qwtPlot);

    //绘制网格线
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableX(true);
    grid->enableY(true);
    grid->setMajorPen(Qt::black,0,Qt::DotLine);
    grid->attach(ui->qwtPlot);

    ui->qwtPlot->replot(); //曲线重绘
}

//绘制反向轨迹曲线
void ScanDialog::drawHgtRetraCurve()
{
    m_HgtRetraCurve = new QwtPlotCurve;
    m_HgtRetraCurve->setTitle("Retrace");
    m_HgtRetraCurve->setPen(Qt::red,1);
    m_HgtRetraCurve->setSamples(xPos,m_vHgtRetraData);
    m_HgtRetraCurve->attach(ui->qwtPlot);

    //绘制网格线
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableX(true);
    grid->enableY(true);
    grid->setMajorPen(Qt::black,0,Qt::DotLine);
    grid->attach(ui->qwtPlot);

    ui->qwtPlot->replot(); //曲线重绘
}

//曲线更新
void ScanDialog::updateHgtCurve()
{
    ui->qwtPlot->detachItems(); //清屏

    drawHgtTraCurve();
    drawHgtRetraCurve();

    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom,"X(Pixel)"); //x轴单位
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,"Height"); //y轴单位
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0,270); //x轴范围
    //ui->qwtPlot_6->setAxisScale(QwtPlot::yLeft,0.0,10.0);

    //曲线图例
    QwtLegend *legend = new QwtLegend;
    ui->qwtPlot->insertLegend(legend,QwtPlot::RightLegend);
}

/////偏差模式/////
//绘制曲线
//正向轨迹曲线
//反向轨迹曲线
void ScanDialog::drawDftTraCurve()
{
    m_DftTraCurve = new QwtPlotCurve;
    m_DftTraCurve->setTitle("Trace");
    m_DftTraCurve->setPen(Qt::blue,1);
    m_DftTraCurve->setSamples(xPos,m_vDftTraData);
    m_DftTraCurve->attach(ui->qwtPlot_2);

    //绘制网格线
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableX(true);
    grid->enableY(true);
    grid->setMajorPen(Qt::black,0,Qt::DotLine);
    grid->attach(ui->qwtPlot_2);

    ui->qwtPlot_2->replot(); //曲线重绘
}

void ScanDialog::drawDftRetraCurve()
{
    m_DftRetraCurve = new QwtPlotCurve;
    m_DftRetraCurve->setTitle("Rerace");
    m_DftRetraCurve->setPen(Qt::red,1);
    m_DftRetraCurve->setSamples(xPos,m_vDftRetraData);
    m_DftRetraCurve->attach(ui->qwtPlot_2);

    //绘制网格线
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableX(true);
    grid->enableY(true);
    grid->setMajorPen(Qt::black,0,Qt::DotLine);
    grid->attach(ui->qwtPlot_2);

    ui->qwtPlot_2->replot(); //曲线重绘
}

//曲线更新
void ScanDialog::updateDftCurve()
{
    ui->qwtPlot_2->detachItems(); //清屏重绘

    drawDftTraCurve();
    drawDftRetraCurve();

    ui->qwtPlot_2->setAxisTitle(QwtPlot::xBottom,"X(Pixel)");
    ui->qwtPlot_2->setAxisTitle(QwtPlot::yLeft,"Height");
    ui->qwtPlot_2->setAxisScale(QwtPlot::xBottom,0,270); //设置坐标轴范围
    //ui->qwtPlot_6->setAxisScale(QwtPlot::yLeft,0.0,10.0);

    QwtLegend *legend = new QwtLegend;
    ui->qwtPlot_2->insertLegend(legend,QwtPlot::RightLegend);
}

/////图像更新/////
//Height高度模式
void ScanDialog::updateHgtImg()
{
    QImage tmpImg;
    tmpImg = m_hgtShowImage.scaled(ui->frame_7->width(),ui->frame_7->height()); //改变大小

    //if
    ui->label_69->setPixmap(QPixmap::fromImage(tmpImg));
}

//偏差模式
void ScanDialog::updateDftImg()
{
    QImage tmpImg;
    tmpImg = m_dftShowImage.scaled(ui->frame_24->width(),ui->frame_24->height());

    ui->label_73->setPixmap(QPixmap::fromImage(tmpImg));
}

/////数据显示/////
//Height高度模式
void ScanDialog::updateHgtDisplay()
{
    if(HgtTrace->m_bAllUpdate)
    {
        ui->lineEdit_18->setText(QString::number(HgtTrace->m_iOffsetMin));
        ui->lineEdit_17->setText(QString::number(HgtTrace->m_iOffsetMax));

        ui->label_85->setNum(HgtTrace->m_iScaleData);
    }
    else if(HgtRetrace->m_bAllUpdate)
    {
        ui->lineEdit_18->setText(QString::number(HgtRetrace->m_iOffsetMin));
        ui->lineEdit_17->setText(QString::number(HgtRetrace->m_iOffsetMax));

        ui->label_85->setNum(HgtRetrace->m_iScaleData);
    }
}

//Deflect偏差模式
void ScanDialog::updateDftDisplay()
{
    if(DftTrace->m_bAllUpdate)
    {
        ui->lineEdit_23->setText(QString::number(DftTrace->m_iOffsetMin));
        ui->lineEdit_22->setText(QString::number(DftTrace->m_iOffsetMax));

        ui->label_86->setNum(DftTrace->m_iScaleData);
    }
    else if(DftRetrace->m_bAllUpdate)
    {
        ui->lineEdit_23->setText(QString::number(DftRetrace->m_iOffsetMin));
        ui->lineEdit_22->setText(QString::number(DftRetrace->m_iOffsetMax));

        ui->label_86->setNum(DftRetrace->m_iScaleData);
    }
}

ScanDialog::~ScanDialog()
{
    delete ui;
}

/////按钮槽函数/////
//Height模式
//手动Scale
//setScale
void ScanDialog::on_pushButton_clicked()
{
    int tmpScale;
    int tmpMin;
    int tmpMax;

    if(ui->comboBox_31->currentIndex()==0)
    {
        HgtTrace->m_bAllUpdate = true;
        HgtRetrace->m_bAllUpdate = false;

        tmpMin = ui->lineEdit_18->text().toInt();
        tmpMax = ui->lineEdit_17->text().toInt();

        if(tmpMax > tmpMin)
        {
            tmpScale = tmpMax - tmpMin;
        }
        else
        {
            tmpScale = 100; //最小值
        }

        ui->label_85->setNum(tmpScale);

        HgtTrace->setParameter(tmpMin,tmpMax);
    }

    else if(ui->comboBox_31->currentIndex()==1)
    {
        HgtRetrace->m_bAllUpdate = true;
        HgtTrace->m_bAllUpdate = false;

        tmpMin = ui->lineEdit_18->text().toInt();
        tmpMax = ui->lineEdit_17->text().toInt();

        if(tmpMax > tmpMin)
        {
            tmpScale = tmpMax - tmpMin;
        }
        else
        {
            tmpScale = 100;
        }

        ui->label_85->setNum(tmpScale);

        HgtRetrace->setParameter(tmpMin,tmpMax);
    }
}

//自动Scale
//autoScale
void ScanDialog::on_pushButton_3_clicked()
{
    int autoMin;
    int autoMax;

    if(ui->comboBox_31->currentIndex()==0)
    {
        HgtTrace->m_bAllUpdate = true;
        HgtRetrace->m_bAllUpdate = false;

        //AutoScale模式
        if(HgtTrace->m_bRaw)
        {
            autoMin = HgtTrace->getAutoOffset(HgtTrace->m_vHgtData).at(0);
            autoMax = HgtTrace->getAutoOffset(HgtTrace->m_vHgtData).at(1);

            HgtTrace->setParameter(autoMin,autoMax);

            updateHgtDisplay();
        }
        else if(HgtTrace->m_bFit)
        {
            autoMin = HgtTrace->getAutoOffset(HgtTrace->m_vFitData).at(0);
            autoMax = HgtTrace->getAutoOffset(HgtTrace->m_vFitData).at(1);

            HgtTrace->setParameter(autoMin,autoMax);

            updateHgtDisplay();
        }
        else if(HgtTrace->m_bZAverage)
        {
            autoMin = HgtTrace->getAutoOffset(HgtTrace->m_vZAverageData).at(0);
            autoMax = HgtTrace->getAutoOffset(HgtTrace->m_vZAverageData).at(1);

            HgtTrace->setParameter(autoMin,autoMax);

            updateHgtDisplay();
        }
    }
    else if(ui->comboBox_31->currentIndex()==1)
    {
        HgtRetrace->m_bAllUpdate = true;
        HgtTrace->m_bAllUpdate = false;

        //AutoScale模式
        if(HgtRetrace->m_bRaw)
        {
            autoMin = HgtRetrace->getAutoOffset(HgtRetrace->m_vHgtData).at(0);
            autoMax = HgtRetrace->getAutoOffset(HgtRetrace->m_vHgtData).at(1);

            HgtRetrace->setParameter(autoMin,autoMax);

            updateHgtDisplay();
        }
        else if(HgtRetrace->m_bFit)
        {
            autoMin = HgtRetrace->getAutoOffset(HgtRetrace->m_vFitData).at(0);
            autoMax = HgtRetrace->getAutoOffset(HgtRetrace->m_vFitData).at(1);

            HgtRetrace->setParameter(autoMin,autoMax);

            updateHgtDisplay();
        }
        else if(HgtRetrace->m_bZAverage)
        {
            autoMin = HgtRetrace->getAutoOffset(HgtRetrace->m_vZAverageData).at(0);
            autoMax = HgtRetrace->getAutoOffset(HgtRetrace->m_vZAverageData).at(1);

            HgtRetrace->setParameter(autoMin,autoMax);

            updateHgtDisplay();
        }
    }
}

//Deflect模式
//手动Scale
//setScale
void ScanDialog::on_pushButton_13_clicked()
{
    int tmpScale;
    int tmpMin;
    int tmpMax;

    if(ui->comboBox_15->currentIndex()==0)
    {
        DftTrace->m_bAllUpdate = true;
        DftRetrace->m_bAllUpdate = false;

        tmpMin = ui->lineEdit_23->text().toInt();
        tmpMax = ui->lineEdit_22->text().toInt();

        if(tmpMax > tmpMin)
        {
            tmpScale = tmpMax - tmpMin;
        }
        else
        {
            tmpScale = 100;
        }

        ui->label_86->setNum(tmpScale);

        DftTrace->setParameter(tmpMin,tmpMax);
    }
    else if(ui->comboBox_15->currentIndex()==1)
    {
        DftRetrace->m_bAllUpdate = true;
        DftTrace->m_bAllUpdate = false;

        tmpMin = ui->lineEdit_23->text().toInt();
        tmpMax = ui->lineEdit_22->text().toInt();

        if(tmpMax > tmpMin)
        {
            tmpScale = tmpMax - tmpMin;
        }
        else
        {
            tmpScale = 100;
        }

        ui->label_86->setNum(tmpScale);

        DftRetrace->setParameter(tmpMin,tmpMax);
    }

}

//自动Scale
//autoScale
void ScanDialog::on_pushButton_4_clicked()
{
    int autoMin;
    int autoMax;

    if(ui->comboBox_15->currentIndex()==0)
    {
        DftTrace->m_bAllUpdate = true;
        DftRetrace->m_bAllUpdate = false;

        //AutoScale模式
        if(DftTrace->m_bRaw)
        {
            autoMin = DftTrace->getAutoOffset(DftTrace->m_vHgtData).at(0);
            autoMax = DftTrace->getAutoOffset(DftTrace->m_vHgtData).at(1);

            DftTrace->setParameter(autoMin,autoMax);

            updateDftDisplay();
        }
        else if(DftTrace->m_bFit)
        {
            autoMin = DftTrace->getAutoOffset(DftTrace->m_vFitData).at(0);
            autoMax = DftTrace->getAutoOffset(DftTrace->m_vFitData).at(1);

            DftTrace->setParameter(autoMin,autoMax);

            updateDftDisplay();
        }
        else if(DftTrace->m_bZAverage)
        {
            autoMin = DftTrace->getAutoOffset(DftTrace->m_vZAverageData).at(0);
            autoMax = DftTrace->getAutoOffset(DftTrace->m_vZAverageData).at(1);

            DftTrace->setParameter(autoMin,autoMax);

            updateDftDisplay();
        }
    }
    else if(ui->comboBox_15->currentIndex()==1)
    {
        DftTrace->m_bAllUpdate = false;
        DftRetrace->m_bAllUpdate = true;

        //AutoScale模式
        if(DftRetrace->m_bRaw)
        {
            autoMin = DftRetrace->getAutoOffset(DftRetrace->m_vHgtData).at(0);
            autoMax = DftRetrace->getAutoOffset(DftRetrace->m_vHgtData).at(1);

            DftRetrace->setParameter(autoMin,autoMax);

            updateDftDisplay();
        }
        else if(DftRetrace->m_bFit)
        {
            autoMin = DftRetrace->getAutoOffset(DftRetrace->m_vFitData).at(0);
            autoMax = DftRetrace->getAutoOffset(DftRetrace->m_vFitData).at(1);

            DftRetrace->setParameter(autoMin,autoMax);

            updateDftDisplay();
        }
        else if(DftRetrace->m_bZAverage)
        {
            autoMin = DftRetrace->getAutoOffset(DftRetrace->m_vZAverageData).at(0);
            autoMax = DftRetrace->getAutoOffset(DftRetrace->m_vZAverageData).at(1);

            DftRetrace->setParameter(autoMin,autoMax);

            updateDftDisplay();
        }
    }
}

//槽函数
//高度模式
void ScanDialog::setHgtDataType()
{
    int autoMin;
    int autoMax;

    if(ui->comboBox_31->currentIndex()==0)
    {
        HgtTrace->m_bAllUpdate = true;
        HgtRetrace->m_bAllUpdate = false;

        if(ui->comboBox_34->currentIndex()==0)
        {
            HgtTrace->m_bRaw = true;
            HgtTrace->m_bFit = false;
            HgtTrace->m_bZAverage = false;

            autoMin = HgtTrace->getAutoOffset(HgtTrace->m_vHgtData).at(0);
            autoMax = HgtTrace->getAutoOffset(HgtTrace->m_vHgtData).at(1);

            HgtTrace->setParameter(autoMin,autoMax);

            updateHgtDisplay();
        }
        else if(ui->comboBox_34->currentIndex()==1)
        {
            HgtTrace->m_bRaw = false;
            HgtTrace->m_bFit = true;
            HgtTrace->m_bZAverage = false;

            autoMin = HgtTrace->getAutoOffset(HgtTrace->m_vFitData).at(0);
            autoMax = HgtTrace->getAutoOffset(HgtTrace->m_vFitData).at(1);

            HgtTrace->setParameter(autoMin,autoMax);

            updateHgtDisplay();
        }
        else if(ui->comboBox_34->currentIndex()==2)
        {
            HgtTrace->m_bRaw = false;
            HgtTrace->m_bFit = false;
            HgtTrace->m_bZAverage = true;

            autoMin = HgtTrace->getAutoOffset(HgtTrace->m_vZAverageData).at(0);
            autoMax = HgtTrace->getAutoOffset(HgtTrace->m_vZAverageData).at(1);

            HgtTrace->setParameter(autoMin,autoMax);

            updateHgtDisplay();
        }
    }

    else if(ui->comboBox_31->currentIndex()==1)
    {
        HgtRetrace->m_bAllUpdate = true;
        HgtTrace->m_bAllUpdate = false;

        if(ui->comboBox_34->currentIndex()==0)
        {
            HgtRetrace->m_bRaw = true;
            HgtRetrace->m_bFit = false;
            HgtRetrace->m_bZAverage = false;

            autoMin = HgtRetrace->getAutoOffset(HgtRetrace->m_vHgtData).at(0);
            autoMax = HgtRetrace->getAutoOffset(HgtRetrace->m_vHgtData).at(1);

            HgtRetrace->setParameter(autoMin,autoMax);

            updateHgtDisplay();
        }
        else if(ui->comboBox_34->currentIndex()==1)
        {
            HgtRetrace->m_bRaw = false;
            HgtRetrace->m_bFit = true;
            HgtRetrace->m_bZAverage = false;

            autoMin = HgtRetrace->getAutoOffset(HgtRetrace->m_vFitData).at(0);
            autoMax = HgtRetrace->getAutoOffset(HgtRetrace->m_vFitData).at(1);

            HgtRetrace->setParameter(autoMin,autoMax);

            updateHgtDisplay();
        }
        else if(ui->comboBox_34->currentIndex()==2)
        {
            HgtRetrace->m_bRaw = false;
            HgtRetrace->m_bFit = false;
            HgtRetrace->m_bZAverage = true;

            autoMin = HgtRetrace->getAutoOffset(HgtRetrace->m_vZAverageData).at(0);
            autoMax = HgtRetrace->getAutoOffset(HgtRetrace->m_vZAverageData).at(1);

            HgtRetrace->setParameter(autoMin,autoMax);

            updateHgtDisplay();
        }
    }

}

//偏差模式
void ScanDialog::setDftDataType()
{
    int autoMin;
    int autoMax;

    if(ui->comboBox_15->currentIndex()==0)
    {
        DftTrace->m_bAllUpdate = true;
        DftRetrace->m_bAllUpdate = false;

        if(ui->comboBox_17->currentIndex()==0)
        {
            DftTrace->m_bRaw = true;
            DftTrace->m_bFit = false;
            DftTrace->m_bZAverage = false;

            autoMin = DftTrace->getAutoOffset(DftTrace->m_vHgtData).at(0);
            autoMax = DftTrace->getAutoOffset(DftTrace->m_vHgtData).at(1);

            DftTrace->setParameter(autoMin,autoMax);

            updateDftDisplay();
        }
        else if(ui->comboBox_17->currentIndex()==1)
        {
            DftTrace->m_bRaw = false;
            DftTrace->m_bFit = true;
            DftTrace->m_bZAverage = false;

            autoMin = DftTrace->getAutoOffset(DftTrace->m_vFitData).at(0);
            autoMax = DftTrace->getAutoOffset(DftTrace->m_vFitData).at(1);

            DftTrace->setParameter(autoMin,autoMax);

            updateDftDisplay();
        }
        else if(ui->comboBox_17->currentIndex()==2)
        {
            DftTrace->m_bRaw = false;
            DftTrace->m_bFit = false;
            DftTrace->m_bZAverage = true;

            autoMin = DftTrace->getAutoOffset(DftTrace->m_vZAverageData).at(0);
            autoMax = DftTrace->getAutoOffset(DftTrace->m_vZAverageData).at(1);

            DftTrace->setParameter(autoMin,autoMax);

            updateDftDisplay();
        }
    }

    else if(ui->comboBox_15->currentIndex()==1)
    {
        DftRetrace->m_bAllUpdate = true;
        DftTrace->m_bAllUpdate = false;

        if(ui->comboBox_17->currentIndex()==0)
        {
            DftRetrace->m_bRaw = true;
            DftRetrace->m_bFit = false;
            DftRetrace->m_bZAverage = false;

            autoMin = DftRetrace->getAutoOffset(DftRetrace->m_vHgtData).at(0);
            autoMax = DftRetrace->getAutoOffset(DftRetrace->m_vHgtData).at(1);

            DftRetrace->setParameter(autoMin,autoMax);

            updateDftDisplay();
        }
        else if(ui->comboBox_17->currentIndex()==1)
        {
            DftRetrace->m_bRaw = false;
            DftRetrace->m_bFit = true;
            DftRetrace->m_bZAverage = false;

            autoMin = DftRetrace->getAutoOffset(DftRetrace->m_vFitData).at(0);
            autoMax = DftRetrace->getAutoOffset(DftRetrace->m_vFitData).at(1);

            DftRetrace->setParameter(autoMin,autoMax);

            updateDftDisplay();
        }
        else if(ui->comboBox_17->currentIndex()==2)
        {
            DftRetrace->m_bRaw = false;
            DftRetrace->m_bFit = false;
            DftRetrace->m_bZAverage = true;

            autoMin = DftRetrace->getAutoOffset(DftRetrace->m_vZAverageData).at(0);
            autoMax = DftRetrace->getAutoOffset(DftRetrace->m_vZAverageData).at(1);

            DftRetrace->setParameter(autoMin,autoMax);

            updateDftDisplay();
        }
    }
}

/////PSD模块/////
