#ifndef PMC400_H
#define PMC400_H

#include <QtCore/qglobal.h>

#if defined(PMC400_QT_CPP_LIBRARY)
#  define PMC400_QT_CPPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PMC400_QT_CPPSHARED_EXPORT Q_DECL_IMPORT
#endif
#include <iostream>
#include <string>
#include <string.h>
using namespace std;
#include <sstream>
//#include <unistd.h>

#include <QTcpSocket>

#include <WinSock2.h>
#include <windows.h>
#include <MMSystem.h>

class  CClientNet
{
public:
    //连接上指定服务器
    int Connect(int port,const char* address);
    //发送信息
    int SendMsg(const char * msg);
    int SendMsg(const char * msg, int len);
    //接收信息
    int RecvMsg(char *msg, int len);
    //关闭
    void Close();

private:
    QTcpSocket* mp_clientSocket;
    SOCKET m_sock;
};

class PMC400_string
{
public:
    void PMC400_String();
    string PID_Command_String;
    //发送命令信息初始化
    string Command_String;
    //下载命令消息初始化
    string DownLoad_String;
    //错误信息初始化
    string Error_String;
    //连接信息
    string Connect_status;
    //pmc400设备序列号
    string sn;
    //实时信息初始化
    string Info_String;
        //"已经处于连接状态"
        //"连接成功"
        //"连接失败，请稍后重试"
        //"连接已断开，请重新连接"
        //"下载程序正在运行"
        //"发送指令失败"
        //"发送下载指令失败"
        //"数据校验出错"
        //"通讯出错"
    string Data_String;
};


class PMC400_QT_CPPSHARED_EXPORT PMC400
{

private:
    static PMC400* m_pmac400;
public:
    PMC400(){ m_pmac400 = this;}
    //构造函数
    void Init();
    void PMC400_String();
    PMC400_string m_string;

    //应在新建的工程文件中重新建立一个一模一样的结构体,或者包含本头文件
    typedef struct PMC400_STATUS
    {
    public:
        //初速度
        int X_StartSpeed;
        int Y_StartSpeed;
        int Z_StartSpeed;
        int U_StartSpeed;

        //加速度
        int X_AccSpeed;
        int Y_AccSpeed;
        int Z_AccSpeed;
        int U_AccSpeed;

        //终速度
        int X_FinalSpeed;
        int Y_FinalSpeed;
        int Z_FinalSpeed;
        int U_FinalSpeed;

        //当前位置
        int X_CurentPosition;
        int Y_CurentPosition;
        int Z_CurentPosition;
        int U_CurentPosition;

        //位置编码
        int X_PositionEncode;
        int Y_PositionEncode;
        int Z_PositionEncode;
        int U_PositionEncode;

        //移动指令
        int X_Is_MoveCommand;
        int Y_Is_MoveCommand;
        int Z_Is_MoveCommand;
        int U_Is_MoveCommand;

        //IO输入输出
        int IO_IN;
        int IO_OUT;

        //轴移动判断
        int X_Is_Moving;
        int Y_Is_Moving;
        int Z_Is_Moving;
        int U_Is_Moving;

        //剩余脉冲数
        int X_Pulse_Remaining;
        int Y_Pulse_Remaining;
        int Z_Pulse_Remaining;
        int U_Pulse_Remaining;

        //轴细分数
        int X_MicroSteps;
        int Y_MicroSteps;
        int Z_MicroSteps;
        int U_MicroSteps;

        //轴的导程
        int X_ScrewLead;
        int Y_ScrewLead;
        int Z_ScrewLead;
        int U_ScrewLead;

        //轴的转速比
        int X_SpeedRatio;
        int Y_SpeedRatio;
        int Z_SpeedRatio;
        int U_SpeedRatio;

        //轴的电流
        int X_Electric_Current;
        int Y_Electric_Current;
        int Z_Electric_Current;
        int U_Electric_Current;

        //分辨率
        int X_Resolution_Ratio;
        int Y_Resolution_Ratio;
        int Z_Resolution_Ratio;
        int U_Resolution_Ratio;

        //反馈脉冲
        int X_Dest_PositionEncode;
        int Y_Dest_PositionEncode;
        int Z_Dest_PositionEncode;
        int U_Dest_PositionEncode;

        //轴的零位信息与下载信息判断
        int XYZU_EL_ORG;
        int PROGRAM_LINE_NUM;//若不为零则下载程序正在进行


        //读取移动距离
        int X_MoveDistance_Read;
        int Y_MoveDistance_Read;
        int Z_MoveDistance_Read;
        int U_MoveDistance_Read;

        //程序行数跳跃
        int PCL400_PROGRAM_LINE_NUM_JP;
        int PMC400_Ver;//版本号

        //IP地址，子网掩码，网关
        int IP;//PCL400_32_temp3;
        int NM;
        int GW;
        int PMC400_Arrival;//闭环到位标志

        //用于CRC和校验
        int PCL400_CRC_SUM;


    }PMC400_status;

