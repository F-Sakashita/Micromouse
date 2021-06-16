#ifndef TRAJ_CONTROL_TASK_HPP
#define TRAJ_CONTROL_TASK_HPP

#include "TaskTemplate.hpp"
#include "TaskList.h"
#include "TaskConfig.h"
#include "MessageQueue.hpp"
#include "MessageQueueType.h"
#include "Odometory.hpp"
#include "PidController.hpp"
#include "DebugQueue.hpp"

class TrajControlTask : public TaskTemplate<TrajControlTask_OsFunc_t>{
    public:
        bool Initialize(const TrajControlTask_OsFunc_t *pOsFunc);
        void Update();
        void ResetPosture(const Posture_t& rPosture){
            osMutexAcquire(pOsFunc->ResetPosMutexId, 0);
            if(!bStartResetPosFlag){
                stRequestResetPosture = rPosture;
                bStartResetPosFlag = true;
                bResetPosCompleted = false;
            }
            osMutexRelease(pOsFunc->ResetPosMutexId);
        }
        bool IsResetPosCompleted(){
            return bResetPosCompleted;
        }
        void SetTopState(EN_TOP_STATE enState){
            enNowTopState = enState;
        }

        static TrajControlTask& GetInstance(){
            static TrajControlTask self;
            return self;
        }
    
        MessageQueue<OdometoryMsg_t>& GetPosMsgQueueInst(){
            return PosMsgQueue;
        }

        MessageQueue<VelControlCmdMsg_t>& GetVelCmdMsgQueueInst(){
            return VelCmdMsgQueue;
        }
        
    private:
        //メンバ関数
        TrajControlTask();
        ~TrajControlTask(){;}
        void InputUpdate();     //入力系のアップデート
        void OutputUpdate();    //出力系のアップデート

        //クラスインスタンスの参照
        static Odometory& rOdometory;
        static DebugQueue&  rDebugQueue;
        static MessageQueue<OdometoryMsg_t>& rVelMsgQueue;
        static MessageQueue<PosControlCmdMsg_t>& rPosCmdMsgQueue;
        
        //内部クラス
        MessageQueue<OdometoryMsg_t> PosMsgQueue;
        MessageQueue<VelControlCmdMsg_t> VelCmdMsgQueue;
        PidController AnglePidCon;

        //メンバ変数
        Posture_t stRequestResetPosture;
        bool bResetPosCompleted;
        bool bStartResetPosFlag;
        EN_TOP_STATE enNowTopState;
        TrajControlTask_OsFunc_t *pOsFunc;

};

#endif /* TRAJ_CONTROL_TASK_HPP */
