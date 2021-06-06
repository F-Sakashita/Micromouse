#ifndef MESSAGE_QUEUE_LIST_HPP
#define MESSAGE_QUEUE_LIST_HPP

#include "MessageQueue.hpp"
#include "MessageQueueType.h"

//タスク間で共有するメッセージキューインスタンスのextern宣言
//初期化及びPushするタスクがインスタンスを定義する

//オドメトリによる速度データメッセージ
extern MessageQueue<OdometoryMsg_t> g_VelMsgQueue;

//オドメトリによる位置データメッセージ
extern MessageQueue<OdometoryMsg_t> g_PosMsgQueue;

//速度指令メッセージ
extern MessageQueue<VelControlCmdMsg_t> g_VelCmdMsgQueue;

//位置指令メッセージ
extern MessageQueue<PosControlCmdMsg_t> g_PosCmdMsgQueue;

#endif  //MESSAGE_QUEUE_LIST_HPP