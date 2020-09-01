//
//  STInputFormat.h
//  assignment2
//
//  Created by admin on 2018-02-23.
//  Copyright © 2018 Maxim Puchkov. All rights reserved.
//

#ifndef STInputFormat_h
#define STInputFormat_h

#include <stdio.h>

// Color table:
// http://web.theurbanpenguin.com/adding-color-to-your-output-from-c/

#define PREFIX   "\e"
#define RED      "[0;31m"
#define BRED     "[1;31m"
#define GREEN    "[0;32m"
#define BGREEN   "[1;32m"
#define MAGENTA  "[0;35m"
#define BMAGENTA "[1;35m"
#define RESET    "[0m"
#define RESETN   "[0m\n"

void red(void);

void bold_red(void);

void green(void);

void bold_green(void);

void magenta(void);

void bold_magenta(void);

void format_reset(void);

void format_specify(char *);

#endif /* STInputFormat_h */
