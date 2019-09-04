/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-03 23:26:20 +0800
 * @LastEditTime: 2019-09-05 00:51:57 +0800
 * @LastEditors: 
 * @Description: 
 */
#include "head.h"
#include"actions.h"
/**
 * @Author: Tian Yuhang
 * @Description: 用于服务器从客户端接受文件
 * @Param: Fd_of_Send 表示要发送给的对象的Fd
 * @Return: 
 * @Throw: 
 */
#include <netinet/in.h> // sockaddr_in
#include <sys/types.h>  // socket
#include <sys/socket.h> // socket
#include <stdio.h>		// printf
#include <stdlib.h>		// exit
#include <string.h>		// bzero
#include "head.h"
#include<time.h>
/**
 * @Author: Tian Yuhang
 * @Description: 用于服务器从客户端接受文件
 * @Param: Fd_of_Send 表示要发送给的对象的Fd
 * @Return: 无
 * @Throw: 
 */
void Rec_File_From_Client(int Fd_of_Send)
{
	char file_name[Size], buffer[Size];
	bzero(file_name, Size);
	bzero(buffer, Size);
	if (read(Fd_of_Send, file_name, Size) < 0)
	{
		perror("Server Recieve Data Failed:");
		return;
	}
	int len = strlen(file_name);
	int pos = len - 1;
	char file_name_final[Size];
	bzero(file_name_final, Size);
	while (pos >= 0 && file_name[pos] != '/')
	{
		pos--;
	}
	if (pos >= 0)
	{
		for (int i = pos + 1; i < len; ++i)
			file_name_final[i - pos - 1] = file_name[i];
	}
	else
		strcpy(file_name_final, file_name);
	time_t t;
	t=time(NULL);
	int tt=time(&t);
	char str[Size];
	// itoa(tt,str,10);
	sprintf(str,"%d",tt);
	int pos1=0;
	for(int i=0;i<strlen(file_name_final);++i)
	{
		if(file_name_final[i]=='.')
		{
			pos1=i;break;
		}
	}
	char sss[Size];
	for(int i=0;i<pos1;++i)
	 sss[i]=file_name_final[i];
	strcat(sss,str);
	char ssss[Size];
	for(int i=pos1;i<strlen(file_name_final);++i)
	  ssss[i-pos1]=file_name_final[i];
	strcat(sss,ssss);
	strcpy(file_name_final,sss);
	FILE *fp = fopen(file_name_final, "w");
	if (NULL == fp)
	{
		printf("File:\t%s Can Not Open To Write,Please check the path\n", file_name_final);
	}
	else
	{
		// 每接收一段数据，便将其写入文件中，循环直到文件接收完并写完为止
		bzero(buffer, Size);
		int length = 0;
		int flag = 1;
		while ((length = recv(Fd_of_Send, buffer, Size, 0)) > 0)
		{
			if (fwrite(buffer, sizeof(char), length, fp) < length)
			{
				printf("File:\t%s Write Failed\n", file_name_final);
				break;
			}
			bzero(buffer, Size);
		}
		// 接收成功后，关闭文件，关闭socket
		printf("Receive File:\t%s From Client Successful!\n", file_name_final);
		close(fp);
	}
}
/**
 * @Author: Tian Yuhang
 * @Description: 该函数实现的是从客户端像服务器发送文件
 * @Param: Client_Socket_Fd ,发送的客户端的文件描述符。
 * @Return: 无
 * @Throw: 
 */
void Send_File_To_Server(int Client_Socket_Fd)
{
    char file_name[Size],buffer[Size];
	char file[Size];
    //scanf("%s",File_name);
	printf("Please Input The File:");
	fgets(file,Size,stdin);
	int ll=strlen(file);
	for(int i=0;i<ll-1;++i)
	  file_name[i]=file[i];
	file_name[ll-1]='\0';
    if(send(Client_Socket_Fd,file_name,Size,0)<0)
    {
        perror("Send File Name Failed:");
        exit(1);
    }
	
    FILE *fp=fopen(file_name,"r");
    if(NULL==fp)
    {
        printf("Open %s Failed! Please Check Your Path!\n",file_name);
    }
    else
    {
        bzero(buffer,Size);
        int len=0;
        int flag=1;
        while((len=fread(buffer,sizeof(char),Size,fp))>0)
        {
            printf("Len=%d\n",len);
            if(send(Client_Socket_Fd,buffer,len,0)<0)
            {
                printf("Send %s Failed!Please Try Again!\n",file_name);
                flag=0;
                break;
            }
            bzero(buffer,Size);
        }
        if(flag)
        {
            printf("Send %s Successfully!\n",file_name);
        }
        fclose(fp);
    }
}

