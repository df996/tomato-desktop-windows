//
// Created by cheng on 2023/5/18.
//

#ifndef TOMATO_DESKTOP_WINDOWS_TD_RDP_H
#define TOMATO_DESKTOP_WINDOWS_TD_RDP_H

#include <stdint.h>

#ifndef TD_RDP_VERSION
#define TD_RDP_VERSION 1.0
#endif

typedef struct {
    uint32_t magic;
    uint16_t version;
    uint16_t command;
    uint32_t data_length;
    char data;
} td_rdp;
#endif //TOMATO_DESKTOP_WINDOWS_TD_RDP_H
