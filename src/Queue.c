/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-04 22:04:19 +0800
 * @LastEditTime: 2019-09-04 23:08:44 +0800
 * @LastEditors: 
 * @Description: 
 */
#include "Queue.h"
#include "head.h"
/**
 * @Author: Tian Yuhang
 * @Description: 初始化消息队列
 * @Param: 无
 * @Return: 
 * @Throw: 
 */
void QueueInit()
{
    MsgId = msgget((key_t)6666, 0666 | IPC_CREAT);
    if (MsgId == -1)
    {
        perror("Create Queue Error");
        exit(1);
    }
}
/**
 * @Author: Tian Yuhang
 * @Description: 将消息封装并放到消息队列中
 * @Param: char Msg[],表示消息字符串
 * @Return: 无
 * @Throw: 
 */
void PushMessage(char Msg[])
{
    Send_Req MSG;
    strcpy(MSG.Message, Msg);
    MSG.Type = 1;
    pthread_mutex_lock(&Lock_Of_Queue);
    if (msgsnd(MsgId, (void *)&MSG, MsgSize, 0) == -1)
    {
        perror("Push Message Failed:");
    }
    pthread_mutex_unlock(&Lock_Of_Queue);
}
/**
 * @Author: Tian Yuhang
 * @Description: 客户端从消息队列获取消息
 * @Param: char Msg[]，需要在客户端提前申请好空间，在函数中将获取到的消息的文本复制给Msg
 * @Return: 
 * @Throw: 
 */
void GetMessage(char Msg[])
{
    Send_Req MSG;
    if (msgrcv(MsgId, (void *)&MSG, Size, 0, 0) == -1) //此处第四个参数用Id(>0)则选择描述符为Id的消息，为0则选择第一条消息。
    {
        perror("Get Message Error");
    }
    strcpy(Msg, MSG.Message);
}
/**
 * @Author: Tian Yuhang
 * @Description: 销毁消息队列
 * @Param: 消息队列的Id
 * @Return: 
 * @Throw: 
 */
void DestroyQueue(int Id)
{
    if (msgctl(Id, IPC_RMID, 0) == -1)
    {
        perror("Delete The Queue Error");
    }
}