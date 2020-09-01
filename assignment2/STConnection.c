//
//  STConnection.c
//  assignment2
//
//  Created by admin on 2018-02-13.
//  Copyright © 2018 Maxim Puchkov. All rights reserved.
//

#include "STConnection.h"

// Helper function to read parameters
long *_indicesOfSpaces(char *src) {
    const char SPACE = ' ';
    static long spaces[20];
    char *temp = strchr(src, SPACE);
    long index = 0;
    while (temp != NULL) {
        spaces[index] = temp - src;
        index++;
        temp = strchr(temp + 1, SPACE);
    }
    spaces[index] = strlen(src);
    return spaces;
}

/* Public Interface Implementation */

STConnection STConnectionCreate(char *src) {
    STConnection con;
    con.src = src;
    
    // If not valid, abort
    STConnectionValidate(&con);
    if (!con.valid) return con;
    
    long *indices = _indicesOfSpaces(con.src);
    long start;
    long end;
    long length;
    
    // Get Port
    start = indices[0];
    end = indices[1];
    length = (end > start) ? end - start : 0;
    static char port[BUFFER_PORT];
    memcpy(port, con.src + start + 1, length - 1);
    port[length - 1] = '\0';
    con.port = port;
    //printf("PORT IS: {%s}\n", port);
    
    // Get remote machine name
    start = indices[1];
    end = indices[2];
    length = (end > start) ? end - start : 0;
    static char remote_name[BUFFER_REMOTE_NAME];
    memcpy(remote_name, con.src + start + 1, length - 1);
    remote_name[length - 1] = '\0';
    con.remoteName = remote_name;
    //printf("remote_name IS: {%s}\n", remote_name);
    
    // Get remote machine port
    start = indices[2];
    end = indices[3];
    length = (end > start) ? end - start : 0;
    static char remote_port[BUFFER_PORT];
    memcpy(remote_port, con.src + start + 1, length);
    remote_port[length - 1] = '\0';
    con.remotePort = remote_port;
    //printf("remote_port IS: {%s}\n", remote_port);
    
    STConnectionInit(&con);
    
    return con;
}

STConnection STConnectionCreateArgv(const char **argv) {
    STConnection con;
    con.valid = true;
    
    con.port = (char *)argv[1];
    con.remoteName = (char *)argv[2];
    con.remotePort = (char *)argv[3];
    
    STConnectionInit(&con);
    
    return con;
}

// Confirm the command begins with "s-talk "
void STConnectionValidate(STConnection *con) {
    char *type = TYPE;
    size_t typeLength = strlen(type);
    size_t srcLength = strlen(con->src);
    bool valid = false;
    if (srcLength >= typeLength + 1) {
        valid = true;
        int i = 0;
        for (i = 0; i < typeLength; i += 1) {
            char srcChar = *(con->src + i);
            char typeChar = *(type + i);
            if (typeChar != srcChar) {
                valid = false;
                break;
            }
        }
        if (*(con->src + i) != ' ') {
            valid = false;
        }
    }
    con->valid = valid;
}

// Initialize connection (bind, listen)
void STConnectionInit(STConnection *con) {
    
    con->exists = true;
    con->connected = false;
    con->remotefd = PARAM_INVALID;
    con->sockfd = PARAM_INVALID;
    
    memset(&con->hints, 0, sizeof(con->hints));
    con->hints.ai_family = AF_UNSPEC;
    con->hints.ai_socktype = SOCK_STREAM;
    
    int x;
    x = getaddrinfo(NULL, con->port, &con->hints, &con->res);
    con->sockfd = socket(con->res->ai_family,
                         con->res->ai_socktype,
                         con->res->ai_protocol);
    x = bind(con->sockfd, con->res->ai_addr, con->res->ai_addrlen);
    x = listen(con->sockfd, BACKLOG);
    
    // other port
    x = getaddrinfo(NULL, con->remotePort, &con->hints, &con->res);
    con->remotefd = socket(con->res->ai_family,
                          con->res->ai_socktype,
                          con->res->ai_protocol);
    
}

// Connect to remote socket file descriptor (remotefd)
void STConnectionConnect(STConnection *con) {
    
    if (con->remotefd == PARAM_INVALID) return;
    
    int status = connect(con->remotefd, con->res->ai_addr, con->res->ai_addrlen);
    
    if (status == -1) {
        magenta();
        printf("Awaiting connection from %s...", con->remoteName);
        format_reset();
    } else {
        con->connected = true;
        magenta();
        printf("You are connected. Say hi! :)");
        format_reset();
    }
    
}

// Read parameters when inputted as string
// Format:
// s-talk [<port> <remote_name> <remote_port>]
char *STConnectionReadParameter(STConnection *con, int index) {
    char *param = NULL;
    long *indices = _indicesOfSpaces(con->src);
    long start = indices[index];
    long end = indices[index + 1];
    long length = end - start;
    memcpy(param, con->src + start, length);
    param[length] = '\0';
    return param;
}

