/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-02 09:35:08 +0800
 * @LastEditTime: 2019-09-02 11:49:25 +0800
 * @LastEditors: 
 * @Description: 
 */
#include "actions.h"
#include"head.h"
//***********************全局变量区************************//
sockaddr_int Server_addr,Client_addr;
int Fd_server,Fd_client;
char Buf[Size],Filename[Size];
FILE *fp;
int Len;
int flag=1;
int main()
{
    Fd_client=createSocket(AF_INET,SOCK_STREAM);
    initialzeSocketaddr(&Client_addr,NULL,Port);
   // initialzeSocketaddr(&Server_addr,NULL,Port);
    if(connect(Fd_client,(struct sockaddr*)&Server_addr,sizeof(Server_addr))<0)
    {
        puts("Failed To Connect The Server");
        exit(1);
    }
    bzero(Filename,Size);
    printf("Please Input The Name Of The File:");
    //scanf("%s",Filename);
    fgets(Filename,Size,stdin);
    //puts(Filename);
    bzero(Buf,Size);
    strncpy(Buf,Filename,strlen(Filename)>Size?Size:strlen(Filename));
    // puts("????");    
    if(send(Fd_client,Buf,Size,0)<0)
    {
        printf("Send File Failed!\n");
        exit(1);
    }
   // puts("????");
    fp=fopen(Filename,"w");
   // puts("????");
    while(Len=recv(Fd_client,Buf,Size,0)>0)
    {
         bzero(Buf,Size);
        if(fwrite(Buf,sizeof(char),Len,fp)<Len)
        {
            printf("Send %s Failed\n",Filename);
            flag=0;
            exit(0);
        }
        bzero(Buf,Size);
    }
    if(flag)
    {
        printf("Send %s Successfully!\n",Filename);
    }
    close(fp);
    close(Fd_client);
    return 0;
}