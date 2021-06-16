#ifndef WALL_DETECT_TASK_HPP
#define WALL_DETECT_TASK_HPP

#include "TaskTemplate.hpp"
#include "TaskList.h"
#include "TaskConfig.h"
#include "DebugQueue.hpp"

class WallDetectTask : public TaskTemplate<WallDetectTask_OsFunc_t>{
    public:
        bool Initialize(const WallDetectTask_OsFunc_t *pOsFunc);
        void Update();
        static WallDetectTask& GetInstance(){
            static WallDetectTask self;
            return self;
        }
        void SetTopState(EN_TOP_STATE enState){
            enNowTopState = enState;
        }
    private:
        WallDetectTask();
        ~WallDetectTask(){;}
        void InputUpdate();

        static DebugQueue&  rDebugQueue;

        EN_TOP_STATE enNowTopState;
};

#endif /* WALL_DETECT_TASK_HPP */
