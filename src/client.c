/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-04 15:48:05 +0800
 * @LastEditTime: 2019-09-04 22:05:50 +0800
 * @LastEditors: 
 * @Description: 
 */
#include "head.h"
#include "actions.h"
#include "Queue.h"
int main()
{
     int Msg_Id=-1;
    Send_Req  data;
    char buffer[102400];
    Msg_Id=msgget((key_t)6666,0666|IPC_CREAT);
    if(Msg_Id==-1)
    {
        perror("Msgget Failed:");
        exit(1);
    }
    while(1)
    {
        if(msgrcv(Msg_Id,(void *)&data,1024,0,0)==-1)
        {
            perror("Receive Message Error");
            exit(1);
        }
        if(strncmp(data.Message,"exit",4)==0)
        {
            break;
        }
        printf("You receiced:%s\n",data.Message);
        // printf("")

    }
    if(msgctl(Msg_Id,IPC_RMID,0)==-1)
    {
        perror("Delete The Queue Error");
    }
    exit(0);
}