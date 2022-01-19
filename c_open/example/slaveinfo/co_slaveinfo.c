/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2017 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

#include "slaveinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

int co_slaveinfo(int argc, char * argv[])
{
    uint8_t node = 1;
    uint32_t bitrate = CAN1MBaud;

    switch ((int) argc)
    {
    case 4:
        node = atoi(argv[2]);
        bitrate = atoi(argv[3]);
        break;
    case 3:
        node = atoi(argv[2]);
        break;
    case 2:
        break;
    default:
        printf("usage: %s <canif> <node> <bitrate>\n", argv[0]);
        return -1;
        break;
    }
    return slaveinfo(argv[1], node, bitrate);
}
#ifdef RT_USING_FINSH
MSH_CMD_EXPORT(co_slaveinfo, slaveinfo sample: co_slaveinfo <canif|node|bitrate>);
#endif

int getSlaveInfo(void){
    slaveinfo("can1", 2, CAN1MBaud);
    return RT_EOK;
}
INIT_APP_EXPORT(getSlaveInfo);
