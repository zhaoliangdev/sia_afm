#include "transdata.h"

TRANSDATA::TRANSDATA()
{
    //iniImgData();
    RESOLUTION = 256;
}

//AD->Height
QVector<double> TRANSDATA::getHgtLineData(QVector<short> vADLineData)
{
    QVector<double> tmpData;
    double tmpHgt;
    int i;
    for(i=0;i<RESOLUTION;i++)
    {
        //15nm PZT
        //转换公式
        tmpHgt = (vADLineData[i] + 32767)*(PZT/65536.0); // 15000.0/65536

        tmpData.append(tmpHgt);
    }

    return tmpData;
}

//生成曲线Y轴数组
//int->double
QVector<double> TRANSDATA::getDoubleADData(QVector<short> vADLineData)
{
    QVector<double> tmpData;

    int bmpWidth;
    double tmpAD;
    bmpWidth = RESOLUTION;

    int i;
    for(i=0;i<bmpWidth;i++)
    {
        tmpAD = vADLineData[i];
        tmpData.append(tmpAD);
    }

    return tmpData;
}

//图像数据初始化
void TRANSDATA::iniImgData()
{
    ImgData = new unsigned char[RESOLUTION*RESOLUTION];

    int i;
    for(i=0;i<RESOLUTION*RESOLUTION;i++)
    {
        ImgData[i] = 128;
    }
}

/////行数据转换/////
//单行数据转换
QVector<int> TRANSDATA::processLine(QVector<short> vADLineData)
{
    QVector<int> tmpData;

    QVector<double> vHgtLineData;
    QVector<double> vFitLineData;
    QVector<double> vZAverageLineData;

    //临时行数据
    QVector<double> vLineData;

    QVector<double> vOffsetLineData; //截取 去除上下

    vHgtLineData = getHgtLineData(vADLineData); //AD行数据转换成高度行数据
    vFitLineData = lineFitData(vHgtLineData); //Hgt行数据进行拟合
    vZAverageLineData = lineZAverageData(vHgtLineData); //Hgt行数据去除Z向平均值

    //数据存储
    recordADData(vADLineData); //AD行数据存储
    recordHgtData(vHgtLineData); //Hgt行数据存储/Raw
    recordFitData(vFitLineData); //拟合行数据存储
    recordZAverageData(vZAverageLineData); //去除Z向平均值数据存储

    //模式判断
    if(m_bFit == true)
    {
        vLineData = vFitLineData;
    }
    else if(m_bZAverage == true)
    {
        vLineData = vZAverageLineData;
    }
    else if(m_bRaw == true)
    {
        vLineData = vHgtLineData;
    }

    vOffsetLineData = getOffsetLineData(vLineData,m_iOffsetMin,m_iOffsetMax);

    tmpData = transImgData(vOffsetLineData,m_iScaleData);

    return tmpData;
}

//全数据转换
QImage TRANSDATA::processAllLine(QVector<double> vAllHgtData)
{
    QVector<double> vTmpLineData;
    QVector<double> vTmpOffsetLineData;
    QVector<int> vTmpImgLineData;

    QImage img;

    int iSize;
    int iLineCount;

    double tmpData;

    iSize = vAllHgtData.size();
    iLineCount = iSize/RESOLUTION;

    int i;
    int j;
    for(i=0;i<iLineCount;i++)
    {
        for(j=0;j<RESOLUTION;j++)
        {
            tmpData = vAllHgtData[i*RESOLUTION+j];

            vTmpLineData.append(tmpData);

        }

        vTmpOffsetLineData = getOffsetLineData(vTmpLineData,m_iOffsetMin,m_iOffsetMax);
        vTmpImgLineData = transImgData(vTmpOffsetLineData,m_iScaleData);

        img = outputImg(vTmpImgLineData,i);

        vTmpLineData.clear();
    }

    return img;
}

/////三种数据转换/////
//RawLine = HgtLine

//Hgt转换成LineFit
QVector<double> TRANSDATA::lineFitData(QVector<double> vHgtLineData)
{
    QVector<double> tmpData;

    int bmpWidth; //图像宽度
    bmpWidth = RESOLUTION;

    // 曲线拟合
    double fHeight;//每一点对应的高度值 单位nm
    double dMeanX; //图像X均值
    double dMeanPztY;
    double dSumX, dSumPztY, dSumPztXY, dSumX2;
    double dFitData;
    dSumX =0;
    dSumPztY = 0;
    dSumPztXY = 0;
    dSumX2 = 0;

    double a,b; //拟合曲线 y = ax + b;

    int i;
    for(i=0;i<bmpWidth;i++)
    {
        dSumX += i + 1;
        dSumX2 += (i + 1)*(i + 1);
        dSumPztXY += (i + 1)*vHgtLineData[i];

        dSumPztY += vHgtLineData[i];
    }

    //求平均值
    //求平均值
    dMeanX = dSumX/bmpWidth;
    dMeanPztY = dSumPztY/bmpWidth; //求一行扫描数据均值

    //求系数a,b,c,d
    a = (dSumPztXY - bmpWidth*dMeanX*dMeanPztY)/(dSumX2-bmpWidth*dMeanX*dMeanX);
    b = dMeanPztY - a*dMeanX;

    //曲线拟合
    //数据存储
    int j;
    for(j=0;j<bmpWidth;j++)
    {
        dFitData = (a*(j+1) + b);

        fHeight = (vHgtLineData[j] - dFitData)*0.18310547;///6.6;//*6400.0/(2*65535);///6.6;
        tmpData.append(fHeight);

        //wholeHgtData.append(fHeight)
        //qDebug()<<fHeight;
    }

    return tmpData;
}

