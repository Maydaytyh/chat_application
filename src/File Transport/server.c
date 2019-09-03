/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-02 09:34:47 +0800
 * @LastEditTime: 2019-09-02 16:41:15 +0800
 * @LastEditors: 
 * @Description: 
 */
#include "actions.h"
#include "head.h"
//***********************全局变量区************************//
sockaddr_int Server_addr, Client_addr;
int Fd_server, Fd_client;
char Buf[Size], Filename[Size];
FILE *fp;
int Len;
/**
 * @Author: Tian Yuhang 5t/
 * @Description: 发送服务器文件到客户端
 * @Param: Fd_of_rec 接受文件的fd
 * @Return: 无
 * @Throw: 
 */
void Send_File_To_Client(int Fd_of_Rec)
{
    bzero(Buf, Size);
    receiveMSG(Fd_of_Rec, Buf, Size, 0);
    bzero(Filename, Size);
    strncpy(Filename, Buf, strlen(Buf) > Size ? Size : strlen(Buf));
    //printf("Filename:%s\n",Filename);
    //打开文件
    fp = fopen(Filename, "r");
    if (NULL == fp)
    {
        printf("Open %s Failed\n", Filename);
    }
    else
    {
        bzero(Buf, Size);
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
void Rec_File_From_Client(int Fd_of_Send)
{
    char file_name[Size];
    bzero(file_name, Size);
    if (recv(Fd_of_Send, file_name, Size, 0) < 0)
    {
        perror("Server Recieve Data Failed:");
        return;
    }
    //scanf("%s",file_name);
    char buffer[Size];
    bzero(buffer, Size);
    FILE *fp = fopen(file_name, "w");
    if (NULL == fp)
    {
        printf("File:\t%s Can Not Open To Write\n", file_name);
        exit(1);
    }

    // 从服务器接收数据到buffer中
    // 每接收一段数据，便将其写入文件中，循环直到文件接收完并写完为止
    bzero(buffer, Size);
    int length = 0;
    while ((length = recv(Fd_of_Send, buffer, Size, 0)) > 0)
    {
        if (fwrite(buffer, sizeof(char), length, fp) < length)
        {
            printf("File:\t%s Write Failed\n", file_name);
            break;
        }
        bzero(buffer, Size);
    }

    // 接收成功后，关闭文件，关闭socket
    printf("Receive File:\t%s From Server IP Successful!\n", file_name);
}
int main()
{
    puts("***************************Server is running******************");
    //创建Socket
    Fd_server = createSocket(AF_INET, SOCK_STREAM);

    initialzeSocketaddr(&Server_addr, NULL, Port);
    //设置端口可重用
    int opt;
    setsockopt(Fd_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    bindSocketAddr(Fd_server, (struct sockaddr *)&Server_addr, sizeof(Server_addr));
    createListen(Fd_server, 10);
    while (1)
    {
        Fd_client = acceptConnection(Fd_server, (struct sockaddr *)&Client_addr, sizeof(Client_addr));
        bzero(Buf, Size);
        receiveMSG(Fd_client, Buf, Size, 0);
        
       /* if (strcmp(Buf, "get") == 0)
        {
            Send_File_To_Client(Fd_client);
        }
        else if(strcmp(Buf,"send")==0)
        {
            Rec_File_From_Client(Fd_client);
        }*/
        close(Fd_client);
    }
    close(Fd_server);
    return 0;
}