    typedef struct PMC400_PID_CFG
    {
    public:
        float kp[4];
        float ki[4];
        float kii[4];
        float kd[4]; //32位 //各轴PID参数 [0-3] =[x,y,z,u]

        float ratioKp[4];
        float ratioKi[4];
        float ratioKd[4]; //32位 //各轴PID参数变化比率, 是个百分比, [0-3] =[x,y,z,u]

        float slewRate[4]; //32位 //积分摆动范围, [0-3] =[x,y,z,u]

        float allowEr[4];//允许误差；脉冲数

        float ratio[4];//倍率；允许误差内，PID系数/这个倍率使用

        signed char kiien[4]; //8位 [0-3] =[x,y,z,u]
        signed char lastkiien[4]; //8位 //[0-3] =[x,y,z,u]

        void Init()
        {
            kp[0]=0;kp[1]=0;kp[2]=0;kp[3]=0;
            ki[0]=0;ki[1]=0;ki[2]=0;ki[3]=0;
            kii[0]=0;kii[1]=0;kii[2]=0;kii[3]=0;
            kd[0]=0;kd[1]=0;kd[2]=0;kd[3]=0;
            ratioKp[0]=0;ratioKp[1]=0;ratioKp[2]=0;ratioKp[3]=0;
            ratioKi[0]=0;ratioKi[1]=0;ratioKi[2]=0;ratioKi[3]=0;
            ratioKd[0]=0;ratioKd[1]=0;ratioKd[2]=0;ratioKd[3]=0;
            slewRate[0]=0;slewRate[1]=0;slewRate[2]=0;slewRate[3]=0;
            allowEr[0]=0;allowEr[1]=0;allowEr[2]=0;allowEr[3]=0;
            ratio[0]=0;ratio[1]=0;ratio[2]=0;ratio[3]=0;
            kiien[0]=0;kiien[1]=0;kiien[2]=0;kiien[3]=0;
            lastkiien[0]=0;lastkiien[1]=0;lastkiien[2]=0;lastkiien[3]=0;
        }
    }PID; //PID参数结构
////////////////////2017.10.26////////////////////////////
    typedef struct PMC400_REALTIMEDATA
    {
        int TimeFromThePrevious;
        int CurentPosition; //当前out脉冲数
        int PositionEncode; //当前编码器脉冲
        int Pulse_Remaining; //剩余脉冲数
        void Init()
        {
            TimeFromThePrevious = 0;
            CurentPosition = 0;
            PositionEncode = 0;
            Pulse_Remaining = 0;
        }
    }REALTIMEDATA;//实时数据
////////////////////2017.10.26////////////////////////////
    //定时器ID初始化
    uint TimerID;
    bool TimerInUse;
    //PID用参数
    static const int length_pid = sizeof(struct PMC400_PID_CFG) +7;//"PIDSET:" +  struct
    unsigned char *p;
    //PMC400的IP地址,端口（固定80）。通信时使用
    const char *pmc400ip;
    int port;
    //连接百分率初始化
    int Connect_Percent_Value;
    //连接信息初始化
    int Is_Connected;
    //socket通信使用
    CClientNet IpConnect;
    //急停，停止初始化
    bool Is_Run_Break;
    //数据结构体声明
    PMC400_status Status , Status_temp;

    //PID参数
    PID PID_CFG;

    REALTIMEDATA realTimeData[50];
    void realTimeInit(int num);//结构体数组批量初始化
    static const int realTime_Len = 50 * sizeof(struct PMC400_REALTIMEDATA) + 8;//"REAL:X" + 数据组数（1字节） +  struct * 数据组数;

    static const int RecvLength = 1024;
    char * byte_CommandReturn;
    int recvlength;//实际接收到数据的长度

    bool pid_on_off[4];//PID使能
    bool FeedToggle[4];//光栅尺方向反转

    int ControlMode[4];//控制方式:0开1闭2手杆3速度

    int ADVAL[4];//手杆模式-阈值
    int SpeedRate[4];//速度倍率
    int HomeDirection[4];//回零方向：0正1负
    bool HomeSearching[4];//正在回零标志
    bool HomeComplete[4];//回零完成标志
    bool HomeWithLimit[4];//带限位回零标志

    int LimitTTL[4];//限位电平
    int MoveMode[4];//移动方式：0直线，1旋转

