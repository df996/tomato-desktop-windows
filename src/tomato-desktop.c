//
// Created by Administrator on 2023/5/14.
//

#include <stdio.h>
#include "client/td_screen.h"

int main() {
    td_screen_rect *rect = td_screen_get_rect();
    free(rect);
    return 0;
}
