#include "pmccontrol.h"

PMCCONTROL::PMCCONTROL()
{

}

//初始化
void PMCCONTROL::iniPMC400()
{
    //实例化pmc400类
    pmc400 = new PMC400;

    //pmc400初始化
    pmc400->Init();

    //pmc400自动连接
    //默认IP地址192.168.1.101 默认端口80
    pmc400->Connect(pmc400->pmc400ip,pmc400->port);

    //设置X、Y轴的初始速度
    pmc400->Set_StartSpeed_Convert(1,10000);
    pmc400->Set_FinalSpeed_Convert(1,10000);
    pmc400->Set_StartSpeed_Convert(2,10000);
    pmc400->Set_FinalSpeed_Convert(2,10000);

    //保存设置
    pmc400->Save_Setup();

}

//四个轴的移动
//每次移动一个步长
//前
void PMCCONTROL::setFrontOnceMove(double stepData)
{
    pmc400->Set_CloseMoveDistance(2,-1000*(stepData));
    //pmc400->Set_MoveDistance_Convert(2,10);
}

//后
void PMCCONTROL::setBackOnceMove(double stepData)
{
    pmc400->Set_CloseMoveDistance(2,1000*(stepData));
}

//左
void PMCCONTROL::setLeftOnceMove(double stepData)
{
    pmc400->Set_CloseMoveDistance(1,1000*(stepData));
}

//右
void PMCCONTROL::setRightOnceMove(double stepData)
{
    pmc400->Set_CloseMoveDistance(1,-1000*(stepData));
}

//点击即刻开始持续移动
//1000000表示运动距离很大，即可保证持续运动
//按钮抬起，移动结束
//前
void PMCCONTROL::setFrontLastMove()
{
    pmc400->Set_CloseMoveDistance(2,-1000000);
}

//后
void PMCCONTROL::setBackLastMove()
{
    pmc400->Set_CloseMoveDistance(2,1000000);
}

//左
void PMCCONTROL::setLeftLastMove()
{
    pmc400->Set_CloseMoveDistance(1,1000000);
}

//右
void PMCCONTROL::setRightLastMove()
{
    pmc400->Set_CloseMoveDistance(1,-1000000);
}

//运动状态相关
//开始运动
void PMCCONTROL::setMoveStart()
{
    pmc400->StartMove();
}

//开始运动
//不返回函数
void PMCCONTROL::setMoveStart_Command()
{
    pmc400->Command_Start();
}

//停止运动
void PMCCONTROL::setMoveStop()
{
    pmc400->StopMove();
}

//回零
//x轴
void PMCCONTROL::goBackX()
{
    pmc400->HomeBack(1);
}

//y轴
void PMCCONTROL::goBackY()
{
    pmc400->HomeBack(2);
}

//xy轴一起
void PMCCONTROL::PMC400goBack()
{
    pmc400->HomeBack(1);
    pmc400->HomeBack(2);
}

/////运动速度相关/////
//设定运动速度
//设定前后运动速度
void PMCCONTROL::setYMoveSpeed(double speedData)
{
    //保持匀速运动
    //初速度与末速度相同
    pmc400->Set_StartSpeed_Convert(2,speedData);
    pmc400->Set_FinalSpeed_Convert(2,speedData);

    //保存状态
    pmc400->Save_Setup();
}

//设定左右运动速度
void PMCCONTROL::setXMoveSpeed(double speedData)
{
    //保持匀速运动
    //初速度与末速度相同
    pmc400->Set_StartSpeed_Convert(1,speedData);
    pmc400->Set_FinalSpeed_Convert(1,speedData);

    //保存状态
    pmc400->Save_Setup();
}
