/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-02 11:51:01 +0800
 * @LastEditTime: 2019-09-03 17:18:48 +0800
 * @LastEditors: 
 * @Description: 
 */
#include<netinet/in.h>  // sockaddr_in   
#include<sys/types.h>  // socket   
#include<sys/socket.h>  // socket   
#include<stdio.h>    // printf   
#include<stdlib.h>    // exit   
#include<string.h>    // bzero   
#include "head.h"
int client_socket_fd;
#define SERVER_PORT 1234  
#define BUFFER_SIZE 1024   
#define FILE_NAME_MAX_SIZE 512   
void Send_File_To_Server()
{
    char File_name[Size],Buf[Size];
    scanf("%s",File_name);
    //writeBack(Fd_client,File_name,Size,0);
    if(send(client_socket_fd,File_name,Size,0)<0)
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
        while((len=fread(Buf,sizeof(char),Size,fp))>0)
        {
            printf("Len=%d\n",len);
            if(send(client_socket_fd,Buf,len,0)<0)
            {
                printf("Send %s Failed\n",File_name);
                flag=0;
                break;
            }
            bzero(Buf,Size);
            //if(len<=0) break;
        }
        if(!len)
        {
            strcpy(Buf,"EOF");
            send(client_socket_fd,Buf,strlen(Buf),0);
        }
       // printf("len=%d\n",len);
        if(flag)
        {
            printf("Send %s Successfully!\n",File_name);
        }
        fclose(fp);
    }
}
void Rec_File_From_Server()
{
    char Filename[Size],Buf[Size];
    bzero(Filename,Size);
    bzero(Buf,Size);
   // receiveMSG(Fd_client,Filename,Size,0);
    scanf("%s",Filename);
    if(send(client_socket_fd,Filename,Size,0)<0)
    {
        perror("Send File Name Failed:");
        exit(0);
    }
   FILE* fp=fopen(Filename,"w");
    int len;
    if (NULL == fp)
    {
        printf("File:\t%s Can Not Open To Write\n", Filename);
        exit(1);
    }
    bzero(Buf,Size);
    int flag=1;
    while((len=recv(client_socket_fd,Buf,Size,0))>0)
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
int main()   
{   
  // 声明并初始化一个客户端的socket地址结构   
  struct sockaddr_in client_addr;   
  bzero(&client_addr, sizeof(client_addr));   
  client_addr.sin_family = AF_INET;   
  client_addr.sin_addr.s_addr = htons(INADDR_ANY);   
  client_addr.sin_port = htons(0);   
    
  // 创建socket，若成功，返回socket描述符   
  client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);   
  if(client_socket_fd < 0)   
  {   
    perror("Create Socket Failed:");   
    exit(1);   
  }   
    
  // 声明一个服务器端的socket地址结构，并用服务器那边的IP地址及端口对其进行初始化，用于后面的连接   
  struct sockaddr_in server_addr;   
  bzero(&server_addr, sizeof(server_addr));   
  server_addr.sin_family = AF_INET;   
  if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) == 0)   
  {   
    perror("Server IP Address Error:");   
    exit(1);   
  }   
  server_addr.sin_port = htons(SERVER_PORT);   
  socklen_t server_addr_length = sizeof(server_addr);   
    
  // 向服务器发起连接，连接成功后client_socket_fd代表了客户端和服务器的一个socket连接   
  if(connect(client_socket_fd, (struct sockaddr*)&server_addr, server_addr_length) < 0)   
  {   
    perror("Can Not Connect To Server IP:");   
    exit(0);   
  }   
  while(1)
  {
    printf("Please Input File Name:\t");   
    Send_File_To_Server();
  }
  close(client_socket_fd);   
  return 0;   
}   

