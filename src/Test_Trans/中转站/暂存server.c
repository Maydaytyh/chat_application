 /*char file_name[Size], buffer[Size];
    bzero(file_name, Size);
    // bzero(buffer, Size);
    if (recv(new_server_socket_fd, file_name, Size, 0) < 0)
    {
        perror("Server Recieve Data Failed:");
        return;
    }
    puts(file_name);
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
	while ((length = recv(new_server_socket_fd, buffer, Size, 0)) > 0)
	{
		printf("Length=%d\n",length);
        if (fwrite(buffer, sizeof(char), length, fp) < length)
		{
			printf("File:\t%s Write Failed\n", file_name);
			break;
		}
		bzero(buffer, Size);
	}
    printf("Len=%d\n",length);
	// 接收成功后，关闭文件，关闭socket 
	printf("Receive File:\t%s From Server IP Successful!\n", file_name);
*/