    bool ShowCommand;//是否显示每次发送的指令，默认不显示
///////////////////////重要函数///////////////////////////////////////
    void Close();//结束控制
    PMC400_status BytesToStruct_Status(char bytes[]);//字节流转结构体1
    PID BytesToStruct_PID(char bytes[]);//字节流转结构体2
    bool BytesToStruct_REALTIMEDATA(char bytes[],int len);//字节流转结构体3
    int ByteToInt(char bytes[]);//字节转整型
    float ByteToFloat(char bytes[]);//字节转浮点

////////////////////////////连接相关////////////////////////////////
    int Conncet_Status_Pencent();//返回连接百分率值
    bool Connect(string Str_Ip, int port);
    bool IsConnected();//判断连接状态
    bool ReConnect();//重新连接

////////////////////////////获取参数数据////////////////////////////
    double Get_StartSpeed(int Axis);
    double Get_AccSpeed(int Axis);
    double Get_FinalSpeed(int Axis);
    double Get_CurrentPosition(int Axis);
    int Get_Download_Line();
    int Get_El_Left_Data(int Axis);
    int Get_EL_Org_Data(int Axis);
    int Get_El_Right_Data(int Axis);
    int Get_Org_Data(int Axis);
    int Get_Electric_Current(int Axis);
    int Get_IO_IN();
    int Get_IO_OUT();
    int Get_Limit_TTL(int Axis);
    int Get_Is_Moving(int Axis);
    int Get_MicroSteps(int Axis);
    int Get_PositionEncode(int Axis);
    int Get_Pulse_Remaining(int Axis);
    int Get_Resolution_Ratio(int Axis);
    int Get_ScrewLead(int Axis);
    int Get_SpeedRatio(int Axis);
    PID Get_PID();
    bool Get_PID_ON_OFF(int Axis);//获取PID使能
    bool Get_FeedToggle(int Axis,bool read);//获取光栅尺方向反转 1为反转
    void Get_Position(int Axis,int num);
    int Get_ControlMode(int Axis,bool read);
    int Get_ADVAL(int Axis,bool read);//获取手杆模式-阈值
    int Get_SpeedRate(int Axis,bool read);//获取速度倍率
    int Get_HomeDirection(int Axis,bool read);//获取回零方向
////////////////////////////设置参数数据////////////////////////////
    void Set_AccSpeed(int Axis, double SpeedData);
    void Set_AccSpeed_Convert(int Axis, double SpeedData);
    void Set_CloseMoveDistance(int Axis, double SpeedData);
    void Set_CurrentPosition(int Axis, double Data);
    void Set_CurrentPosition_Convert(int Axis, double Data);
    void Set_DevSN(string sn);
    void Set_Electric_Current(int Axis, int Data);
    void Set_FinalSpeed(int Axis, double SpeedData);
    void Set_FinalSpeed_Convert(int Axis, double SpeedData);
    void Set_IO_OUT(int Data);
    void Set_Limit_TTL(int Axis, int Data);
    void Set_MicroSteps(int Axis, int Data);
    void Set_MoveDistance_Convert(int Axis, double SpeedData);
    void Set_NetAddr_Int(int ipdata, int nmdata, int gwdata);
    void Set_NetAddr_String(string ip, string netmask, string gw);
    void Set_OpenMoveDistance(int Axis, double SpeedData);
    void Set_Resolution_Ratio(int Axis, int Data);
    void Set_ScrewLead(int Axis, int Data);
    void Set_SpeedRatio(int Axis, int Data);
    void Set_StartSpeed(int Axis, double SpeedData);
    void Set_StartSpeed_Convert(int Axis, double SpeedData);
    void Set_PID(PID pid);
    void Set_PID_ON_OFF(int Axis, int Data);//设置PID使能
    void Set_FeedToggle(int Axis, int Data);//设置光栅尺方向反转 1为反转
    void Set_ControlMode(int Axis,int Mode);
    void Set_ADVAL(int Axis,int Value);//设置手杆模式-阈值
    void Set_SpeedRate(int Axis,int rate);//设置速度倍率
    void Set_HomeDirection(int Axis,int Direction);//设置回零方向

///////////////////////////其他指令//////////////////////////////////////
    static void PASCAL CallBackFunc(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);//定时器回调函数1
    void PASCAL CallBackFunc2(UINT wTimerID, UINT msg,DWORD dwl,DWORD dw2);//定时器回调函数2
    void TimerStart();//定时器开始
    void TimerStop();//定时器停止
    bool Connect_Tock();//定时器相关（校验）

    int HomeBack(int Axis);//轴回零
    int HomeBackCheck(int Axis);//轴回零检查
    bool HomeBack_SendCommand(int Axis);//发送回零指令
    void Command_Start();//执行所有指令
    void StartMove();//执行运动指令
    void Stop();//停止（急停）
    void StopMove();//停止执行程序（指令）
    int Get_PMC400_Ver();
    void WaitForStop(int Axis);

    void Save_PID();
    void Save_Setup();

    void Save_ControlMode();//保存控制方式
    void Save_ADVAL();//保存手杆阈值
    void Save_SpeedRate();//保存速度倍率
    void Save_HomeDirection();//保存回零方向

    int AxisIndex(int Axis);
    int Axisname(int axis);
///////////////////////////发送命令/////////////////////////////////////////
    bool Send_DownLoadString(string Str_Command);//发送下载指令
    bool Send_PIDString(string Str_Command);//发送PID指令(主)
    ////////////////////2017.10.26////////////////////////////
    bool Send_GetPosition(string Str_Command);//发送获取实时位置指令
    ////////////////////2017.10.26////////////////////////////
    bool Command(string Str_Command);//发送指令（主）
    bool SendCommand(string Str_Data);//发送指令（次）及检验接收数据

    bool Manual_SendCommand(string Str_Data);//发送指令（自由发送）
    int IsCommand(string Str_Data);
    bool ProcessCommands(string Str_Send, string Str_Receive);
///////////////////////////其他/////////////////////////////////////////
    std::string to_string(double d_data);
    std::string to_string(int i_data);

};

#endif // PMC400_H
