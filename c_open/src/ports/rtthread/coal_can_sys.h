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
/**
 * @author moth
 * @date   2022.01.16
 * @version V1.0.0
 */
#ifndef COAL_CAN_SYS_H
#define COAL_CAN_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

#define OS_CHANNEL

#include <osal_sys.h>

typedef struct
{
   os_device_t handle;
   void (*callback) (void * arg);
   void * arg;
} os_channel_t;

#ifdef __cplusplus
}
#endif

#endif /* COAL_CAN_SYS_H */
