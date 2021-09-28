#ifndef TRANSDATA_H
#define TRANSDATA_H

#define PZT 15000.0 //宏定义压电陶瓷伸缩

#include <QVector>
#include <QDebug>
#include <QRgb>
#include <QImage>

class TRANSDATA
{
public:
    TRANSDATA();

public:

    //AD转换成高度Hgt
    QVector<double> getHgtLineData(QVector<short int> vADLineData);

    /////曲线数据/////
    //AD数据转换成double
    QVector<double> getDoubleADData(QVector<short int> vADLineData);

    /////行数据转换/////
    //单行数据转换
    QVector<int> processLine(QVector<short> vADLineData);

    //有改变出现时
    //全数据按行改变
    QImage processAllLine(QVector<double> vAllHgtData);

    /////三种数据转换/////
    //RawLine = HgtLine //默认模式是Raw

    //Hgt转换成Linefit
    QVector<double> lineFitData(QVector<double> vHgtLineData);

    //Hgt转换成ZAverage
    QVector<double> lineZAverageData(QVector<double> vHgtLineData);

    /////图像/////
    //图像数据初始化
    void iniImgData();

    //offset
    QVector<double> getOffsetLineData(QVector<double> vLineData,int iOffset_min,int iOffset_max);

    //scale
    QVector<int> transImgData(QVector<double> vOffsetLineData,int iScaleData);

    //AutoScale
    //返回值数组的第一个元素是Min；
    //返回值数组的第二个元素是Max；
    QVector<int> getAutoOffset(QVector<double> vHgtData);

    //输出图像
    QImage outputImg(QVector<int> vImgLine,int iLineNum);

    /////数据存储/////
    //AD数据存储
    void recordADData(QVector<short int> vADLineData);

    //Hgt数据存储
    void recordHgtData(QVector<double> vHgtLineData);

    //Fit数据存储
    void recordFitData(QVector<double> vFitLineData);

    //ZAverage数据存储
    void recordZAverageData(QVector<double> vZAverageLineData);

    //参数设置
    void setParameter(int iOffsetMin,int iOffsetMax);

//全局变量
public:

    unsigned char *ImgData; //图像数据

    int RESOLUTION;

    //图像颜色表
    QVector<QRgb> colorTable{4282122240, 4282122240, 4282187776, 4282187776, 4282253312, 4282318848, 4282318848, 4282384384, 4282449920, 4282449920, 4282515456, 4282580992, 4282580992, 4282646528, 4282712064, 4282712064, 4282777600, 4282843136, 4282843136, 4282908672, 4282974208, 4282974208, 4283039744, 4283105280, 4283105280, 4283170816, 4283236352, 4283236352, 4283301888, 4283367424, 4283367424, 4283432960, 4283498496, 4283498496, 4283564032, 4283629568, 4283629568, 4283695104, 4283760640, 4283760640, 4283826176, 4283891712, 4283891712, 4283957248, 4284022784, 4284022784, 4284088320, 4284153856, 4284153856, 4284219392, 4284284928, 4284284928, 4284350464, 4284350464, 4284416000, 4284481536, 4284481536, 4284547072, 4284612608, 4284612608, 4284678144, 4284743680, 4284743680, 4284809216, 4284874752, 4284874752, 4284940288, 4285005824, 4285005824, 4285071360, 4285136896, 4285136896, 4285202432, 4285267968, 4285267968, 4285333504, 4285399040, 4285399040, 4285464576, 4285530112, 4285530112, 4285595648, 4285661184, 4285661184, 4285726720, 4285792256, 4285792768, 4285858816, 4285924864, 4285925376, 4285991424, 4286057472, 4286057984, 4286124032, 4286190080, 4286190592, 4286256640, 4286322688, 4286323200, 4286389248, 4286455296, 4286455808, 4286521856, 4286522368, 4286588416, 4286654464, 4286654976, 4286721024, 4286787072, 4286787584, 4286853888, 4286919936, 4286920448, 4286986496, 4287052544, 4287053056, 4287119104, 4287185152, 4287185664, 4287251712, 4287317760, 4287318272, 4287384320, 4287450368, 4287450880, 4287516928, 4287582976, 4287583488, 4287649536, 4287715584, 4287716096, 4287782144, 4287848192, 4287848704, 4287914752, 4287981056, 4287981568, 4288047616, 4288113664, 4288114176, 4288180224, 4288246272, 4288246784, 4288312832, 4288378880, 4288379392, 4288445440, 4288511488, 4288512000, 4288578048, 4288644096, 4288644608, 4288710656, 4288711168, 4288777216, 4288843264, 4288843776, 4288909824, 4288975872, 4288976384, 4289042688, 4289108736, 4289109248, 4289175296, 4289241344, 4289241856, 4289307904, 4289373952, 4289374464, 4289440512, 4289506560, 4289506562, 4289572100, 4289637638, 4289637640, 4289703178, 4289768716, 4289768718, 4289834256, 4289899794, 4289899796, 4289965334, 4290030872, 4290030874, 4290096412, 4290161950, 4290161952, 4290227490, 4290293028, 4290293030, 4290358568, 4290424106, 4290424108, 4290489646, 4290555184, 4290555187, 4290620725, 4290686263, 4290686265, 4290751803, 4290817341, 4290817343, 4290882881, 4290882883, 4290948421, 4291013959, 4291013961, 4291079499, 4291145037, 4291145039, 4291210577, 4291276115, 4291276117, 4291341655, 4291407193, 4291407195, 4291472733, 4291538271, 4291538273, 4291603811, 4291669350, 4291669352, 4291734890, 4291800428, 4291800430, 4291865968, 4291931506, 4291931508, 4291997046, 4292062584, 4292062586, 4292128124, 4292193662, 4292193664, 4292259202, 4292324740, 4292324742, 4292390280, 4292455818, 4292455820, 4292521358, 4292586896, 4292586898, 4292652436, 4292717974, 4292717977, 4292783515, 4292849053, 4292849055, 4292914593, 4292980131, 4292980133, 4293045671, 4293045673, 4293111211, 4293176749};

    //模式选择bool
    bool m_bAllUpdate = false;
    bool m_bFit = false;
    bool m_bZAverage = false;
    bool m_bRaw = false;

    //offset&scale
    //scale默认是第一行数据的AutoScale
    int m_iOffsetMin;
    int m_iOffsetMax;
    int m_iScaleData;

    /////数据链表/////
    QVector<short int> m_vADData; //原始AD数据
    QVector<double> m_vHgtData; //Hgt数据 Raw
    QVector<double> m_vFitData; //拟合Fit数据
    QVector<double> m_vZAverageData; //去除Z向平均值数据
};

#endif // TRANSDATA_H
