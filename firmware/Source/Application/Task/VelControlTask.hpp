#ifndef VEL_CONTROL_TASK_HPP
#define VEL_CONTROL_TASK_HPP

#include "TaskTemplate.hpp"
#include "TaskList.h"
#include "TaskConfig.h"
#include "MessageQueue.hpp"
#include "MessageQueueType.h"
#include "Odometory.hpp"
#include "PidController.hpp"
#include "Blink.hpp"
#include "DCMotor.hpp"
#include "DebugQueue.hpp"
#include "BatteryMonitor.hpp"

class VelControlTask : public TaskTemplate<VelControlTask_OsFunc_t>{
    public:
        bool Initialize(const VelControlTask_OsFunc_t *pOsFunc);
        void Update();
        bool IsCalibCompleted(){
            return bCalibCompleted;
        }
        void StartCalib(){
            if(!bStartCalibFlag){
                bStartCalibFlag = true;
                bCalibCompleted = false;
            }
        }
        void SetTopState(EN_TOP_STATE enState){
            enNowTopState = enState;
        }

        static VelControlTask& GetInstance(){
            static VelControlTask self;
            return self;
        }

        MessageQueue<OdometoryMsg_t>& GetVelMsgQueueInst(){
            return VelMsgQueue;
        }

    private:
        //メンバ関数
        VelControlTask();
        ~VelControlTask(){;}
        void InputUpdate();     //入力系のアップデート
        void OutputUpdate();    //出力系のアップデート

        //内在するクラスインスタンスの参照
        static DCMotor& rLeftMotor;
        static DCMotor& rRightMotor;
        static Odometory& rOdometory;
        static DebugQueue&  rDebugQueue;
        static BatteryMonitor &rBattMoni;
        
        //内部クラス
        PidController WheelVelPidCon[DCMotor::EN_MOTOR_LAST];
        static MessageQueue<OdometoryMsg_t> VelMsgQueue;

        //メンバ変数
        bool bCalibCompleted;
        bool bStartCalibFlag;
        EN_TOP_STATE enNowTopState;

};

#endif /* VEL_CONTROL_TASK_HPP */
