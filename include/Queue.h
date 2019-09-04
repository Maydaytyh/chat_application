/*
 * @Copyright: CSDepartment,BIT
 * @Author: Tian Yuhang
 * @File name: 
 * @Version: 
 * @Date: 2019-09-04 15:14:57 +0800
 * @LastEditTime: 2019-09-04 22:12:28 +0800
 * @LastEditors: 
 * @Description: 
 */
#include"head.h"
/************************定义区**********************/
#define MsgSize 512
//发送请求
typedef struct Send_Request{
    char Message[102400];
    int Type;
}Send_Req;
int MsgId=-1;
static pthread_mutex_t Lock_Of_Queue;
void QueuInit();
void PushMessage(Send_Req Msg);
void GetMessage(int Id);
void DestroyQueue(int Id);