/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-08-30 22:51:46 +0800
 * @LastEditTime: 2019-09-01 14:52:12 +0800
 * @LastEditors: 
 * @Description: 
 */
#include "head.h"
//***************************全局变量**************************************
char Buf[Size];
sockaddr_int Server_Addr,Client_Addr;
int Fd_Server,Fd_Client;
int Len_Server,Len_Client;
int number=0;
int Client[Max]={0};
//*************************************************************
/**
 * @Author: Tian Yuhang
 * @Description: 向当前活跃的客户端派发消息
 * @Param: Fd:发出消息的客户端 Buf 消息内容 size 尺寸
 * @Return: 无
 * @Throw: 
 */
//该函数用于向别的用户发送消息
void Send_Message(int Fd,char *Buf,int size)
{
    for(int i=0;i<Max;++i)
    {
       if(Client[i]&&Client[i]!=Fd){

       send(Client[i],Buf,size,0);
        printf("Send Message To %d Successfully\n",Client[i]);
       }
    }
}
/**
 * @Author: Tian Yuhang
 * @Description: 该函数用于接收来自Pa_fd客户端的消息以及派发
 * @Param: void *Pa_Fd  发送的客户端指针
 * @Return: 无
 * @Throw: 
 */
void *Pthread_Service(void* Pa_Fd)
{
    int Fd=*(int *)Pa_Fd;
    while(1)
    {
        int Num_Of_Bytes;
        Num_Of_Bytes=recv(Fd,Buf,Size,0);
        if(Num_Of_Bytes<=0)
        {
            for(int i=0;i<Max;++i)
            {
                if(Fd==Client[i]) Client[i]=0;
            }
            printf("Num_Of_Bytes=%d\nExit at Fd=%d\n",Num_Of_Bytes,Fd);
            break;
        }
        printf("Receive Message Frome %d\n",Fd);
        Send_Message(Fd,Buf,Num_Of_Bytes);
        bzero(Buf,Size);
    }
    close(Fd);
}
int main()
{
    printf("******************Server is running!***************************\n");
    Fd_Server=socket(AF_INET,SOCK_STREAM,0);//定义套接字
    if(Fd_Server==-1)
    {
         puts("***************************Socket Error!*************************");
        exit(1);
    }
    int opt = SO_REUSEADDR;
    setsockopt(Fd_Server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bzero(&Server_Addr,sizeof(Server_Addr));
    Server_Addr.sin_family=AF_INET;
    Server_Addr.sin_port=htons(Port);
    Server_Addr.sin_addr.s_addr=htonl(INADDR_ANY);
    Len_Server=sizeof(struct sockaddr);
    if(bind(Fd_Server,(struct sockaddr *)&Server_Addr,Len_Server)==-1)
    {
        puts("***************************Bind Error!*************************");
        exit(1);
    }
    if(listen(Fd_Server,1)==-1)
    {
        puts("**************************Listen Error!*************************");
        exit(1);
    }
    puts("****************************Listening!*******************************");
    Len_Client=sizeof(Client_Addr);
    while(1)
    {
        if((Fd_Client=accept(Fd_Server,(struct sockaddr*)&Client_Addr,&Len_Client))==-1)
        {
            puts("**************************Accept Error!*************************");
            exit(1);
        }
        if(number>=Max)
        {
            puts("*******************Over The Limit!****************************");
            close(Fd_Client);
        }
        for(int i=0;i<Max;i++)
        {
            if(!Client[i]) {Client[i]=Fd_Client;break;}
        }
      //  printf("%s log in successfully!\n",inet_ntoa(Client_Addr.sin_addr));//输出ip地址，转换为.数表示
        pthread_t tid;
        pthread_create(&tid,NULL,(void *)Pthread_Service,&Fd_Client);
        number++;
    }
    close(Fd_Server);
}