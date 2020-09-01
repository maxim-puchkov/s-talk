//
//  STMessage.c
//  assignment2
//
//  Created by admin on 2018-02-23.
//  Copyright © 2018 Maxim Puchkov. All rights reserved.
//

#include "STMessage.h"

STMessage STMessageInit(char *src, int sender) {
    STMessage message;
    message.src = src;
    message.sender = sender;
    return message;
}
