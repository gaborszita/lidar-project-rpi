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

#include "motors.h"
#include "tcp.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ioctl.h>

char recvbuf[DEAFULT_BUFLEN];
char * recvbufpointer = recvbuf;
int recvbuflen;

int Connect()
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
    return new_socket;
    /*while (true) {
        printf("before\n");
        recv(server_fd, recvbuf, DEAFULT_BUFLEN, 0);
        printf("rbuf: %c\n", recvbuf[0]);
        if(strcmp(recvbuf, "computer-hello")==0){

            break;
        }
    }*/
    return server_fd;
    /*valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");*/
}

int tcpmsgreceive(int client)
{
	// Read the first batch of the TcpServer response bytes.
		//do {
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
		//} while(bytes==0);
	/*if (iResult == 0) {
	return;
	}*/
	return 0;
	// Close everything.
	//stream.Close();
	//client.Close();
}

void updaterecvbuffer(int client) {
	if (recvbufpointer >= recvbuf + recvbuflen)
	{
		tcpmsgreceive(client);
		recvbufpointer = recvbuf;
	}
}

bool tcpchangebuffer(char msgsearch[], int client)
{
	updaterecvbuffer(client);
	if (strcmp(recvbufpointer, msgsearch) == 0)
	{
		recvbufpointer += strlen(msgsearch) + 1;
		//if (recvbufpointer >= recvbuf + recvbuflen - 1) printf("recvbufpointer overrun E %d", recvbufpointer);
		return true;
	}
	else
	{
		return false;
	}
}

int checkforrecv(int client, AdafruitMotorsDriver *motors){
    
    if (tcpchangebuffer((char*)"exitprogram", client))
	{
		
        exit(0);
	}
	else if (tcpchangebuffer((char*)"motorsrun", client))
	{
        //printf("motorsrun\n");
        int speeds[4];

        sscanf(recvbufpointer, "%d", &speeds[0]);
        //printf("%d\n", speed);
        recvbufpointer += strlen(recvbufpointer) + 1;

        sscanf(recvbufpointer, "%d", &speeds[1]);
        //printf("%d\n", speed);
        recvbufpointer += strlen(recvbufpointer) + 1;

        sscanf(recvbufpointer, "%d", &speeds[2]);
        //printf("%d\n", speed);
        recvbufpointer += strlen(recvbufpointer) + 1;

        sscanf(recvbufpointer, "%d", &speeds[3]);
        //printf("%d\n", speed);
		recvbufpointer += strlen(recvbufpointer) + 1;

        motors->motorControl(speeds);

        return 4;
	}
    return 0;
}

