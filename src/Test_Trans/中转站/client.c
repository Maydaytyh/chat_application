/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-02 21:24:14 +0800
 * @LastEditTime: 2019-09-03 11:06:17 +0800
 * @LastEditors: 
 * @Description: 
 */
#include "actions.h"
#include "head.h"
//***********************全局变量区************************//
sockaddr_int Server_addr, Client_addr;
int Fd_server, Fd_client;
FILE *fp;
int Len;
void Rec_File_From_Server()
{
    char Filename[Size],Buf[Size];
    bzero(Filename,Size);
    bzero(Buf,Size);
   // receiveMSG(Fd_client,Filename,Size,0);
    scanf("%s",Filename);
    if(send(Fd_client,Filename,Size,0)<0)
    {
        perror("Send File Name Failed:");
        exit(0);
    }
    fp=fopen(Filename,"w");
    int len;
    if (NULL == fp)
    {
        printf("File:\t%s Can Not Open To Write\n", Filename);
        exit(1);
    }
    bzero(Buf,Size);
    int flag=1;
    while((len=recv(Fd_client,Buf,Size,0))>0)
    {
        // len=strlen(Buf);
        // bzero(Buf,Size);
        if(fwrite(Buf,sizeof(char),len,fp)<len)
        {
            printf("Receive %s Failed\n",Filename);
           flag=0;
            break;
        }
        bzero(Buf,Size);
    }
     if(flag)
     {
        printf("Receive %s Successfully!\n",Filename);
     }
    fclose(fp);
}
void Send_File_To_Server()
{
    char File_name[Size],Buf[Size];
    scanf("%s",File_name);
    //writeBack(Fd_client,File_name,Size,0);
    if(send(Fd_client,File_name,Size,0)<0)
    {
        perror("Send File Name Failed:");
        exit(1);
    }
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
    initialzeSocketaddr(&Server_addr,NULL,Port);
    if(connect(Fd_client,(struct sockaddr*)&Server_addr,sizeof(struct sockaddr_in))<0)
    {
        // perror("Connect Error!")
        puts("Connect Failed!");
        perror("Can't Connect To Server IP:");
        exit(0);
    }
    
    while(1)
    {
      //  puts("please Input The Name Of The File");
        printf("Please Input The Name Of The File:");
        // Rec_File_From_Server();
        Send_File_To_Server();
    }
    close(Fd_client);
    return 0;
}