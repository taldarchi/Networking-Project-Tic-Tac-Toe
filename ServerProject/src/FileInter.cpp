/*
 * FileInter.cpp
 *
 *  Created on: Apr 3, 2016
 *      Author: user
 */

#include "FileInter.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

using namespace std;



void FileInter::close(){
	::shutdown(socket_fd,SHUT_RDWR);
	::close(socket_fd);
	socket_fd=-1;
}

FileInter::~FileInter(){

}

int FileInter::read(char* buffer, int length){
	return ::read(socket_fd,buffer,length);
}


int FileInter::write(const char* buffer, int length){
	return ::write(socket_fd,buffer,length);
}

int FileInter::getSocketFd(){
	return socket_fd;
}