/**
 * @Author: Tian Yuhang
 * @Description: 向客户端发送文件
 * @Param: Fd_of_Rec  发送的客户端口 Name 文件在服务器上的位置
 * @Return: 
 * @Throw: 
 */
void Send_File_To_Client(int Fd_of_Rec,char Name[])
{
    char Buf[Size], Filename[Size];
    bzero(Buf, Size);
	//获取客户端ID
    if(recv(Fd_of_Rec, Buf, Size, 0)<0)
	{
		perror("Get The Id Error:");
		exit(1);
	}
    bzero(Filename, Size);
	//先处理文件路径为文件纯名字
	char ss[Size];
	int pos=strlen(Name)-1;
	while(pos>=0&&Name[pos]!='/') pos--;
	if(pos>0)
	{
		for(int i=pos+1;i<strlen(Name);++i)
		   ss[i-pos-1]=Name[i];
	}
	else strcpy(ss,Name);
	//生成存放在客户端的路径
	strcpy(Filename,"/home/");
	strcat(Filename,Buf);
	strcat(Filename,"/Desktop/");
	strcat(Filename,Name);
	printf("%s\n",Filename);
    //打开文件
	if(send(Fd_of_Rec,Filename,Size,0)<0)
	{
		perror("Send The Name Failed:");
		exit(0);
	}
    FILE *fp = fopen(Name, "r");
    if (NULL == fp)
    {
        printf("Open %s Failed Please Check The Path!\n", Filename);
    }
    else
    {
        bzero(Buf, Size);
        int Len = 0;
        while ((Len = fread(Buf, sizeof(char), Size, fp)) > 0)
        {
            if (send(Fd_of_Rec, Buf, Len, 0) < 0)
            {
                printf("Send File %s Failed!\n", Filename);
                break;
            }
        }
        bzero(Buf, Size);
        fclose(fp);
        printf("File %s Transfered Successfully!\n", Filename);
    }
}
/**
 * @Author: Tian Yuhang
 * @Description: 该函数实现的是从客户端像服务器发送文件
 * @Param: Client_Socket_Fd ,发送的客户端的文件描述符。
 * @Return: 无
 * @Throw: 
 */
void Send_File_To_Server(int Client_Socket_Fd)
{
	char file_name[Size], buffer[Size];
	char file[Size];
	//scanf("%s",File_name);
	printf("Please Input The File:");
	fgets(file, Size, stdin);
	int ll = strlen(file);
	for (int i = 0; i < ll - 1; ++i)
		file_name[i] = file[i];
	file_name[ll - 1] = '\0';
	if (send(Client_Socket_Fd, file_name, Size, 0) < 0)
	{
		perror("Send File Name Failed:");
		exit(1);
	}

	FILE *fp = fopen(file_name, "r");
	if (NULL == fp)
	{
		printf("Open %s Failed! Please Check Your Path!\n", file_name);
	}
	else
	{
		bzero(buffer, Size);
		int len = 0;
		int flag = 1;
		while ((len = fread(buffer, sizeof(char), Size, fp)) > 0)
		{
			printf("Len=%d\n", len);
			if (send(Client_Socket_Fd, buffer, len, 0) < 0)
			{
				printf("Send %s Failed!Please Try Again!\n", file_name);
				flag = 0;
				break;
			}
			bzero(buffer, Size);
		}
		if (flag)
		{
			printf("Send %s Successfully!\n", file_name);
		}
		fclose(fp);
	}
}