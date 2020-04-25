/* 
* lidar project
* 
* Copyright (C) 2017-2020 Gabor Szita
* 
* This file is part of lidar project.
*
* Lidar project is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 3 as published by
* the Free Software Foundation.
*
* Lidar project is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with lidar project.  If not, see https://www.gnu.org/licenses/ 
*/ 

#ifndef _TCP_H
#define _TCP_H

#include "navigation.h"

//int Connect();
//int tcpmsgreceive(int client);
//void updaterecvbuffer(int client);
//bool tcpchangebuffer(char msgsearch[], int client);
//int checkforrecv(int client, Navigation *navigation);
//void sendString(const char sendID[], const char sendMessage[], int *socket, char buf[100000], char* bufptr);


#define DEAFULT_BUFLEN 100000

class TCP
{
private:
    char recvbuf[DEAFULT_BUFLEN];
    char * recvbufpointer = recvbuf;
    int recvbuflen;
    int client;

private:
    void updaterecvbuffer();
    int tcpmsgreceive();

public:
    int Connect();
    bool tcpchangebuffer(char msgsearch[]);
    int checkforrecv(Navigation *navigation);
    void sendString(const char sendID[], const char sendMessage[], char buf[100000], char* bufptr);
    int sendData(char buf[], int len);
};


#endif
