//
//  STInputFormat.c
//  assignment2
//
//  Created by admin on 2018-02-23.
//  Copyright © 2018 Maxim Puchkov. All rights reserved.
//

#include "STInputFormat.h"

void _STInputFormatDisplay(char *color) {
    printf("%s%s", PREFIX, color);
}

void red() {
    _STInputFormatDisplay(RED);
}

void bold_red() {
    _STInputFormatDisplay(BRED);
}

void green() {
    _STInputFormatDisplay(GREEN);
}

void bold_green() {
    _STInputFormatDisplay(BGREEN);
}

void magenta() {
    _STInputFormatDisplay(MAGENTA);
}

void bold_magenta() {
    _STInputFormatDisplay(BMAGENTA);
}

void format_reset() {
    _STInputFormatDisplay(RESETN);
}

void format_specify(char *color) {
    _STInputFormatDisplay(color);
}
