/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-02 09:34:47 +0800
 * @LastEditTime: 2019-09-02 11:31:45 +0800
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
int main()
{
    puts("***************************Server is running******************");
    //创建Socket
    Fd_server=createSocket(AF_INET,SOCK_STREAM);

    initialzeSocketaddr(&Server_addr,NULL,Port);
    //设置端口可重用
    int opt;
    setsockopt(Fd_server,SOL_SOCKET, SO_REUSEADDR,&opt,sizeof(int));
    bindSocketAddr(Fd_server,(struct sockaddr*)&Server_addr,sizeof(Server_addr));
    createListen(Fd_server,10);
    while(1)
    {
        Fd_client=acceptConnection(Fd_server,(struct sockaddr*)&Client_addr,sizeof(Client_addr));
        bzero(Buf,Size);
        receiveMSG(Fd_client,Buf,Size,0);
        bzero(Filename,Size);
        strncpy(Filename,Buf,strlen(Buf)>Size?Size:strlen(Buf));
        printf("Filename:%s\n",Filename);
        //打开文件
        fp=fopen(Filename,"r");
        if(NULL==fp)
        {
            printf("Open %s Failed\n",Filename);
        }
        else
        {
            bzero(Buf,Size);
            while((Len=fread(Buf,sizeof(char),Size,fp))>0)
            {
                if(send(Fd_client,Buf,Len,0)<0)
                {
                    printf("Send File %s Failed!\n",Filename);
                    break;
                }
            } 
            bzero(Buf,Size);  
            fclose(fp);
            printf("File %s Transfered Successfully!\n",Filename);
        }
        close(Fd_client);
    }
    close(Fd_server);
    return 0;
}