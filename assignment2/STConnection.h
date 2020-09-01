//
//  STConnection.h
//  assignment2
//
//  Created by admin on 2018-02-13.
//  Copyright © 2018 Maxim Puchkov. All rights reserved.
//

#ifndef STConnection_h
#define STConnection_h

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <errno.h>
#include "STInputFormat.h"

#define TYPE "s-talk"
#define BACKLOG 10
#define PARAM_INVALID -1
#define BUFFER_PORT 16
#define BUFFER_MESSAGE 1024
#define BUFFER_REMOTE_NAME 64

typedef struct STConnection {
    bool exists;
    bool valid;
    bool connected;
    char *src;
    char *remoteName;
    char *port;
    char *remotePort;
    int sockfd;
    int remotefd;
    struct addrinfo hints;
    struct addrinfo *res;
    struct sockaddr_in sockaddr_local;
    struct sockaddr_in sockaddr_remote;
} STConnection;

/* Public Interface */

STConnection STConnectionCreate(char *);

STConnection STConnectionCreateArgv(const char **);

void STConnectionValidate(STConnection *);

void STConnectionInit(STConnection *);

void STConnectionConnect(STConnection *);

char *STConnectionReadParameter(STConnection *, int);

#endif /* STConnection_h */
