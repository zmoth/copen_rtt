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
 
#ifndef OPTIONS_H
#define OPTIONS_H

#include "co_options.h"

#ifndef LOG_LEVEL
#define LOG_LEVEL            (LOG_LEVEL_DEBUG)
#endif

#ifndef CO_ALLOC_LOG
#define CO_ALLOC_LOG         (LOG_STATE_ON)
#endif

#ifndef CO_CAN_LOG
#define CO_CAN_LOG           (LOG_STATE_ON)
#endif

#ifndef CO_SDO_LOG
#define CO_SDO_LOG           (LOG_STATE_ON)
#endif

#ifndef CO_OD_LOG
#define CO_OD_LOG            (LOG_STATE_ON)
#endif

#ifndef CO_NMT_LOG
#define CO_NMT_LOG           (LOG_STATE_ON)
#endif

#ifndef CO_EMCY_LOG
#define CO_EMCY_LOG          (LOG_STATE_ON)
#endif

#ifndef CO_HEARTBEAT_LOG
#define CO_HEARTBEAT_LOG     (LOG_STATE_ON)
#endif

#ifndef CO_NODE_GUARD_LOG
#define CO_NODE_GUARD_LOG    (LOG_STATE_ON)
#endif

#ifndef CO_LSS_LOG
#define CO_LSS_LOG           (LOG_STATE_ON)
#endif

#ifndef SDO_TIMEOUT
#define SDO_TIMEOUT          (1000)
#endif

#ifndef CO_THREAD_PRIO
#define CO_THREAD_PRIO       (10)
#endif

#ifndef CO_THREAD_STACK_SIZE
#define CO_THREAD_STACK_SIZE (2048)
#endif

#endif  /* OPTIONS_H */
