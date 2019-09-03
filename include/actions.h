/*
 * @Copyright: CS of BIT
 * @Author: 王占坤
 * @File name: 
 * @Version: 
 * @Date: 2019-09-01 10:03:32 +0800
 * @LastEditTime: 2019-09-03 10:54:35 +0800
 * @LastEditors: 
 * @Description: 
 */
#ifndef __ACTIONS_H__
#define __ACTIONS_H__
#include "head.h"

int sendDataToServer(char* data);

// void reUse(socketfd skf);

void createConnection(socketfd skf, struct sockaddr* serv_addr, size_t addr_len);

socketfd createSocket(int type, int protocol);

void initialzeSocketaddr(struct sockaddr_in *addr, char *ip_addr, int port);

void bindSocketAddr(socketfd skf_socketfd, struct sockaddr *addr_sockaddr, int addrlen);

void createListen(socketfd skf, int num);

socketfd acceptConnection(socketfd sfk, struct sockaddr *addr, socklen_t len_addr);

int sendMSG(socketfd, char* buff, size_t n_bytes, int flag);

int receiveMSG(socketfd skf, char *buff, size_t max_bytes, int flag);

void writeBack(socketfd skf, const void *buff, size_t n_bytes, int flag);

#endif