//Hgt转换成ZAverage
QVector<double> TRANSDATA::lineZAverageData(QVector<double> vHgtLineData)
{
    QVector<double> returnData;

    int bmpWidth; //图像宽度
    bmpWidth = RESOLUTION;

    double tmpData;
    double tmpData2;
    double sum = 0;
    double mean;
    int i;
    int j;
    for(i=0;i<bmpWidth;i++)
    {
        tmpData = vHgtLineData[i];
        sum = sum + tmpData;
    }

    mean = sum/bmpWidth;
    for(j=0;j<bmpWidth;j++)
    {
        tmpData2 = vHgtLineData[j] - mean;
        returnData.append(tmpData2);
    }

    return  returnData;
}

/////图像/////
//offset
QVector<double> TRANSDATA::getOffsetLineData(QVector<double> vLineData,int iOffset_min,int iOffset_max)
{
    QVector<double> tmpData;

    double tmpHgt1;
    double tmpHgt2;

    int i;
    for(i=0;i<vLineData.size();i++)
    {
        tmpHgt1 = vLineData[i];

        if(tmpHgt1>iOffset_max)
        {
            tmpHgt2 = iOffset_max;
        }
        else if(tmpHgt1<iOffset_min)
        {
            tmpHgt2 = iOffset_min;
        }
        else
        {
            tmpHgt2 = tmpHgt1;
        }

        tmpData.append(tmpHgt2);

    }

    return tmpData;
}

//获取Auto
QVector<int> TRANSDATA::getAutoOffset(QVector<double> vHgtData)
{
    QVector<int> offsetData;

    double minData;
    double maxData;

    int autoOffsetMin;
    int autoOffsetMax;

    auto autoMin = std::min_element(vHgtData.begin(),vHgtData.end());
    auto autoMax = std::max_element(vHgtData.begin(),vHgtData.end());

    minData = *autoMin;
    maxData = *autoMax;

    autoOffsetMin = minData - minData*0.1;
    autoOffsetMax = maxData + maxData*0.1;

    offsetData.append(autoOffsetMin);
    offsetData.append(autoOffsetMax);

    return offsetData;
}

//scale
QVector<int> TRANSDATA::transImgData(QVector<double> vOffsetLineData,int iScaleData)
{
    QVector<int> vTmpImgData;

    int tmpImgData;
    double tmpLineData;
    int i;
    for(i=0;i<vOffsetLineData.size();i++)
    {
        tmpLineData = vOffsetLineData[i];
        tmpImgData = 128 + 127*(2*tmpLineData -(m_iOffsetMax+m_iOffsetMin))/iScaleData;//公式

        vTmpImgData.append(tmpImgData);
    }

    return vTmpImgData;
}

/////数据存储/////
//原始AD数据存储
void TRANSDATA::recordADData(QVector<short> vADLineData)
{
    short int tmpData;
    int i;
    int bmpWidth;
    bmpWidth = RESOLUTION;

    for(i=0;i<bmpWidth;i++)
    {
        tmpData = vADLineData[i];
        m_vADData.append(tmpData);
    }
}

//Hgt数据存储Raw
void TRANSDATA::recordHgtData(QVector<double> vHgtLineData)
{
    double tmpData;
    int i;
    int bmpWidth;
    bmpWidth = RESOLUTION;

    for(i=0;i<bmpWidth;i++)
    {
        tmpData = vHgtLineData[i];
        m_vHgtData.append(tmpData);
    }
}

//拟合数据存储Fit
void TRANSDATA::recordFitData(QVector<double> vFitLineData)
{
    double tmpData;
    int i;
    int bmpWidth;
    bmpWidth = RESOLUTION;

    for(i=0;i<bmpWidth;i++)
    {
        tmpData = vFitLineData[i];
        m_vFitData.append(tmpData);
    }
}

//去除Z向平均值数据存储
void TRANSDATA::recordZAverageData(QVector<double> vZAverageLineData)
{
    double tmpData;
    int i;
    int bmpWidth;
    bmpWidth = RESOLUTION;

    for(i=0;i<bmpWidth;i++)
    {
        tmpData = vZAverageLineData[i];
        m_vZAverageData.append(tmpData);
    }
}

//图像输出
QImage TRANSDATA::outputImg(QVector<int> vImgLine,int iLineNum)
{
    int bmpWidth;
    bmpWidth = RESOLUTION;

    int tmpData;
    int i;
    for(i=0;i<bmpWidth;i++)
    {
        tmpData = vImgLine[i];

        //一整行数据存入图像
        *(ImgData + bmpWidth*iLineNum + i) = tmpData;
    }

    QImage Image(ImgData,RESOLUTION,RESOLUTION,QImage::Format_Indexed8);

    //设置颜色表
    Image.setColorTable(colorTable);

    return Image;
}

//参数设置
void TRANSDATA::setParameter(int iOffsetMin, int iOffsetMax)
{
    m_iOffsetMin = iOffsetMin;
    m_iOffsetMax = iOffsetMax;

    if(m_iOffsetMax > m_iOffsetMin)
    {
        m_iScaleData = m_iOffsetMax - m_iOffsetMin;
    }
    else
    {
        m_iScaleData = 100;
    }
}
