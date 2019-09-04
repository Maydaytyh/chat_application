/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-04 16:38:56 +0800
 * @LastEditTime: 2019-09-04 22:13:26 +0800
 * @LastEditors: 
 * @Description: 
 */
#include "head.h"
#define MAX_SIZE 512
int Msg_Id=-1;
static pthread_mutex_t queue_lock;
typedef struct Send_Request{
    char Message[10240];
    int User_Id;
}Send_Req;

void Push_Message(Send_Req Msg)
{
    pthread_mutex_lock(&queue_lock);
    if(msgsnd(Msg_Id,(void *)&Msg,512,0)==-1)
    {
           perror("Push Message Failed:");
    }
    pthread_mutex_unlock(&queue_lock);
}
void Get_Message(int Id)
{
    Send_Req Msg;
    while(1)
    {
        if(msgrcv(Msg_Id,(void *)&Msg,512,0,0)==-1)
        {
            //接受消息后的操作
            int Fd;//得到客户端的Fd
            
        }
    }
}
void Queue_Init()
{
    Msg_Id=msgget((key_t)6666,0666|IPC_CREAT);
    if(Msg_Id==-1)
    {
        perror("Create Queue Error");
        exit(1);
    }
}
int main()
{
    Queue_Init();
}

