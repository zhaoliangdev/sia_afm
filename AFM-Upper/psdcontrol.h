#ifndef PSDCONTROL_H
#define PSDCONTROL_H
//PSD数据转换类

class PSDControl
{
public:
    PSDControl();

public:
    /////PSD数据转换/////
    //垂直电压数据转换
    double transVerPsdData(int tmpVerData);

    //水平电压数据转换
    double transHorPsdData(int tmpHorData);

    //总电压数据转换
    double transSumSignalData(int tmpSumData);

    /////PZT数据转换/////
    //压电陶瓷数据转换
    double transPztData(int tmpPztData);
};

#endif // PSDCONTROL_H
