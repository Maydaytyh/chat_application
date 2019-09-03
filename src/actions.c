/*
 * @Copyright: CS of BIT
 * @Author: 王占坤
 * @File name: 
 * @Version: 
 * @Date: 2019-08-30 21:22:06 +0800
 * @LastEditTime: 2019-09-03 13:12:12 +0800
 * @LastEditors: 
 * @Description: 
 */
#include "head.h"
#include "actions.h"
#define SERVER_PORT 8888
#define SERVER_ADDR "0.0.0.0"
// #define TEST
// // #define UDP
// #define TCP
// #define SOCKET_PROTOCOL 0

// #ifdef UDP
// #define SOCKET_TYPE SOCK_DGRAM
// #endif

// #ifdef TCP
// #define SOCKET_TYPE SOCK_STREAM
// #endif


//////////////////不允许外部调用的函数//////////////

/**
 * @Author: 王占坤
 * @Description: 不允许外部调用
 * @Param: 
 * @Return: 
 * @Throw: 
 */
void reUse(socketfd skf)
{
    INT16 optval = 1;
    int res = setsockopt(skf, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    if(res == -1)
    {
        perror("setsockopt");
    }
}





///////////////允许外部调用的函数////////////////////


int sendDataToServer(char *data)
{
    socketfd skf = createSocket(SOCK_STREAM, 0);
    struct sockaddr_in ser_addr;
    initialzeSocketaddr(&ser_addr, SERVER_ADDR, SERVER_PORT);
    createConnection(skf, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    sendMSG(skf, data, strlen(data), 0);
    // printf("%s\n",data);
    // printf("%d\n",strlen(data));
    close(skf);
}




/**
 * @Author: 王占坤
 * @Description: 在skf上创建一个到serv_addr的连接
 * @Param: socketfd skf, 需要创建连接的socket句柄
 * @Param: struct sockaddr* serv_addr 连接的目标地址
 * @Param: int addr_len 目标地址大小
 * @Return: 
 * @Throw: 
 */
void createConnection(socketfd skf, struct sockaddr *serv_addr, size_t addr_len)
{
    if (serv_addr == NULL)
    {
        printf("地址不存在！\n");
    }
    int res = connect(skf, serv_addr, addr_len);
    if (res == -1)
    {
        perror("connect");
        printf("create connection error: %s(errno: %d)\n", strerror(errno), errno);
        exit(1);
    }
}

/**
 * @Author: 王占坤
 * @Description: 创建socket进程
 * @Param: int type SOCK_STREAM是TCP模式，SOCK_DGRAM是UDP模式
 * @Param: int protocol 一般传入0即可
 * @Return: socketfd 返回创建的socket
 * @Throw: 
 */
socketfd createSocket(int type, int protocol)
{
    socketfd skf = socket(AF_INET, SOCK_STREAM, 0);
    if (skf == -1)
    {
        perror("socket");
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit(1);
    }
    else
    {
        reUse(skf);
        return skf;
    }
}

/**
 * @Author: 王占坤
 * @Description: 初始化地址
 * @Param: struct sockaddr_in* addr  待初始化的地址指针
 * @Param：in_addr_t ip_addr, ip地址,NULL代表自动获取ip
 * @Param: int port， 端口号
 * @Return: void
 * @Throw: 
 */
void initialzeSocketaddr(struct sockaddr_in *addr, char *ip_addr, int port)
{
    memset(addr, 0, sizeof(addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    if (ip_addr == NULL)
        addr->sin_addr.s_addr = htonl(INADDR_ANY);
    else
        addr->sin_addr.s_addr = inet_addr(ip_addr);
}

/**
 * @Author: 王占坤
 * @Description: 将socket绑定至指定地址
 * @Param: socketfd* skf_socketfd 待被绑定地址的socket句柄
 * @Param：const struct sockaddr* addr_sockaddr被绑定的地址
 * @Param：int addrlen 地址长度
 * @Return: void
 * @Throw: 
 */
void bindSocketAddr(socketfd skf_socketfd, struct sockaddr *addr_sockaddr, int addrlen)
{
    int res = bind(skf_socketfd, addr_sockaddr, addrlen);
    if (res == -1) //失败
    {
        perror("bind");
        exit(1);
    }
}

/**
 * @Author: 王占坤
 * @Description: 对需要进行监听的socket开启监听
 * @Param: socketfd& skf, 需要进行监听的socket
 * @Param：int num, 最大监听数量
 * @Return: void
 * @Throw: 
 */
void createListen(socketfd skf, int num)
{
    int res = listen(skf, num);
    if (res == -1) //执行失败
    {
        perror("listen");
        exit(1);
    }
}

/**
 * @Author: 王占坤
 * @Description: 监听到有连接时进行连接
 * @Param: 
 * @Return: 
 * @Throw: 
 */
socketfd acceptConnection(socketfd sfk, struct sockaddr *addr, socklen_t len_addr)
{
    //判断是否连接如果可以连接返回新建的
    socketfd cli_skf = accept(sfk, addr, &len_addr);
    if (cli_skf == -1)
    {
        printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
        exit(1);
    }
    return cli_skf;
}

/**
 * @Author: 王占坤
 * @Description: 从buff向skf发送n_bytes个字节的数据
 * @Param: socketfd skf  目标socket
 * @Param: char* buff  缓冲区
 * @Param: size_t n_bytes 要发送的字节数目
 * @Param: flag 一般默认为0即可
 * @Return: int 0:成功
 * @Throw: 
 */
int sendMSG(socketfd skf, char *buff, size_t n_bytes, int flag)
{
    if (n_bytes >= BUFSIZ)
        n_bytes = BUFSIZ - 1;
    if (n_bytes < 0)
        n_bytes = 0;

    int num_of_sending_words = send(skf, buff, n_bytes, flag);
    if (num_of_sending_words == -1) //失败
    {
        printf("receive message error: %s(errno: %d)\n", strerror(errno), errno);
        exit(1);
    }

    if (num_of_sending_words >= BUFSIZ)
        num_of_sending_words = BUFSIZ - 1;
    buff[num_of_sending_words] = 0;
    return 0;
}

/**
 * @Author: 王占坤
 * @Description: 从socket接受信息放到buff区，最大字节数为max_size
 * @Param: socketfd skf  信息源socket
 * @Param: char *buff  用于存储的缓冲
 * @Param: size_t n_bytes 最大接受多少字节的信息
 * @Param: int flag 一般传入为0即可
 * @Return: int 0:成功
 * @Throw: 
 */
int receiveMSG(socketfd skf, char *buff, size_t n_bytes, int flag)
{
    memset(buff, 0, BUFSIZ);
    if (n_bytes > BUFSIZ)
        n_bytes = BUFSIZ - 1;
    int num_of_reading_words = recv(skf, buff, n_bytes, flag);
    printf("num_of_reading_words: %d\n", num_of_reading_words);
    printf("%s\n", buff);
    if (num_of_reading_words == -1) //失败
    {
        printf("receive message error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }
    if (num_of_reading_words >= BUFSIZ)
        num_of_reading_words = BUFSIZ - 1;
    buff[num_of_reading_words] = 0;
    return 0;
}

/**
  * @Author: 王占坤
  * @Description: 向socket回传数据
  * @Param: socketfd skf 目标socket
  * @Param: const void* buff  需要发送的数据指针
  * @Param: size_t n_bytes  需要发送的字节数
  * @Param: int flag  一般传入0即可
  * @Return: void
  * @Throw: 
  */
void writeBack(socketfd skf, const void *buff, size_t n_bytes, int flag)
{
    if (!fork())
    {
        int res = send(skf, buff, n_bytes, flag);
        if (res == -1)
        {
            printf("write back error: %s(errno: %d)", strerror(errno), errno);
            exit(0);
        }
    }
}

#ifdef TEST

int main(int argc, char *argv[])
{

    connect(int fd, (struct sockaddr *)&addr, socklen_t len_addr);

    char buff[1024];
    struct sockaddr_in addr;

    initialzeSocketaddr(&addr, NULL, PORT);

    socketfd server_socket, client_socket;

    server_socket = createSocket(SOCK_STREAM, 0);
    int reuse = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopet error \n");
        return -1;
    }

    bindSocketAddr(server_socket, (struct sockaddr *)&addr, sizeof(addr));

    // printf("%d\n", listen(server_socket, 5));
    createListen(server_socket, 5);

    client_socket = acceptConnection(server_socket, (struct sockaddr *)NULL, 0);
    // acceptConnection(&client_socket, &server_socket, (struct sockaddr*)NULL, NULL);

    receiveMSG(client_socket, buff, 100, 0);
    printf("%s\n", buff);

    // sendMSG(client_socket, buff, 100, 0);

    printf("%s\n", buff);
    // writeBack(client_socket, buff, sizeof(buff), 0);

    close(client_socket);

    close(server_socket);
    // printf("hello world\n");

    return 0;
}

#endif