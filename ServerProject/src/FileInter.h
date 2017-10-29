/*
 * FileInter.h
 *
 *  Created on: Apr 3, 2016
 *      Author: user
 */

#ifndef FILEINTER_H_
#define FILEINTER_H_

#include <netinet/in.h>
#include <inttypes.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;


class FileInter {
protected:
	int socket_fd;

public:
	virtual void close();
	virtual ~FileInter();
	virtual int read(char* buffer, int length);
	virtual int write(const char* buffer, int length);
	int getSocketFd();
};


#endif /* FILEINTER_H_ */
