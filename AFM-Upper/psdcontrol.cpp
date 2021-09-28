#include "psdcontrol.h"

PSDControl::PSDControl()
{

}

/////PSD数据转换/////
//垂直电压转换
double PSDControl::transVerPsdData(int tmpVerData)
{
    double tmpData = tmpVerData; //int转换成double
    double verDeflectData;

    //公式 可以自己修改
    verDeflectData = (tmpData/32768)*10;

    return verDeflectData;
}

//水平电压转换
double PSDControl::transHorPsdData(int tmpHorData)
{
    double tmpData = tmpHorData; //int转换成double
    double horDeflectData;

    //公式 可以自己修改
    horDeflectData = (tmpData/32768)*10;

    return horDeflectData;
}

//总电压转换
double PSDControl::transSumSignalData(int tmpSumData)
{
    double tmpData = tmpSumData; //int转换成double
    double sumDeflectData;

    //公式 可以自己修改
    sumDeflectData = (tmpData/32768)*10;

    return sumDeflectData;
}

/////压电陶瓷数据转换/////
//PZT数据转换
double PSDControl::transPztData(int tmpPztData)
{
    double tmpData = tmpPztData; //int转换成double
    double pztData;

    //公式 可以自己修改
    pztData = (tmpData/32768)*10;

    return pztData;
}
