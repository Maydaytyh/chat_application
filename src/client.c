/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-08-31 21:14:51 +0800
 * @LastEditTime: 2019-09-01 10:50:50 +0800
 * @LastEditors: 
 * @Description: 
 */
#include "head.h"
//***************************全局变量**************************************
char Buf[Size];
char Rec_Buf[Size];
struct sockaddr_in Server_Addr;
int Fd_Server;
int Len_Server;
int Num_Of_Bytes;
char Name[1024];
//*************************************************************
void Pthread_Recv(void *Ptr)
{
    while(1)
    {
        if(recv(Fd_Server,Rec_Buf,Size,0)==-1)
        {
            puts("***************************Receive Error!*************************");
            exit(1);
        }
        printf("%s",Rec_Buf);
        memset(Rec_Buf,0,sizeof Rec_Buf);
    }
}
int main()
{
   /* if((Fd_Server=socket(AF_INET,SOCK_STREAM,0)==-1))
    {
       puts("***************************Socket Error!*************************");
        exit(1);
    }
    bzero(&Server_Addr,sizeof(Server_Addr));
    //初始化
    Server_Addr.sin_family=AF_INET;
    Server_Addr.sin_port=htons(Port);
    Server_Addr.sin_addr.s_addr=INADDR_ANY;
    if(connect(Fd_Server,(struct sockaddr *)&Server_Addr,sizeof(struct sockaddr))==-1)
    {
         puts("***************************Connect Error!*************************");
         exit(1);
    }*/
    if ((Fd_Server=socket(AF_INET, SOCK_STREAM, 0))==-1){ 
    printf("socket() error\n"); 
    exit(1); 
    } 

    bzero(&Server_Addr,sizeof(Server_Addr));

    Server_Addr.sin_family = AF_INET; 
    Server_Addr.sin_port = htons(Port); 
    //server.sin_addr = *((struct in_addr *)he->h_addr); 
    Server_Addr.sin_addr.s_addr = INADDR_ANY;
    if(connect(Fd_Server, (struct sockaddr *)&Server_Addr,sizeof(struct sockaddr))==-1){  
    printf("connect() error\n"); 
    exit(1); 
    } 

    puts("***************************Connect Success!*************************");
    char Print[]="已加入聊天\n";
    printf("请输入用户名：");
    fgets(Name,sizeof(Name),stdin);
    send(Fd_Server,Name,(strlen(Name)-1),0);
    send(Fd_Server,Print,strlen(Print),0);
    pthread_t pid;
    pthread_create(&pid,NULL,Pthread_Recv,NULL);
    while(1)
    {
         memset(Buf,0,sizeof(Buf));
         //printf("%s:",Name);
         fgets(Buf,sizeof(Buf),stdin);
         if(strcmp(Buf,"exit")==0)
         {
             bzero(Buf,sizeof(Buf));
             printf("********************您已经退出群聊！*******************\n");
             break;
         }
        // Name[strlen(Name)-1]=':';
         send(Fd_Server,Name,strlen(Name)-1,0);
         send(Fd_Server,":",1,0);
         send(Fd_Server,Buf,strlen(Buf),0);
    }
    close(Fd_Server);
}
