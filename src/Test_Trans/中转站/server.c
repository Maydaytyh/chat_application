/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-02 21:24:07 +0800
 * @LastEditTime: 2019-09-03 10:52:49 +0800
 * @LastEditors: 
 * @Description: 
 */

#include "actions.h"
#include "head.h"
//***********************全局变量区************************//
sockaddr_int Server_addr, Client_addr;
int Fd_server, Fd_client;
void Send_File_To_Client(int Fd_of_Rec)
{
    char Buf[Size], Filename[Size];
    bzero(Buf, Size);
    receiveMSG(Fd_of_Rec, Buf, Size, 0);
    bzero(Filename, Size);
    strncpy(Filename, Buf, strlen(Buf) > Size ? Size : strlen(Buf));
    //打开文件
   FILE* fp = fopen(Filename, "r");
    if (NULL == fp)
    {
        printf("Open %s Failed\n", Filename);
    }
    else
    {
        bzero(Buf, Size);
        int Len=0;
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
    char file_name[Size], buffer[Size];
    bzero(file_name, Size);
    bzero(buffer, Size);
    if (recv(Fd_of_Send, file_name, Size, 0) < 0)
    {
        perror("Server Recieve Data Failed:");
        return;
    }
    FILE *fp = fopen(file_name, "w");
    if (NULL == fp)
    {
        printf("File:\t%s Can Not Open To Write\n", file_name);
        exit(1);
    }
    // 每接收一段数据，便将其写入文件中，循环直到文件接收完并写完为止
    bzero(buffer, Size);
    int length = 0;
    int flag = 1;
    while ((length = recv(Fd_of_Send, buffer, Size, 0)) > 0)
    {
        if (fwrite(buffer, sizeof(char), length, fp) < length)
        {
            printf("File:\t%s Write Failed\n", file_name);
            flag = 0;
            break;
        }
        bzero(buffer, Size);
    }
    if (flag)
        // 接收成功后，关闭文件，关闭socket
        printf("Receive File:\t%s From Server IP Successful!\n", file_name);
    close(fp);
}
int main()
{
    puts("***************************Server is running******************");
    //创建Socket
  /*  Fd_server = createSocket(AF_INET, SOCK_STREAM);
    initialzeSocketaddr(&Server_addr, NULL, Port);
    int opt;
    setsockopt(Fd_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    bindSocketAddr(Fd_server, (struct sockaddr *)&Server_addr, sizeof(Server_addr));
    createListen(Fd_server, 10);*/
    struct sockaddr_in Server_addr;   
  bzero(&Server_addr, sizeof(Server_addr));   
  Server_addr.sin_family = AF_INET;   
  Server_addr.sin_addr.s_addr = htons(INADDR_ANY);   
  Server_addr.sin_port = htons(Port);   
    
  // 创建socket，若成功，返回socket描述符   
  int Fd_server = socket(PF_INET, SOCK_STREAM, 0);   
  if(Fd_server< 0)   
  {   
    perror("Create Socket Failed:");   
    exit(1);   
  }   
  int opt = 1;   
  setsockopt(Fd_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));   
    
  // 绑定socket和socket地址结构   
  if(-1 == (bind(Fd_server, (struct sockaddr*)&Server_addr, sizeof(Server_addr))))   
  {   
    perror("Server Bind Failed:");   
    exit(1);   
  }   
      
  // socket监听   
  if(-1 == (listen(Fd_server, 10)))   
  {   
    perror("Server Listen Failed:");   
    exit(1);   
  }   
    while (1)
    {
        // Fd_client = acceptConnection(Fd_server, (struct sockaddr *)&Client_addr, sizeof(Client_addr));
        // //Send_File_To_Client(Fd_client);
        // Rec_File_From_Client(Fd_client);
        // close(Fd_client);

        // 定义客户端的socket地址结构
        struct sockaddr_in client_addr;
        socklen_t client_addr_length = sizeof(client_addr);

        // 接受连接请求，返回一个新的socket(描述符)，这个新socket用于同连接的客户端通信
        // accept函数会把连接到的客户端信息写到client_addr中
        int new_server_socket_fd = accept(Fd_server, (struct sockaddr *)&client_addr, &client_addr_length);
        if (new_server_socket_fd < 0)
        {
            perror("Server Accept Failed:");
            break;
        }
        char file_name[Size ];
        bzero(file_name, Size);
        if (recv(new_server_socket_fd, file_name, Size, 0) < 0)
        {
            perror("Server Recieve Data Failed:");
            break;
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
        while ((length = recv(new_server_socket_fd, buffer,Size, 0)) > 0)
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
        close(fp);
        // 关闭与客户端的连接
        close(new_server_socket_fd);
    }
    close(Fd_server);
    return 0;
}