/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-03 17:42:45 +0800
 * @LastEditTime: 2019-09-04 01:26:29 +0800
 * @LastEditors: 
 * @Description: 
 */
#include <netinet/in.h> // sockaddr_in
#include <sys/types.h>  // socket
#include <sys/socket.h> // socket
#include <stdio.h>		// printf
#include <stdlib.h>		// exit
#include <string.h>		// bzero
#include <pwd.h>
#define SERVER_PORT 8000
#define BUFFER_SIZE 102400
#define FILE_NAME_MAX_SIZE 102400
#define Size 102400
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
void Rec_File_From_Server(int Client_Socket_Fd)
{
	char Filename[Size], Buf[Size];
	bzero(Filename, Size);
	bzero(Buf, Size);
	// receiveMSG(Fd_client,Filename,Size,0);
	struct passwd *pwd;
	pwd = getpwuid(getuid());
	char User_name[Size];
	strcpy(User_name, pwd->pw_name);
	if (send(Client_Socket_Fd, User_name, Size, 0) < 0)
	{
		perror("You User_name Is Wrong:");
		exit(0);
	}
	// scanf("%s",Filename);
	if (recv(Client_Socket_Fd, Filename, Size, 0) < 0)
	{
		perror("Get The Name Error:");
		exit(0);
	}
	FILE *fp = fopen(Filename, "w");
	int len;
	if (NULL == fp)
	{
		printf("File:\t%s Can Not Open To Write!Please Check The Path\n", Filename);
		exit(1);
	}
	bzero(Buf, Size);
	int flag = 1;
	while ((len = recv(Client_Socket_Fd, Buf, Size, 0)) > 0)
	{
		if (fwrite(Buf, sizeof(char), len, fp) < len)
		{
			printf("Receive %s Failed\n", Filename);
			flag = 0;
			break;
		}
		bzero(Buf, Size);
	}
	if (flag)
	{
		printf("Receive %s Successfully!\n", Filename);
	}
	fclose(fp);
}
int main()
{
	// 声明并初始化一个客户端的socket地址结构
	struct sockaddr_in client_addr;
	bzero(&client_addr, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = INADDR_ANY;
	client_addr.sin_port = htons(0);

	// 创建socket，若成功，返回socket描述符
	int client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket_fd < 0)
	{
		perror("Create Socket Failed:");
		exit(1);
	}
	// 声明一个服务器端的socket地址结构，并用服务器那边的IP地址及端口对其进行初始化，用于后面的连接
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	// if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) == 0)
	// {
	// 	perror("Server IP Address Error:");
	// 	exit(1);
	// }
	server_addr.sin_addr.s_addr = inet_addr("192.168.43.8");
	server_addr.sin_port = htons(SERVER_PORT);
	socklen_t server_addr_length = sizeof(server_addr);

	// 向服务器发起连接，连接成功后client_socket_fd代表了客户端和服务器的一个socket连接
	if (connect(client_socket_fd, (struct sockaddr *)&server_addr, server_addr_length) < 0)
	{
		perror("Can Not Connect To Server IP:");
		exit(0);
	}
	// Send_File_To_Server(client_socket_fd);
	Rec_File_From_Server(client_socket_fd);
	close(client_socket_fd);
	return 0;
}
