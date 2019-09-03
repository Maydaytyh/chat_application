/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-02 09:35:08 +0800
 * @LastEditTime: 2019-09-02 21:23:17 +0800
 * @LastEditors: 
 * @Description: 
 */
#include "actions.h"
#include"head.h"
//***********************全局变量区************************//
sockaddr_int Server_addr,Client_addr;
int Fd_server,Fd_client;
FILE *fp;
int Len;
int flag=1;
/**
 * @Author: Tian Yuhang
 * @Description: 从服务器接受文件
 * @Param: 服务器的Fd
 * @Return: 
 * @Throw: 
 */
void Rec_File(void *Ptr)
{
    char Filename[Size],Buf[Size];
    while(1){
    bzero(Filename,Size);
    bzero(Buf,Size);
    receiveMSG(Fd_client,Filename,Size,0);
    fp=fopen(Filename,"w");
    int len;
    if (NULL == fp)
    {
        printf("File:\t%s Can Not Open To Write\n", Filename);
        exit(1);
    }
    while(receiveMSG(Fd_client,Buf,Size,0))
    {
         len=strlen(Buf);
         bzero(Buf,Size);
        if(fwrite(Buf,sizeof(char),len,fp)<len)
        {
            printf("Receive %s Failed\n",Filename);
            flag=0;
            exit(0);
        }
        bzero(Buf,Size);
    }
    if(flag)
    {
        printf("Receive %s Successfully!\n",Filename);
    }
    fclose(fp);
    }
}
void Send_File(int fd)
{
    char File_name[Size],Buf[Size];
    scanf("%s",File_name);
    writeBack(Fd_client,File_name,Size,0);
    FILE *fp=fopen(File_name,"r");
    if(NULL==fp)
    {
        printf("Open %s Failed\n",File_name);
    }
    else
    {
        bzero(Buf,Size);
        int len=0;
        int flag=1;
        while(len=fread(Buf,sizeof(char),Size,fp)>0)
        {
            if(send(Fd_client,Buf,len,0)<0)
            {
                printf("Send %s Failed\n",File_name);
                flag=0;
                break;
            }
        }
        if(flag)
        {
            printf("Send %s Successfully!\n",File_name);
        }
        fclose(fp);
    }
    
}
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
    pthread_t pid;
    pthread_create(&pid,NULL,Rec_File,NULL);
    while(1)    
    {
        //printf("Who do you want to Send File to?");
        
    }
    close(Fd_client);
    return 0;
}