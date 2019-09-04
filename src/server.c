/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-04 15:32:35 +0800
 * @LastEditTime: 2019-09-04 16:45:00 +0800
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
       /* printf("Who Do You Want To Send?");
        fgets(buffer,Size,stdin);
        strcpy(data.UserName,buffer);*/
        printf("Plese Input What You Want Send:");
        fgets(buffer,Size,stdin);
        strcpy(data.Message,buffer);
        data.Type=1;
        if(msgsnd(Msg_Id,(void *)&data,1024,0)==-1)
        {
                perror("Send Message Failed:");
                exit(1);
        }
        if(strncmp(buffer,"exit",4)==0)
        {
            break;
        }
    }
    exit(0);
}
