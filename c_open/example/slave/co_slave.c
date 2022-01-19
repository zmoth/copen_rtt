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

#include "slave.h"
#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>
#include <rtdevice.h>

int co_slave(int argc, char * argv[])
{
    uint32_t bitrate = CAN1MBaud;

    switch ((int) argc)
    {
    case 3:
        bitrate = atoi(argv[2]);
        break;
    case 2:
        break;
    default:
        printf("usage: %s <canif> <bitrate>\n", argv[0]);
        return -1;
        break;
    }

    return slave_init(argv[1], bitrate);
}

#ifdef RT_USING_FINSH
MSH_CMD_EXPORT(co_slave, slave sample: co_slave <canif|bitrate>);
#endif

