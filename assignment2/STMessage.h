//
//  STMessage.h
//  assignment2
//
//  Created by admin on 2018-02-23.
//  Copyright © 2018 Maxim Puchkov. All rights reserved.
//

#ifndef STMessage_h
#define STMessage_h

#include <stdio.h>

#define STMESSAGE_SENDER_SELF 0
#define STMESSAGE_SENDER_OTHER 1

typedef struct STMessage {
    char *src;
    int sender;
} STMessage;

STMessage STMessageInit(char *, int);

#endif /* STMessage_h */
