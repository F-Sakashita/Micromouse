#ifndef DEBUG_QUEUE_HPP
#define DEBUG_QUEUE_HPP

#include "MessageQueue.hpp"
#include "MessageQueueType.h"

//MessageQueueクラスを継承
class DebugQueue : public MessageQueue<DebugMsg_t>{
    private:
        DebugQueue(){}
        ~DebugQueue(){;}

        DebugQueue(const DebugQueue &other);
    public:
        static DebugQueue &GetInstance(){
            static DebugQueue self;
            return self;
        }
        bool Printf(uint32_t uiTimeout, const char *pFormat, ...);
        
};

#endif //DEBUG_QUEUE_HPP