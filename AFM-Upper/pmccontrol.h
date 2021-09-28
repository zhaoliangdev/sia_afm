#ifndef PMCCONTROL_H
#define PMCCONTROL_H

//PMC400库的头文件
//如有需要
//可以根据三英说明书
//自行封装函数
#include "pmc400.h"

class PMCCONTROL
{
public:
    PMCCONTROL();

public:

    //实例化PMC400对象
    PMC400 *pmc400;

    //初始化函数
    //PMC400初始化
    void iniPMC400();

    //四个轴的运动
    //每次运动一个步长
    //前
    void setFrontOnceMove(double stepData);

    //后
    void setBackOnceMove(double stepData);

    //左
    void setLeftOnceMove(double stepData);

    //右
    void setRightOnceMove(double stepData);

    //四个轴运动
    //持续运动
    //前
    void setFrontLastMove();

    //后
    void setBackLastMove();

    //左
    void setLeftLastMove();

    //右
    void setRightLastMove();

    /////运动状态/////
    //运动开始
    void setMoveStart();

    //运动开始
    //不返回函数
    void setMoveStart_Command();

    //运动停止
    void setMoveStop();

    //回零运动
    //尽量不要使用
    //x轴回零
    void goBackX();

    //y轴回零
    void goBackY();

    //x轴y轴一起回零
    void PMC400goBack();

    /////运动速度相关/////
    //PMC400设定前后运动速度
    void setYMoveSpeed(double speedData);

    //PMC400设定左右运动速度
    void setXMoveSpeed(double speedData);

};

#endif // PMCCONTROL_H
