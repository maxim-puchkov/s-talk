//
//  main.c
//  assignment2
//
//  Created by admin on 2018-02-12.
//  Copyright © 2018 Maxim Puchkov. All rights reserved.
//

#include <stdio.h>
#include <pthread.h>

#include <stdlib.h>
#include "STConnection.h"
#include "STMessage.h"
#include "STInputFormat.h"
#include "LIST.h"

#define END_STR "!"

STConnection con;
LIST *received;
LIST *sent;
pthread_mutex_t mutex_received = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_sent = PTHREAD_MUTEX_INITIALIZER;



void *keyboardEventListener(void *state) {
    bool *done = (bool *)state;
    bool readInput = true;
    char line[BUFFER_MESSAGE];
    
    // If there is no connection, wait until user initiates one
    while (!con.valid) {
        scanf("%1023[^\n]%*c", line);
        if (strcmp(line, END_STR) == 0) {
            readInput = false;
            break;
        }
        con = STConnectionCreate(line);
        if (con.valid) STConnectionConnect(&con);
    }
    
    
    // Read user input and add it to the list of to-be-sent messages
    // Stop when input is "!"
    while (readInput && !*done) {
        scanf("%1023[^\n]%*c", line);
        readInput = strcmp(line, END_STR) != 0;
        if (con.connected) {
            STMessage msg = STMessageInit(line, STMESSAGE_SENDER_SELF);
            pthread_mutex_lock(&mutex_sent);
            ListAdd(sent, &msg);
            pthread_mutex_unlock(&mutex_sent);
        }
    }
    
    // Wait until all messages have been sent and displayed to the screen.
    // Signal other threads to stop executing and invalidate connection
    while (!readInput && !*done) {
        if (sent->length <= 0 && received->length <= 0) {
            *done = true;
            con.exists = false;
            con.connected = false;
            break;
        }
    }
    
    return NULL;
}



void *connectionHandler(void *state) {
    bool *done = (bool *)state;
    
    // Add connections when someone connects to the listening port
    while (!*done) {
        if (con.exists) {
            struct sockaddr_un their_addr;
            socklen_t addr_size;
            addr_size = sizeof(struct sockaddr_un);
            int remotefd = accept(con.sockfd, (struct sockaddr *)&their_addr, &addr_size);
            if (remotefd != -1) {
                con.remotefd = remotefd;
                con.connected = true;
                magenta();
                printf("You are connected. Say hi! :)");
                format_reset();
            }
        }
    }
    
    return NULL;
}



void *dataAnalysis(void *state) {
    bool *done = (bool *)state;
    
    // Receive messages
    while (!*done || sent->length > 0 || received->length > 0) {
        if (con.connected) {
            char buff[BUFFER_MESSAGE];
            ssize_t bytes = recv(con.sockfd, buff, BUFFER_MESSAGE, 0);
            if (bytes == -1) bytes = recv(con.remotefd, buff, BUFFER_MESSAGE, 0);
            if (bytes > 0) {
                pthread_mutex_lock(&mutex_received);
                STMessage message = STMessageInit(buff, STMESSAGE_SENDER_OTHER);
                ListAdd(received, &message);
                pthread_mutex_unlock(&mutex_received);
            }
            // Clear excess characters
            memset(buff + bytes, 0, sizeof(buff));
        }
    }
    
    return NULL;
}



void *dataTransmitter(void *state) {
    bool *done = (bool *)state;
    
    // Send messages if there are any in the list
    while (!*done || sent->length > 0) {
        if (con.connected) {
            pthread_mutex_lock(&mutex_sent);
            if (sent->length > 0) {
                void *item = ListTrim(sent);
                STMessage message = *(STMessage *)item;
                send(con.remotefd, message.src, strlen(message.src), 0);
            }
            pthread_mutex_unlock(&mutex_sent);
        }
    }
    
    return NULL;
}



void *dataOutput(void *state) {
    bool *done = (bool *)state;
    
    // Output messages to the scrren if there are any in the list
    // When other user sends "!", stop the program
    while (!*done || received->length > 0) {
        if (con.connected) {
            pthread_mutex_lock(&mutex_received);         // lock
            if (received->length > 0) {
                void *item = ListTrim(received);
                STMessage *message = (STMessage *)item;
                green();
                printf("%s", message->src);
                format_reset();
                if ((strcmp(message->src, END_STR) == 0)) {
                    magenta();
                    printf("Your S-Talk session has been terminated.\n");
                    printf("Press Return to leave.");
                    format_reset();
                    *done = true;
                }
            }
            pthread_mutex_unlock(&mutex_received);      // unlock
        }
    }

    return NULL;
}



int main(int argc, const char *argv[]) {
    
    // Initialize variables
    bool state = false;
    sent = ListCreate();
    received = ListCreate();
    
    // Show welcome message
    magenta();
    printf("Welcome to Simple-Talk");
    format_reset();

    // Read command line arguments
    if (argc == 4) {
        con = STConnectionCreateArgv(argv);
        if (con.valid) STConnectionConnect(&con);
    } else {
        // User is also allowed to connect after the program starts
        if (argc > 2) printf("Command line arguments invalid :(\n");
        printf("Input the following:\n");
        magenta();
        printf("s-talk <your_port> <remote_name> <remote_port>");
        format_reset();
        printf("Or input \"%s\" to terminate S-Talk\n", END_STR);
    }
    
    // References to five (5) threads
    pthread_t keyboardEventListenerThread;  // Reads keyboard input
    pthread_t connectionHandlerThread;      // Accepts all incoming connections for the listening port
    pthread_t dataAnalysisThread;           // Receives messages from other sockets
    pthread_t dataTransmitterThread;        // Transmits messages to connected socket
    pthread_t dataOutputThread;             // Outputs data to the screen
    
    // Create all five (5) threads
    if (pthread_create(&keyboardEventListenerThread, NULL, keyboardEventListener, &state)
        || pthread_create(&connectionHandlerThread, NULL, connectionHandler, &state)
        || pthread_create(&dataAnalysisThread, NULL, dataAnalysis, &state)
        || pthread_create(&dataTransmitterThread, NULL, dataTransmitter, &state)
        || pthread_create(&dataOutputThread, NULL, dataOutput, &state))
    {
        fprintf(stderr, "Error creating threads\n");
        return 1;
    }
    
    // Wait for three (3) threads to finish
    if (pthread_join(keyboardEventListenerThread, NULL)
        || pthread_join(dataTransmitterThread, NULL)
        || pthread_join(dataOutputThread, NULL)
        )
    {
        fprintf(stderr, "Error joining threads\n");
        return 2;
    }
    
    
    
    format_reset();
    
    // Close connection to remote socket before finishing
    shutdown(con.remotefd, SHUT_RDWR);
    
    return 0;
    
}
