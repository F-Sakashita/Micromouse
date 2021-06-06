#ifndef MESSAGE_QUEUE_HPP
#define MESSAGE_QUEUE_HPP

#include "cmsis_os.h"
#include <cstdint>

template<typename T>
class MessageQueue{
    private:
        MessageQueue(const MessageQueue &other);
        osMessageQueueId_t QueueId;
        bool bInitialized;

    public:
        MessageQueue(){
            QueueId = NULL;
            bInitialized = false;
        }
        ~MessageQueue(){;}

        //初期化
        bool Initialize(const osMessageQueueId_t QueueId){
            if(bInitialized){
                //初期化済ならTrueをリターン
                return true;
            }
            if(NULL == QueueId){
                return false;
            }
            this->QueueId = static_cast<osMessageQueueId_t>(QueueId);
            bInitialized = true;
            return true;
        }
        //初期化完了
        bool IsInitialized(){
            return bInitialized;
        }
        //キューフルチェック
        bool IsFull(){
            if(0u == osMessageQueueGetSpace(QueueId)){
                return true;
            }
            return false;
        }
        //キューエンプティチェック
        bool IsEmpty(){
            if(0u == osMessageQueueGetCount(QueueId)){
                return true;
            }
            return false;
        }
        //プッシュ
        bool Push(const T* pWriteData, uint32_t uiTimeout){
            if(!bInitialized || NULL == pWriteData){
                return false;
            }
            //CMSIS V2では第2引数のMessage Priorityが内部で無視される
            osStatus status = osMessageQueuePut(QueueId, static_cast<const void*>(pWriteData), 0u, uiTimeout);
            if(osOK != status){
                return false;
            }
            return true;
        }
        //ポップ
        bool Pop(T *pReadData, uint32_t uiTimeout){
            if(!bInitialized || NULL == pReadData){
                return false;
            }
            //CMSIS V2では第2引数のMessage Priorityポインタが内部で無視される
            osStatus status = osMessageQueueGet(QueueId, static_cast<void *>(pReadData), NULL, uiTimeout);
            if(osOK != status){
                return false;
            }
            return true;
        }
        //保管データ数
        uint32_t IsStoredDataNum(){
            return osMessageQueueGetCount(QueueId);
        }
};


#endif  //MESSAGE_QUEUE_HPP
