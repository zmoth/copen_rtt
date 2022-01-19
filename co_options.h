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
 
#ifndef CO_OPTIONS_H
#define CO_OPTIONS_H

#ifndef MAX_NODES
#define MAX_NODES       (2)
#endif

#ifndef MAX_EMCY_COBIDS
#define MAX_EMCY_COBIDS (11)
#endif

#ifndef MAX_HEARTBEATS
#define MAX_HEARTBEATS  (5)
#endif

#ifndef MAX_PDO_ENTRIES
#define MAX_PDO_ENTRIES (2)
#endif

#ifndef MAX_TX_PDO
#define MAX_TX_PDO      (5)
#endif

#ifndef MAX_RX_PDO
#define MAX_RX_PDO      (5)
#endif

#ifndef MAX_ERRORS
#define MAX_ERRORS      (10)
#endif

#endif /* CO_OPTIONS_H */
