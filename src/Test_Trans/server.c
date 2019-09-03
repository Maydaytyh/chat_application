/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-02 12:15:46 +0800
 * @LastEditTime: 2019-09-03 17:01:00 +0800
 * @LastEditors: 
 * @Description: 
 */ 
#include "head.h"
#include "actions.h"
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
    if (read(Fd_of_Send, file_name, Size) < 0)
    {
        perror("Server Recieve Data Failed:");
        return;
    }
    printf("%s\n",file_name);
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
   while ((length = read(Fd_of_Send, buffer, Size)) > 0)
    {
       printf("%s\n",buffer);       
       if(strcmp(buffer,"EOF")==0)
       {
            goto tt;
       }
        printf("Len=%d\n",length);
        fprintf(fp,buffer);
        // if (fwrite(buffer, sizeof(char), length, fp) < length)
        // {
        //     printf("File:\t%s Write Failed\n", file_name);
        //   //  flag = 0;
        //     break;
        // }
        // printf("YES\n");
        bzero(buffer, Size);
        // puts("你怎么回事？");
    }
   tt: printf("Length=%d\n",length);
   // if (flag)
        // 接收成功后，关闭文件，关闭socket
        printf("Receive File:\t%s From Server IP Successful!\n", file_name);
    close(fp);
}
int main(void)   
{   
  // 声明并初始化一个服务器端的socket地址结构   
  struct sockaddr_in server_addr;   
  bzero(&server_addr, sizeof(server_addr));   
  server_addr.sin_family = AF_INET;   
  server_addr.sin_addr.s_addr = htons(INADDR_ANY);   
  server_addr.sin_port = htons(Port);   
  
  // 创建socket，若成功，返回socket描述符   
  int server_socket_fd = socket(PF_INET, SOCK_STREAM, 0);   
  if(server_socket_fd < 0)   
  {   
    perror("Create Socket Failed:");   
    exit(1);   
  }   
  int opt = 1;   
  setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));   
    
  // 绑定socket和socket地址结构   
  if(-1 == (bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))))   
  {   
    perror("Server Bind Failed:");   
    exit(1);   
  }   
      
  // socket监听   LENGTH_OF_LISTEN_QUEUE
  if(-1 == (listen(server_socket_fd, 20)))   
  {   
    perror("Server Listen Failed:");   
    exit(1);   
  }   
    
  while(1)   
  {   
    // 定义客户端的socket地址结构   
    struct sockaddr_in client_addr;   
    socklen_t client_addr_length = sizeof(client_addr);   
    
    // 接受连接请求，返回一个新的socket(描述符)，这个新socket用于同连接的客户端通信   
    // accept函数会把连接到的客户端信息写到client_addr中   
    int new_server_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_addr, &client_addr_length);   
    if(new_server_socket_fd < 0)   
    {   
      perror("Server Accept Failed:");   
      break;   
    }   
    Rec_File_From_Client(new_server_socket_fd);
    // 关闭与客户端的连接   
    close(new_server_socket_fd);   
  }   
  // 关闭监听用的socket   
  close(server_socket_fd);   
  return 0;   
}   

