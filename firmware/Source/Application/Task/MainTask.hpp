#ifndef MAIN_TASK_HPP
#define MAIN_TASK_HPP

#include "TaskTemplate.hpp"
#include "TaskConfig.h"
#include "TaskList.h"
#include "MessageQueue.hpp"
#include "MessageQueueType.h"
#include "DebugQueue.hpp"
#include "Odometory.hpp"
#include "Button.hpp"
#include "Blink.hpp"
#include "TrajControlTask.hpp"
#include "VelControlTask.hpp"
#include "WallDetectTask.hpp"

class MainTask : public TaskTemplate<MainTask_OsFunc_t>{
    public:
        bool Initialize(const MainTask_OsFunc_t *pOsFunc);
        void Update();

        static MainTask& GetInstance(){
            static MainTask self;
            return self;
        }

        MessageQueue<PosControlCmdMsg_t>& GetPosCmdMsgQueueInst(){
            return PosCmdMsgQueue;
        }

    private:
        typedef enum{
            EN_MODE_FIRST = 0,
            EN_MODE_FREE = 0,
            EN_MODE_ENKAI,
            EN_MODE_ANG_SIN,
            EN_MODE_LAST,
        }EN_MODE;

        MainTask();
        ~MainTask(){;}
        void InputUpdate();
        void OutputUpdate();
        
        void SetLedMode(EN_MODE enMode, EN_TOP_STATE enState);
        void SetTopState(EN_TOP_STATE enState);

        static DebugQueue& rDebugQueue;
        static MessageQueue<OdometoryMsg_t> &rPosMsgQueue;
        static TrajControlTask& rTrajTask;
        static VelControlTask& rVelTask;
        static WallDetectTask& rWallTask;

        //内部クラス
        MessageQueue<PosControlCmdMsg_t> PosCmdMsgQueue;
        Button ModeSelectSW;
        Button StartSW;
        Blink TickLed;
        Blink DbgLed[3];

        //メンバ変数
        EN_MODE enNowMode;
        EN_TOP_STATE enNowState;
        EN_TOP_STATE enOldState;
};


#endif /* MAIN_TASK_HPP */
