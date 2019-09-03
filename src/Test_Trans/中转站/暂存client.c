/*char buffer[BUFFER_SIZE];   
  bzero(buffer, BUFFER_SIZE);   
  // 输入文件名 并放到缓冲区buffer中等待发送   
  char file_name[FILE_NAME_MAX_SIZE+1];   
  bzero(file_name, FILE_NAME_MAX_SIZE+1);   
  printf("Please Input File Name:\t");   
  scanf("%s", file_name);   
   if(send(client_socket_fd, file_name, BUFFER_SIZE, 0) < 0)   
  {   
    perror("Send File Name Failed:");   
    exit(1);   
  }  
  FILE *fp=fopen(file_name,"r");
  if(NULL == fp)   
    {   
      printf("File:%s Not Found\n", file_name);   
    }   
    else  
    {   
      bzero(buffer, BUFFER_SIZE);   
      int length = 0;   
      // 每读取一段数据，便将其发送给客户端，循环直到文件读完为止   
      while((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)   
      {   
        if(send(client_socket_fd, buffer, length, 0) < 0)   
        {   
          printf("Send File:%s Failed./n", file_name);   
          break;   
        }   
        bzero(buffer, BUFFER_SIZE);   
      }   
    
      // 关闭文件   
      fclose(fp);   
      printf("File:%s Transfer Successful!\n", file_name);   
    }   */
