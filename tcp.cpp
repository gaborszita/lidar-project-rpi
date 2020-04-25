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


// Server side C/C++ program to demonstrate Socket programming

#define PORT 19984

#define SOCKET_ERROR -1

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ioctl.h>


#include "motors.h"
#include "tcp.h"


int TCP::Connect()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
      
    // Creating socket file descriptor
    //tcp: SOCK_STREAM
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    /*if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | -1, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }*/
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons( PORT );
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    client=new_socket;
    return 0; 
}

int TCP::tcpmsgreceive()
{
    ioctl(client, FIONREAD, &recvbuflen);
    if (recvbuflen != 0) {
        recvbuflen = recv(client, recvbuf, DEAFULT_BUFLEN, 0);
    }
    else{
        recvbuf[0] = '\0';
    }
    if (recvbuflen == SOCKET_ERROR) {
        printf("SOCKET_ERROR");
        shutdown(client, 2);
        return 1;
    }
	return 0;
}

void TCP::updaterecvbuffer() {
	if (recvbufpointer >= recvbuf + recvbuflen)
	{
		tcpmsgreceive();
		recvbufpointer = recvbuf;
	}
}

bool TCP::tcpchangebuffer(char msgsearch[])
{
	updaterecvbuffer();
	if (strcmp(recvbufpointer, msgsearch) == 0)
	{
		recvbufpointer += strlen(msgsearch) + 1;
		return true;
	}
	else
	{
		return false;
	}
}

int TCP::checkforrecv(Navigation *navigation){
    
    if (tcpchangebuffer((char*)"exitprogram"))
	{
		
        exit(0);
	}
	else if (tcpchangebuffer((char*)"motorsrun"))
	{
        int speeds[4];

        sscanf(recvbufpointer, "%d", &speeds[0]);
        recvbufpointer += strlen(recvbufpointer) + 1;

        sscanf(recvbufpointer, "%d", &speeds[1]);
        recvbufpointer += strlen(recvbufpointer) + 1;

        sscanf(recvbufpointer, "%d", &speeds[2]);
        recvbufpointer += strlen(recvbufpointer) + 1;

        sscanf(recvbufpointer, "%d", &speeds[3]);
		recvbufpointer += strlen(recvbufpointer) + 1;

        navigation->motorsRun(speeds);

        return 4;
	}
    return 0;
}

void TCP::sendString(const char sendID[], const char sendMessage[], char buf[100000], char* bufptr){
    bufptr = buf; 
    sprintf(bufptr, sendID);
    bufptr+=strlen(bufptr)+1;
    sprintf(bufptr, sendMessage);
    bufptr+=strlen(bufptr)+1;
    send(client, buf, bufptr-buf, 0);
}

int TCP::sendData(char buf[], int len)
{
    if(!send(client, buf, len, 0))
    {
        printf("tcp send failed\n");
        return 1;
    }
    return 0;
}
