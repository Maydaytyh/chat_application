/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-04 22:04:19 +0800
 * @LastEditTime: 2019-09-04 22:13:54 +0800
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
    MsgId=msgget((key_t)6666,0666|IPC_CREAT);
    if(MsgId==-1)
    {
        perror("Create Queue Error");
        exit(1);
    }
}
void PushMessage(Send_Req Msg)
{
    pthread_mutex_lock(&Lock_Of_Queue);
    if(msgsnd(MsgId,(void *)&Msg,MsgSize,0)==-1)
    {
           perror("Push Message Failed:");
    }
    pthread_mutex_unlock(&Lock_Of_Queue);
}
void GetMessage(int Id)
{
     Send_Req Msg;
    while(1)
    {
        if(msgrcv(MsgId,(void *)&Msg,Size,Id,0)==-1)//此处第四个参数用Id(>0)则选择描述符为Id的消息，为0则选择第一条消息。
        {
            //接受消息后的操作
            //处理消息，相应的显示操作
        }
    }
}
void DestroyQueue(int Id)
{
     if(msgctl(Id,IPC_RMID,0)==-1)
    {
        perror("Delete The Queue Error");
    }
}