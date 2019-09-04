/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-04 15:14:57 +0800
 * @LastEditTime: 2019-09-04 23:02:37 +0800
 * @LastEditors: 
 * @Description: 
 */
#include"head.h"
/************************定义区**********************/
#define MsgSize 512
#define BUFFER_SIZE 1024
//发送请求
typedef struct Send_Request{
    char Message[102400];
    int Type;
}Send_Req;
int MsgId=-1;
static pthread_mutex_t Lock_Of_Queue;
void QueuInit();
void PushMessage(char Msg[]);
void GetMessage(char Msg[]);
void DestroyQueue(int Id);